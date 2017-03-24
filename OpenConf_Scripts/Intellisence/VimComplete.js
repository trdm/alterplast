/*===========================================================================
Copyright (c) 2004-2005 Alexander Kuntashov
=============================================================================
Скрипт:  VimComplete.js
Версия:  1.4
Автор:   Александр Кунташов
E-mail:  kuntashov at yandex dot ru
ICQ UIN: 338758861
Описание: 
    Атодополнение слов в стиле редактора Vim
===========================================================================*/
/*
    Макрос NextWord()
            Подбирает часть слова слева от курсора и пытается дополнить его,
        ища вперед по тексту слова, с такой же левой частью. Подставляет 
        первое подходящее. Следующий вызов макроса подставит следующее за 
        первым найденным словом и так далее по кругу (дойдя до последней строки 
        модуля поиск продолжится с первой строки). 
        
    Макрос PrevWord()
            Тоже самое, только поиск слов осуществляется в обратном направлении.
   
    В классическом Vim используются следующие хоткеи:
    
        Ctrl + N  для дополнения с поиском вперед (следующее слово, Next word)
        Ctrl + P  для дополнения с поиском назад  (предыдущее слово, Previous word)
*/

/* ==========================================================================
                                    МАКРОСЫ
========================================================================== */

// следующее соответствие
function NextWord() // Ctrl + N
{
    completeWord();
}

// предыдущее соответствие
function PrevWord() // Ctrl + P
{
    completeWord(true);
}

/* ==========================================================================
                                IMPLEMENTATION                       
========================================================================== */

var CurDoc = null;
var VimComplete = new VimAutoCompletionTool;

function completeWord(lookBackward)
{
	if (!(CurDoc = CommonScripts.GetTextDocIfOpened(false, true))) {
		// если не окно не текстовое, то позволим отработать стандартному
		// 1С'овскому хоткею (если таковой назначен)
		Configurator.CancelHotKey = true;
		return;
	}
	VimComplete.completeWord(lookBackward);
	CurDoc = null;
}

/* Возвращает часть слова слева от 
    текущего положения курсора */
function getLeftWord(doc)
{
    var cl, word = '';
    cl = doc.Range(doc.SelStartLine);
    for (var i=doc.SelStartCol-1;
            (i>=0)&&cl.charAt(i).match(/[\wА-Яа-я]/i);
                word = cl.charAt(i--) + word)
        ;    
    return word;
}

/* Примитивный класс для выделения слов в строке, 
их фильтрации и последовательному перебору. 
Написано в лоб, так что больших скоростей не обещаю
(собственно, именно по этому я не разбираю сразу весь 
текст на слова, а делаю это построчно, лишая себя возможности
легко избавиться от повторяющихся соответствий, что мне кажется 
менее критичным в контексте юзабилити)*/

function Line(str)
{
// private 
    var s = str;
    var words = null;     
// public
    this.reset = function () 
    {
        words = s.split(/[^\wА-я]+/);
    }
    this.assign = function (ix) 
    {
        return ((typeof(words)=="object")&&(ix>=0)&&(ix<words.length));                  
    }
    this.word = function (ix) 
    {
        if (this.assign(ix)) return words[ix];
    }
    this.count = function ()
    {
        return words.length;        
    }
	this.words = function ()
	{
		return words;
	}
	/* возвращает объект-итератор с единственным методом next() 
	   c помощью которого осуществляется перебор строк (до тех пор, 
	   пока не вернет значение undefined, означающее конец списка */
	this.iterator = function (r)
	{
		var collection = this;
		return {
			collection	: collection,
			iterator	: r ? collection.count() : (-1),														 
			next		: function(reverse)
		    {        
				return this.collection.word( this.iterator += (reverse?(-1):1) );
		    }   
		}
	}
	/* фильтрует элементы, оставляя только те, значения которых
	   матчат шаблон pattern */
    this.filter = function (pattern, unique)
    {
        var used = {};
        if (this.assign(0)) {
            var nw = new Array();
            for (var i=0; i<this.count(); i++) {
                if (this.word(i).match(pattern)) {					
                    if (unique) {
                        if (!used[this.word(i)]) {
                            used[this.word(i)] = true;
                            nw[nw.length] = this.word(i);
                        }
                    }
                    else {
                       nw[nw.length] = this.word(i);
                    }
                }
            }
            words = nw;
            return true;
        }
        return false;
    }        
   
    this.reset(); // инициализация
}

/* По [моим] ощущениям практически повторяет поведение соответсвующих
функций Vim (я про ^P/^N), за исключением относительно
небольшого, но все же заметного минуса - повторяющиеся в 
разных строках соответствия не исключаются из списка 
подстановки. Пока меня это устраивает, хотя бы потому,
что скрипт используется совместно с Телепатом, а не сам по себе. 
Еще один небольшой баг заключается в том, что при дополнении
пустого слова в строку статуса выводится не совсем корректная
информация и к исходному (пустому) слову, к сожалению, не 
вернуться никак, но это совсем уж не существенно, хотя возможно,
как нибудь исправлю для красоты :-) */

function VimAutoCompletionTool(_) // orgy citadel :-)
{
// private

    var srcDocPath;	// путь до исходного документа (используется для идентификации документов)
    var srcLine;	// исходная строка документа
    var srcCol;		// первая позиция в строке перед исходным словом
    var srcWord;	// исходное слово (которое пытаемся дополнить)
    var lastWord;	// последнее использованное в подстановке слово
    var curLineIx;	// индекс текущей строки (из которой берутся соответствия)
    var words;		// список слов-соответствий текущей строки
    
    var backwardSearch;	// обратный поиск (по умолчанию поиск прямой, "вперед")
    var pattern;		// шаблон (регулярное выражение), описывающий соответствие исходному слову 
    
    var counter;	// счетчик соответствий
    var total;		// общее число соответствий

// public

    /* выполняет (ре)инициализацию объекта, если это необходимо */
    this.setup = function (lookBackward)
    {
        var word = getLeftWord(CurDoc);		
        if (this.isNewLoop(CurDoc, word)) { // реинициализация			
            srcDocPath	= CurDoc.Path;            
            srcLine		= CurDoc.SelStartLine;
            srcCol		= CurDoc.SelStartCol - word.length;            
            srcWord		= word;            
            lastWord	= word; // чтобы корректно сделать первую подстановку
            curLineIx	= srcLine;            
            pattern		= new RegExp("^" + word, "i");     
            // начинаем искать соответствия начиная с исходной строки
            words = this.parseLine(lookBackward ? this.leftPart() : this.rightPart());  
            // счетчики
            counter = 0;
            total = null;
        }               
        backwardSearch = lookBackward;
    }
	/* условие необходимости произвести переинициализацию 
        переменных членов объекта VimAutoCompletionTool */
    this.isNewLoop = function (doc, word)
    {
        return !(words 
			&& (srcDocPath	== doc.Path) 
            && (srcLine		== doc.SelStartLine) 
            && (lastWord	== word)
			&& (srcCol		== (doc.SelStartCol-word.length)));         
    }
	/* проверяет, не выходит ли индекс строки за допустимые границы */
    this.assign = function (lIx)
    {
        return (CurDoc&&(0<=lIx)&&(lIx<CurDoc.LineCount));
    }
	/* берет следующее соответствие и подставляет его на место исходного слова */
    this.completeWord = function (lookBackward)
    {       
		this.setup(lookBackward);
        while (true) {  
            var word = words.next(lookBackward);
            if (word) {
                this.complete(word);
                return;
            }
            words = this.nextLine();
        }
    }
	/* строит и возвращает список соответсвующих слов для 
		следующей по порядку строки */
    this.nextLine = function ()
    {                   
        curLineIx += (backwardSearch ? -1 : 1); 
		if (backwardSearch) {
			if (curLineIx < 0) {
				curLineIx = CurDoc.LineCount-1;
			}
		} 
		else {
            if (curLineIx == CurDoc.LineCount) {
                curLineIx = 0;
            }
         }
        return this.parseLine(this.curLine());               
    }
	/* "разбирает" переданную в качестве параметра строку на слова
		и фильтрует их в соотвествии с шаблоном, который описывает
		подходящие соответствия для исходного слова */
    this.parseLine = function (srcLine)
    {
        var w = new Line(srcLine);
        w.filter(pattern, true);       
        return w.iterator(backwardSearch);
    }
	/* выполняет подстановку очередного соответствия вместо исходного слова */
    this.complete = function (word)
    {        
        CurDoc.Range(srcLine) = this.leftPart() + word + this.rightPart();
        CurDoc.MoveCaret(srcLine, srcCol+word.length);
            
        lastWord = word;

        counter += backwardSearch ? -1 : 1;          
        if ((curLineIx == srcLine)&&(lastWord == srcWord)) {
            if ((!total)&&counter) {                                
                total = Math.abs(counter) - 1;                     
            }
            counter = 0;
        }       

        Status(counter 
            ? "Cоответствие: "+Math.abs(counter).toString()+(total?" из " + total:"")
            : (total ? "Исходное слово" : "Шаблон не найден")); 
    } 
    /* возвращает текущую строку */
    this.curLine = function ()
    {
        var str = "";
        if (this.assign(curLineIx)) {
            /* поскольку исходная строка у нас постоянно меняется,
                ее "собираем" отдельно, возвращая на место исходное слово */
            if (curLineIx == srcLine) {
                str = this.leftPart() + srcWord + this.rightPart();           
            }
            else {
                str = CurDoc.Range(curLineIx);
            }           
        }
        return str;
    }
    /* левая половина строки, содержащей исходное слово; 
        само исходное слово не включается */
    this.leftPart = function ()
    {
        return CurDoc.Range(srcLine, 0, srcLine, srcCol);
    }
    /* правая половина строки, содержащей исходное слово;
        само исходное слово не включается */
    this.rightPart = function ()
    {      
        return CurDoc.Range(
            srcLine, srcCol+lastWord.length,
            srcLine, CurDoc.LineLen(srcLine));
    }
}

/*
 * Процедура инициализации скрипта 
 */
function Init(_) // Фиктивный параметр, чтобы процедура не попадала в макросы
{    
    try {
        var c = null;
        if (!(c = new ActiveXObject("OpenConf.CommonServices"))) {
            throw(true); // вызываем исключение
        }        
        c.SetConfig(Configurator);
        SelfScript.AddNamedItem("CommonScripts", c, false);
    }
    catch (e) {
        Message("Не могу создать объект OpenConf.CommonServices", mRedErr);    
        Message("Скрипт " & SelfScript.Name & " не загружен", mInformation);
        Scripts.UnLoad(SelfScript.Name);
    }
}

Init(); // При загрузке скрипта выполняем его инициализацию
