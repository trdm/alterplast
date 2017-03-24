// (с) Александр Орефков
var inFileName, intDir, outDir	// Стартовые параметры


var xml,	// Входной файл
	xmlProgID="MSXML2.DOMDocument.5.0"	// Версия парсера

var fso=new ActiveXObject("Scripting.FileSystemObject")

var nodeHead,	// Заготовка для вставки в создаваемые файлы
	nodeBody,	// Заготовка для вставки в создаваемые файлы
	nodeCommon,	// Узел xml[common]
	nodeDocument// Корень документа
	
	

var hhcFile					// Файл содержания

///////////////////////////////////////////////////////////////
// Вспомогательные методы
///////////////////////////////////////////////////////////////
function ShowText(txt){WScript.Echo(txt)}

var CreatedFiles=Array()	// Имена всех создаваемых файлов

function CreateFile(name)
{
	try{
		var file=fso.CreateTextFile(name)
		if(file)
		{
			CreatedFiles[name]=1
			return file
		}
	}catch(e){}
	return null
}

function FatalError(txt)
{
	ShowText(txt)

	if(hhkFile)
		hhkFile.Close()
	if(hhcFile)
		hhcFile.Close()

	for(var i in CreatedFiles)
		fso.DeleteFile(i, true)

	WScript.Quit(1)
}

// Получение атрибута узла
function GetAttrib(node, atrName)
{
	if(node.attributes.getNamedItem(atrName))
		return node.attributes.getNamedItem(atrName).text
	return ""
}

// Установка атрибута тэга
function SetAttrib(node, atrName, atrVal)
{
	if (!node.attributes.getNamedItem(atrName))
	{
		var atr=xml.createAttribute(atrName)
		node.attributes.setNamedItem(atr)
	}
	node.attributes.getNamedItem(atrName).text=atrVal
}

////////////////////////////////////////////////////////////////////////////
// ОБЪЕКТ HHKNode
// Узел в дереве ключевых слов
////////////////////////////////////////////////////////////////////////////
var hhkRoot, hhkFile		// Для работы с индексами

// Конструктор узла ключевых слов
function HHKNode()
{
	this.anchors=Array()	// Массив пар - ключевое слово/массив ссылок
	this.childs=Array()		// Массив пар - ключевое слово/потомок
	// Назначаем функции
	this.AddKeyword=HHKNode_AddKeyword
	this.Save=HHKNode_Save
}

// Добавление ключевого слова
// Для разделения многоуровневых слов используется ','
// Например
// слово1
// слово1, слово2
// создаст такую структуру в индексе
// слово1
//     слово2
function HHKNode_AddKeyword(anchor, keyword)
{
	var idx=keyword.indexOf(",")	// Проверяем, есть ли подслова
	// Выделяем первое слово и обрезаем слева/справа
	var key = (idx<0?keyword:keyword.substr(0, idx)).replace(/^\s+|\s+$/g, "")
	if(!key.length)
		return

	// Если для слова еще нет массива ссылок, создадим его
	if(!this.anchors[key])
		this.anchors[key]=Array()
	// Теперь добавим ссылку в соответствующий слову массив
	// при этом ссылка добавляется или если нет подслов
	// или это первая ссылка для слова
	if(idx<0 || !this.anchors[key].length)
		this.anchors[key][this.anchors[key].length]=anchor

	// Если есть подслова, создадим потомка для данного слова
	// и добавим в него остаток строки
	if(idx>=0)
	{
		if(!this.childs[key])
			this.childs[key]=new HHKNode()
		this.childs[key].AddKeyword(anchor, keyword.substr(idx+1))
	}

}

// Сохранение дерева индексов в файл
function HHKNode_Save()
{
	// Начнем список
	hhkFile.Write('<UL>\r\n')
	
	// Переберем все добавленные слова
	for(var key in this.anchors)
	{
		// Для всех ключевых слов добавим пункт списка
		hhkFile.Write(
			'  <LI><OBJECT type="text/sitemap">\r\n'+
			'  <param name="Name" value="'+norm(key)+'">\r\n')
		// И впишем все ссылки для этого слова
		for(var anchor in this.anchors[key])
			hhkFile.Write('    <param name="Local" value="'+this.anchors[key][anchor]+'">\r\n')
		hhkFile.Write('    </OBJECT>\r\n')
		// Если у слова есть подслова, их тоже сохраним		
		if(this.childs[key])
			this.childs[key].Save()
	}
	// Закончили список
	hhkFile.Write('</UL>\r\n')
}

////////////////////////////////////////////////////////////////////////////
// ОБЪЕКТ DocSection
// Представляет собой секцию документа и хранит всякую инфу о нем
////////////////////////////////////////////////////////////////////////////

// Массивы секций, выносимых в топики.
// Заполняются при обработке содержания и построения дерева секций.
// Первый заполняется в порядке "Сначала потомки, потом узел"
// и служит для последующего "вынесения" секций в топики,
// чтобы сначала дочерние секции удалились из документа,
// а второй заполняется "Сначала узел, затем потомки",
// для организации навигации "Следующий/Предыдущий"
var SectionFromChild=Array(),
	SectionFromParent=Array()

var RootTopic,	// Корень иерархии секций
	RootTitle	// Общий заголовок документа

// Конструктор объекта
// Вызывается при первом обходе узла-секции документа, до
// обработки потомков
// parent - объект родитель
// node - узел секции документа
function DocSection(parent, node)
{
	// Назначим методы объекта
	this.AfterChildProcessed	= DocSection_AfterChildProcessed
	this.Save					= DocSection_Save
	this.MakeHref				= DocSection_MakeHref
	this.CombineHrefs			= DocSection_CombineHrefs
	this.ProcessKeywords		= DocSection_ProcessKeywords
	this.MakeSubst				= DocSection_MakeSubst
	this.InheritChmInfo			= DocSection_InheritChmInfo
	this.MakeToc				= DocSection_MakeToc
	
	// Помним о предках и о себе
	this.parent=parent
	this.node=node
	
	// Теперь получим инфу об chm-разметке
	// Если ее нет, создадим пустую
	this.chminfo=node.selectSingleNode("chminfo")

	if(!this.chminfo)
	{
		this.chminfo=xml.createElement("chminfo")
		if(node.hasChildNodes())
			node.insertBefore(this.chminfo, node.childNodes.item(0))
		else
			node.appendChild(this.chminfo)
	}

	// Сделаем замены		
	MakeReplaces(this.chminfo, ".//insert")
	
	// Теперь надо подготовить инфу о себе, получив
	// ее или из разметки, или от родителей
	this.InheritChmInfo()
	

	// Изначально считаем, что секция выносится в топик		
	this.IsTopic=true

	if(parent)
	{
		// Заголовок секции
		this.nTitle=node.selectSingleNode("*[name()!='chminfo']/descendant-or-self::*[starts-with(name(), 'h')]/a[@name]")
		if(!this.nTitle)
			FatalError("Not found title in "+node.xml)
		this.Title=this.nTitle.text
		this.Anchor=GetAttrib(this.nTitle, "name")
		// Выносить ли в топик
		if(this.chminfo.attributes.getNamedItem("split"))
		{
			if(this.chminfo.attributes.getNamedItem("split").text=="0")
				this.IsTopic=false
		}
		else
		{
			if(this.parent.chminfo.attributes.getNamedItem("splitchild"))
			{
				if(this.parent.chminfo.attributes.getNamedItem("splitchild").text=="0")
					this.IsTopic=false
			}
			else
				this.IsTopic=this.parent.IsTopic
		}
	}
	else
	{
		// Это самая первая секция
		// Заголовок секции
		this.Title=RootTitle
		// Проверим, что по умолчанию href не пуст
		// Хотя не знаю, может по умолчанию и не ставить ссылки?
		var hrefNode=this.chminfo.selectSingleNode("href")
		if(!hrefNode.hasChildNodes())
		{
			var a=xml.createElement("a")
			SetAttrib(a, "href", "self")
			var t=xml.createElement("title")
			SetAttrib(t, "to", "self")
			a.appendChild(t)
			var div=xml.createElement("div")
			div.appendChild(a)
			hrefNode.appendChild(div)
		}
	}
	// Запомним секции в порядке обхода содержания
	// от родителей к потомкам
	this.SectionNum=SectionFromParent.length
	SectionFromParent[this.SectionNum]=this

	// Определим имя файла, в котором будет лежать секция
	var t=this;
	while(!t.IsTopic)
		t=t.parent
	this.FileName=inFileName + t.SectionNum + ".html"
	this.ChildsContent=""
}

function DocSection_InheritChmInfo()
{
	if(!this.parent)
	{
		// Для корневой секции просто убедимся в наличии обязательных тэгов
		var inheritNodes=Array('decoration', 'href', 'hrefcombine', 'autokw')
		for(var i=0;i<inheritNodes.length;i++)
		{
			var iNode=this.chminfo.selectSingleNode(inheritNodes[i])
			if(!iNode)
				this.chminfo.appendChild(xml.createElement(inheritNodes[i]))
		}
	}
	else
	{
		// Для остальных секций надо просто скопировать
		// содержимое родительской секции, пропуская совпадающие
		// тэги и переместив все из <childs> в <chminfo>
		var childs=this.parent.chminfo.selectSingleNode("childs")
		if(childs)
		{
			// Из узла childs копируем все отсутствующее в секции
			for(var i=0;i<childs.childNodes.length;i++)
			{
				var curNode=childs.childNodes.item(i)
				if(curNode.baseName.length && !this.chminfo.selectSingleNode(curNode.baseName))
					this.chminfo.appendChild(curNode.cloneNode(true))
			}
		}
		
		// А затем откопируем все из потомков chminfo, пропуская некоторые узлы
		for(var i=0;i<this.parent.chminfo.childNodes.length;i++)
		{
			var curNode=this.parent.chminfo.childNodes.item(i)
			switch(curNode.baseName)
			{
			case 'childs':
			case 'alinks':
			case 'klinks':
			case 'nochilds':
			case 'toc':
				break;
			default:
				if(curNode.baseName.length && !this.chminfo.selectSingleNode(curNode.baseName))
					this.chminfo.appendChild(curNode.cloneNode(true))
			}
		}
	}
}

function MakeReplaces(node, tag)
{
	var arrExpr=Array()
	for(;;)
	{
		var replaces=node.selectNodes(tag)
		var curExpr=Array()
		var processed=0
		for(var cc=0;cc<replaces.length;cc++)
		{
			var repl=replaces.item(cc)
			var isProcessed=false
			var sNodes=GetAttrib(repl, "nodes")
			var asXml=GetAttrib(repl, "asxml")
			if(sNodes.length && !arrExpr[sNodes])
			{
				curExpr[sNodes]=1
				isProcessed=true
				var nList=node.selectNodes(sNodes)
				for(var i=0;i<nList.length;i++)
				{
					var newNode
					if(asXml=="1")
						newNode=xml.createTextNode(nList.item(i).xml)
					else
						newNode=nList.item(i).cloneNode(true)
					repl.parentNode.insertBefore(newNode, repl)
				}
			}
			sNodes=GetAttrib(repl, "node")
			if(sNodes.length && !arrExpr[sNodes])
			{
				curExpr[sNodes]=1
				isProcessed=true
				nList=node.selectSingleNode(sNodes)
				if(nList)
				{
					var newNode
					if(asXml=="1")
						newNode=xml.createTextNode(nList.xml)
					else
						newNode=nList.cloneNode(true)
					repl.parentNode.insertBefore(newNode, repl)
				}
			}
			repl.parentNode.removeChild(repl)
			if(isProcessed)
				processed++
		}
		if(!processed)
			break;
		for(var expr in curExpr)
			arrExpr[expr]=1
	}
}

/*
Следующий этап обработки
Вызывается при обходе содержания после обработки
всех дочерних для этой секции секций.
Основное назначение - правка ссылок в документе.
Так мы можем гарантровать, что не испортим ссылки,
уже обработанные в дочерних секциях

Алгоритм таков - выбираем все узлы <a>, содержащиеся в
узле секции, и имееющие атрибут name (якори).
Так как в этот момент из документа еще не удалены вынесенные
топики, то в выборку попадут и все якори из дочерних
секций. Но так как все обработанные якоря мы запоминаем в arLinks,
то пропуская их при обработке, получим только якоря в этой секции.
Затем для каждого якоря из выборки выбираем все узлы <a> в документе,
имеющие атрибут href="#"+name якоря. И правим у них href, добавляя
имя файла.
*/

var arLinks=Array()			// Массив имен обработанных ссылок

function DocSection_AfterChildProcessed()
{
	// Занесем секцию в обратный список
	SectionFromChild[SectionFromChild.length]=this
	
	// Выбираем все якоря в этой и дочерних секциях
	var targets=this.node.selectNodes("*[name()!='chminfo']/descendant-or-self::a[@name]|*[name()!='chminfo']/descendant-or-self::node()[@id]")
	for(var i=0;i<targets.length;i++)
	{
		var targ=targets.item(i)
		var lName=targ.attributes.getNamedItem(targ.baseName=="a"?"name":"id").text
		if(!arLinks[lName])	// Если якорь еще не был обработан
		{
			arLinks[lName]=1
			var newLink = this.FileName + ((this.IsTopic && lName==this.Anchor)?"":"#"+lName)
			var links=nodeDocument.selectNodes(".//a[@href='#"+lName+"']")
			for(var j=0;j<links.length;j++)
				SetAttrib(links.item(j), "href", newLink)
		}
	}
}

/*
Выполняет стандартные замены в chminfo
<a href="self|next|prev|parent[n]|home"> - <a href="FileName">
<title to="self|selfremove|next|prev|parent[n]|home"/> - "Topic Title"
<chmlink>
*/

function DocSection_MakeSubst()
{
	var p=this;
	while(!p.IsTopic)
		p=p.parent;
	var refSelf=p.SectionNum
	var refPrev=refSelf-1
	while(refPrev >= 0 && !SectionFromParent[refPrev].IsTopic)
		refPrev--
	var refNext=refSelf+1
	while(refNext<SectionFromParent.length && !SectionFromParent[refNext].IsTopic)
		refNext++
	
	var fnd=this.chminfo.selectNodes(".//a")
	for(var i=0;i<fnd.length;i++)
	{
		var a=fnd.item(i)
		var href=GetAttrib(a, "href")
		if(href=="self")
			SetAttrib(a, "href", p.FileName)
		else if(href=="next")
			SetAttrib(a, "href", refNext<SectionFromParent.length? SectionFromParent[refNext].FileName:"")
		else if(href=="prev")
			SetAttrib(a, "href", refPrev>=0? SectionFromParent[refPrev].FileName:"")
		else if(href=="home")
			SetAttrib(a, "href", SectionFromParent[0].FileName)
		else if(href.match(/^parent(?:\[\d+\]$|$)/))
		{
			var level=href.match(/\d+/)
			level=level?parseInt(level[0]):0
			var par=p.parent
			while(level && par)
			{
				par=par.parent
				level--
			}
			SetAttrib(a, "href", par?par.FileName:"")
		}
	}
	
	var removeTitle=false
	fnd=this.chminfo.selectNodes(".//title")
	for(var i=0;i<fnd.length;i++)
	{
		var t=fnd.item(i)
		var title=GetAttrib(t, "to")
		if(title=="self")
			t.parentNode.replaceChild(xml.createTextNode(p.Title), t)
		else if(title=="selfremove")
		{
			t.parentNode.replaceChild(xml.createTextNode(p.Title), t)
			removeTitle=true
		}
		else if(title=="prev")
			t.parentNode.replaceChild(xml.createTextNode(refPrev>=0? SectionFromParent[refPrev].Title:""), t)
		else if(title=="next")
			t.parentNode.replaceChild(xml.createTextNode(refNext<SectionFromParent.length? SectionFromParent[refNext].Title:""), t)
		else if(title=="home")
			t.parentNode.replaceChild(xml.createTextNode(SectionFromParent[0].Title), t)
		else if(title.match(/^parent(?:\[\d+\]$|$)/))
		{
			var level=title.match(/\d+/)
			level=level?parseInt(level[0]):0
			var par=p.parent
			while(level && par)
			{
				par=par.parent
				level--
			}
			t.parentNode.replaceChild(xml.createTextNode(par?par.Title:""), t)
		}
	}
	if(removeTitle && this.IsTopic && this.nTitle)
	{
		var nTitle=this.nTitle.parentNode
		nTitle.parentNode.removeChild(nTitle)
	}

	var chmLinks=this.chminfo.selectNodes(".//chmlink")
	for(var i=0;i<chmLinks.length;i++)
	{
		var link=chmLinks.item(i)
		var keywords=""
		var keys=link.selectSingleNode("keys")
		if(keys)
			keywords=keys.text.replace(/^\s+|\s+$/g, "")
		if(!keywords.length)
			link.parentNode.removeChild(link)
		else
		{
			var refName=GetAttrib(link, "type")
			var obj=xml.createElement("object")
			var idName="hhlink"+refName+i
			SetAttrib(obj, "id", idName)
			SetAttrib(obj, "classid", "clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11")
			SetAttrib(obj, "type", "application/x-oleobject")
			var param=xml.createElement("param")
			SetAttrib(param, "name", "Command")
			SetAttrib(param, "value", refName)
			obj.appendChild(param)
			param=xml.createElement("param")
			SetAttrib(param, "name", "Item1")
			SetAttrib(param, "value", "")
			obj.appendChild(param)
			keywords=keywords.split(/[\r\n]/)
			for(var j=0;j<keywords.length;j++)
			{
				param=xml.createElement("param")
				SetAttrib(param, "name", "Item"+(j+2))
				SetAttrib(param, "value", keywords[j].replace(/^\s+|\s+$/g, ""))
				obj.appendChild(param)
			}
			keys.parentNode.replaceChild(obj, keys)
			var a=link.selectNodes(".//a[@href='click']")
			for(var j=0;j<a.length;j++)
				SetAttrib(a.item(j), "href", "JavaScript:"+idName+".Click()")
			_CopyChilds(link, link, true)
		}
	}
}

// Функция формирует ссылку на этот топик
// и возвращает ее ввиде узла <hrefitem>
function DocSection_MakeHref()
{
	if(!this.parent)
		return null
	var href=this.chminfo.selectSingleNode("href")
	if(!href.hasChildNodes())
		return null
	var hrefinfo=xml.createElement("hrefinfo")
	for(var i=0;i<href.childNodes.length;i++)
		hrefinfo.appendChild(href.childNodes.item(i).cloneNode(true))
	return hrefinfo
}

function _CopyChilds(nSrc, nDst, bRemove)
{
	for(var k=0;k<nSrc.childNodes.length;k++)
		nDst.parentNode.insertBefore(nSrc.childNodes.item(k).cloneNode(true), nDst)
	if(bRemove)
		nDst.parentNode.removeChild(nDst)
}

function DocSection_CombineHrefs()
{
	var combine=this.chminfo.selectSingleNode("hrefcombine")

	var hrefs=this.node.selectNodes(".//hrefinfo")
	
	var refCombined=Array()
	for(var i=0;i<hrefs.length;i++)
	{
		var href=hrefs.item(i)
		refCombined[refCombined.length]=href
		if(i==hrefs.length-1 || !(href.nextSibling && href.nextSibling.baseName=="hrefinfo" && href.nextSibling.parentNode==href.parentNode))
		{
			var nCombine=combine.cloneNode(true)
			var foreach=nCombine.selectSingleNode(".//foreach")
			if(!foreach)
			{
				foreach=xml.createElement("foreach")
				nCombine.appendChild(foreach)
			}
			for(var j=0;j<refCombined.length;j++)
			{
				var nForEach=foreach.cloneNode(true)
				var nItem=nForEach.selectSingleNode(".//item")
				if(!nItem)
				{
					nItem=xml.createElement("item")
					nForEach.appendChild(nItem)
				}
				_CopyChilds(refCombined[j], nItem, true)
				_CopyChilds(nForEach, foreach, false)
			}
			foreach.parentNode.removeChild(foreach)
			_CopyChilds(nCombine, refCombined[0], true)
			for(var k=1;k<refCombined.length;k++)
				refCombined[k].parentNode.removeChild(refCombined[k])
			refCombined.length=0
		}
	}
}

function DocSection_ProcessKeywords()
{
	hrefFname=this.FileName
	if(!this.IsTopic)
		hrefFname+="#"+this.Anchor
	
	// Обработаем KLink	
	var aklinks=this.chminfo.selectNodes("klinks")
	for(var i=0;i<aklinks.length;i++)
	{
		var keys=aklinks.item(i).text.split(/[\r\n]/g)
		for(var k in keys)
			hhkRoot.AddKeyword(hrefFname, keys[k])
	}
	var akwMode=parseInt(GetAttrib(this.chminfo.selectSingleNode("autokw"), "mode"))

	if(akwMode & 1)
		hhkRoot.AddKeyword(hrefFname, this.Title.replace(/,/g, ""))
	if(akwMode & 2)
	{
		var keys=this.Title.split(/[ ,;:\/\\]/)
		for(var i in keys)
			hhkRoot.AddKeyword(hrefFname, keys[i])
	}
	
	// Теперь обработаем ALink
	aklinks=this.chminfo.selectNodes("alinks")
	if(!aklinks.length)
		return
	if(!this.IsTopic)
	{
		// ALink нельзя привязать к месту на странице,
		// поэтому просто перекинем их в родительский топик,
		// он еще не обработан
		var parent=this.parent
		while(!parent.IsTopic)
			parent=parent.parent
		for(var i=0;i<aklinks.length;i++)
			parent.chminfo.appendChild(aklinks.item(i))
	}
	else
	{
		// вставим объект
		var allkeys=""
		for(var i=0;i<aklinks.length;i++)
			allkeys+='\n'+aklinks.item(i).text
		var ob = xml.createElement("object")
		SetAttrib(ob, "type", "application/x-oleobject")
		SetAttrib(ob, "classid", "clsid:1e2a7bd0-dab9-11d0-b93a-00c04fc99f9e")
		var keys=allkeys.split(/[\r\n]/g)
		for(var i in keys)
		{
			var key=keys[i].replace(/^\s+|\s+$/g, "")
			if(key.length)
			{
				var para = xml.createElement("param")
				SetAttrib(para, "Name", "ALink Name")
				SetAttrib(para, "value", key)
				ob.appendChild(para)
			}
		}
		this.node.appendChild(ob)
	}
}

function DocSection_MakeToc(node)
{
	var res=""
	for(var i=0;i<node.childNodes.length;i++)
		res+=this.MakeToc(node.childNodes.item(i))

	switch(node.baseName)
	{
	case "folder":
		return '<LI><OBJECT type="text/sitemap">\r\n'+
				'<param name="Name" value="' + norm(GetAttrib(node, "name")) + '">\r\n'+
				'</OBJECT>\r\n<UL>'+res+"</UL>"
	case "merge":
		var file=GetAttrib(node, "file")
		return '</UL><OBJECT type="text/sitemap">\r\n'+
			'<param name="Merge" value="'+ file+'.chm::\\'+file+'.hhc">\r\n\r\n'+
			'</OBJECT>\r\n<UL>'
	case "self":
		this.cntTitle=this.chminfo.selectSingleNode("cnttitle")
		this.cntTitle=(this.cntTitle && this.cntTitle.hasChildNodes())?this.cntTitle.text:this.Title
		var fName=this.FileName
		if(!this.IsTopic)
			fName+="#"+this.Anchor
		if(res.length)
			res='<UL>\r\n' + res + '</UL>\r\n'
		return '<LI><OBJECT type="text/sitemap">\r\n'+
					'<param name="Name" value="' + norm(this.cntTitle) + '">\r\n'+
					'<param name="Local" value="' + fName +'">\r\n'+
					'</OBJECT>\r\n'+res
	case "childstoc":
		return this.ChildsContent
	}
	return res
}

function DocSection_Save()
{
	// Заменим subst'ы
	MakeReplaces(this.node, ".//subst")

	// Удалим chminfo
	this.chminfo.parentNode.removeChild(this.chminfo)

	// Сделаем замены
	this.MakeSubst()
	// Обработаем ключевые слова
	this.ProcessKeywords()
	// Обработаем ссылки из этой секции на дочерние
	this.CombineHrefs()

	// Обработка построения содержания
	var nContents=this.chminfo.selectSingleNode("toc")
	if(!nContents)
	{
		nContents=xml.createElement("toc")
		var self=xml.createElement("self")
		self.appendChild(xml.createElement("childstoc"))
		nContents.appendChild(self)
	}
	var Contents=this.MakeToc(nContents)

	if(this.parent)
		this.parent.ChildsContent+=Contents
	else
		this.ChildsContent=Contents
		
	// Если это топик, скинем в файл
	if(this.IsTopic)
	{
		// Подготовим ссылку на эту секцию
		var nHRef=this.MakeHref()
		if(nHRef)
			this.node.parentNode.replaceChild(nHRef, this.node)

		var xmlNew=new ActiveXObject(xmlProgID)
		var nnHtml = xmlNew.createElement("html")
		nnHtml.appendChild(nodeHead.cloneNode(true))

		var chmTitle=this.chminfo.selectSingleNode("chmtitle")
		chmTitle=(chmTitle && chmTitle.hasChildNodes())?chmTitle.text:this.cntTitle
		nnHtml.selectSingleNode("//title").text=chmTitle
		
		var bd = nodeBody.cloneNode(false)
		
		if(!this.parent && nodeCommon)
			nodeCommon.parentNode.removeChild(nodeCommon)
		
		var decor=this.chminfo.selectSingleNode("decoration")
		var ins=decor.selectSingleNode(".//topic")
		if(ins)
			ins.parentNode.replaceChild(this.node, ins)
		else
			decor.appendChild(this.node)
		
		for(var k=0;k<decor.childNodes.length;k++)
			bd.appendChild(decor.childNodes.item(k).cloneNode(true))

		nnHtml.appendChild(bd)
		xmlNew.appendChild(nnHtml)
		var file=CreateFile(intDir+this.FileName)
		file.Write(xmlNew.xml)
		file.Close()
	}
}

// Подготовка входного файла
function PrepeareFile(path)
{
	// Проверим существование файла
	try{
		var f=fso.GetFile(path)
	}catch(e)
	{
		FatalError("can't get file "+path+'\n'+e.Description)
	}

	// Запомним его каталог, это будет и каталог промежуточных файлов
	intDir=f.ParentFolder.Path
	if(!intDir.match(/\\$/))
		intDir+='\\'
	
	inFileName=f.Name.replace(/\.[^\.]*$/, "")
	f=null
	
	// Далее надо:
	// заменить codepage на windows-1251
	// убрать комментарии вокруг chminfo
	// записать файл
    var InFile = fso.OpenTextFile(path)

	var line=InFile.ReadLine()
	if(line.search(/cp1251/i)>=0)
	{
		// Если в строке есть CP1251, значит, файл только что создан
		// rest'ом, надо раскрыть комментарии.
		// Также удалим ссылки на всякие внешние схемы, правда
		// придется описать &nbsp;
		var inText = line.replace(/cp1251/i, "windows-1251")+
			'\r\n<!DOCTYPE rest [<!ENTITY nbsp "&#160;">]>\r\n<html>'
    	InFile.ReadLine()
		InFile.ReadLine()
		
		// Теперь будем искать codepage в заголовке <meta>
		while(!InFile.AtEndOfStream)
		{
			line = InFile.ReadLine()
			if(line.search(/cp1251/i)>=0)
			{
				inText += "\r\n" + line.replace(/cp1251/i, "windows-1251")
				break
			}
			inText += "\r\n" + line
		}
		
		// Дочитаем остальное из файла
		inText += "\r\n" + InFile.ReadAll()
		// И запишем обратно, убрав каменты вокруг chminfo
		InFile.Close()
		InFile = fso.CreateTextFile(path)
		InFile.Write(inText.
			replace(/<!--\s*(?=<chminfo)/gi, "").
			replace(/\/chminfo>\s*-->/gi, "/chminfo>").
			replace(/chminfo\/>\s*-->/gi, "chminfo/>"))
	}
    InFile.Close()
}

function norm(text)
{
	return text.replace(/&/g,"&amp;").replace(/"/g,"&quot;").replace(/</g, "&lt;").replace(/>/g, "&gt;")
}

function MakeSection(name, parent)
{
	var target=nodeDocument.selectSingleNode(".//a[@name='"+name.substr(1)+"']")
	while(target)
	{
		target=target.parentNode;
		if(target.baseName.toLowerCase()=="div" && GetAttrib(target, "class").match(/section/i))
			return new DocSection(parent, target)
	}
	return null
}

function ProcessContentList(node, parentSection)
{
    var childs=new Enumerator(node.childNodes)
    for(;!childs.atEnd();childs.moveNext())
    {
		var li=childs.item()
		if (li.baseName.toLowerCase()=="li")
		{
			var a=li.selectSingleNode("a")
			var section=MakeSection(GetAttrib(a, "href"), parentSection)

			var ul=li.selectSingleNode("ul")
			if(ul)
				ProcessContentList(ul, section)
			section.AfterChildProcessed()
		}
	}
}

// Функция обрабатывает содержание html-файла, и подготавливает
// дерево секций (DocSection) на его основе. Одновременно создается
// файл содержания для chm-проекта
function Split2Topic(content)
{
	ProcessContentList(content.selectSingleNode(".//ul"), RootTopic)
    RootTopic.AfterChildProcessed()
    
    content.parentNode.removeChild(content)
    
    for(var i in SectionFromChild)
		SectionFromChild[i].Save()
}

function MakeHHPFiles()
{
    var fName=intDir+inFileName+".hhc"
    hhcFile=CreateFile(fName)
    if(!hhcFile)
		FatalError("can't create content file: "+fName)
	hhcFile.Write('<HTML><UL>'+RootTopic.ChildsContent.replace(/<UL>\s*<\/UL>/gi, "")+'</UL></HTML>')
	hhcFile.Close();
	hhcFile=null
	
	var cntFileName=inFileName
	var defFile=RootTopic.FileName
	var MainTitle=RootTitle
	
	var collection=null
	if(nodeCommon)
	{
		collection=nodeCommon.selectSingleNode(".//common/collection[file[@name='"+inFileName+"']]")
		if(collection)
		{
			MainTitle=GetAttrib(collection, "title")
			var master=GetAttrib(collection, "master")
			if(master.length && master!=inFileName)
			{
				cntFileName=master+".chm::\\"+master
				defFile=cntFileName+"0.html"
			}
		}
	}

    output = CreateFile(intDir+inFileName + ".hhp")
    if(!output)
		FatalError("Don't create hhp file")
    output.WriteLine("[Options]")
    output.WriteLine("Auto Index = Yes")
    output.WriteLine("Binary TOC = Yes")
    output.WriteLine("Compatibility = 1.1 Or later")
    output.WriteLine("Compiled File = " + outDir + inFileName + ".chm")
    output.WriteLine("Contents File = " + inFileName + ".hhc")
    output.WriteLine("Default Window = Main")
    output.WriteLine("Default topic = " + RootTopic.FileName)
    output.WriteLine("Display compile progress=No")
    output.WriteLine("Full-text search=Yes")
    output.WriteLine("Index file=" + inFileName + ".hhk")
    output.WriteLine("Language=0x419 Русский")
    output.WriteLine("Title=" + RootTitle)
    output.WriteLine("")
    output.WriteLine("[Windows]")
    output.WriteLine('main="'+MainTitle+'","' + cntFileName + '.hhc","' + inFileName + '.hhk","'+
		defFile + '","' + defFile + '",,,,,0x32420,,0x60380e,,0xb0000,,,,,,0')
    output.WriteLine("")
    output.WriteLine("[Files]")

	var nTopics=0    
    for(var i in SectionFromParent)
    {
		if(SectionFromParent[i].IsTopic)
		{
			output.WriteLine(SectionFromParent[i].FileName)
			nTopics++
		}
	}
	if(collection)
	{
		var merge=collection.selectNodes("file[@name!='"+inFileName+"']")
		if(merge.length)
		{
			output.WriteLine("[MERGE FILES]")
			for(var i=0;i<merge.length;i++)
				output.WriteLine(GetAttrib(merge.item(i), "name") + ".chm")
		}
	}
    output.Close()

    hhkFile = CreateFile(intDir+inFileName + ".hhk")
    if(!hhkFile)
		FatalError("Don't create hhk file")
	hhkFile.WriteLine('<HTML>')
	hhkRoot.Save()
	hhkFile.Write('</HTML>')
	hhkFile.Close()
	ShowText("    Sections: "+SectionFromParent.length+", Topics: "+nTopics)
}

function ProcessAKlinkButtons(paramName, refName)
{
	var links=nodeDocument.selectNodes(".//a[@href='"+refName+"']")
	for(var id=0;id<links.length;id++)
	{
		var link=links.item(id)
		var refText=link.text.match(/[^\|]+/g)
		if(!refText || refText.length==1)
			ShowText("warning: "+link.xml+" not contain keywords")
		else
		{
			link.text=refText[0]
			var obj=xml.createElement("object")
			var idName="hhctr"+refName+id
			SetAttrib(obj, "id", idName)
			SetAttrib(obj, "classid", "clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11")
			SetAttrib(obj, "type", "application/x-oleobject")
			SetAttrib(obj, "width", "0")
			SetAttrib(obj, "height", "0")
			var param=xml.createElement("param")
			SetAttrib(param, "name", "Command")
			SetAttrib(param, "value", paramName)
			obj.appendChild(param)
			param=xml.createElement("param")
			SetAttrib(param, "name", "Item1")
			SetAttrib(param, "value", "")
			obj.appendChild(param)
			
			for(var i=1;i<refText.length;i++)
			{
				param=xml.createElement("param")
				SetAttrib(param, "name", "Item"+(i+1))
				SetAttrib(param, "value", refText[i])
				obj.appendChild(param)
			}
			SetAttrib(link, "href", "JavaScript:"+idName+".Click()")
			link.parentNode.insertBefore(obj, link)
		}
	}
}

function main()
{
	var argc=WScript.Arguments.Count()
	if(!argc)
		FatalError("usage: cscript.exe html2chm.js InFileName [outFilesDir]")
	
	// Готовим входной файл
	PrepeareFile(WScript.Arguments.item(0))
	
	outDir=intDir
	// Возможно задан выходной каталог в параметрах
	if(argc>1)
	{
		try{
			var fOutDir=fso.GetFolder(WScript.Arguments.item(1))
		}catch(e)
		{
			FatalError("can't get outdir "+WScript.Arguments.item(1)+'\n'+e.Description)
		}
		outDir=fOutDir.Path
	}
	// Дополним по необходимости слэшем
	if(!outDir.match(/\\$/))
		outDir+='\\'

	xml=new ActiveXObject(xmlProgID)
	if(!xml)
		FatalError("Error create xml doc")

    xml.async = false
    xml.validateOnParse = false
    xml.resolveExternals = false
    if(!xml.load(WScript.Arguments.item(0)))
		FatalError("not load xml\r\nline: "+xml.parseError.line+"\r\nreason: "+xml.parseError.reason)
	
	
	var nodeHtml=xml.documentElement	
	nodeHead=nodeHtml.selectSingleNode("head")
	nodeBody=nodeHtml.selectSingleNode("body").cloneNode(false)
	nodeDocument=nodeHtml.selectSingleNode("//div[@class='document']")
	nodeCommon=nodeHtml.selectSingleNode("//xml[common]")
		
	if(!nodeDocument)
		FatalError("not find document")
	// Удалим все каменты
	var comments=nodeHtml.selectNodes("//comment()")
	for(var i=0;i<comments.length;i++)
		comments.item(i).parentNode.removeChild(comments.item(i))

	var title=nodeDocument.selectSingleNode("h1[contains(@class, 'title')]")
	if(!title)
		FatalError("Not found title")
	RootTitle=title.text
	
	var content=nodeDocument.selectSingleNode("div[@class='contents topic']")
	if(!content)
		FatalError("Not found contents topic")
	
	// Сразу заменим во всем документе ссылки на klink/alink
	ProcessAKlinkButtons("ALink", "alink")
	ProcessAKlinkButtons("KLink", "klink")
	
	// Создадим корневую секцию
	//debugger
	RootTopic=new DocSection(null, nodeDocument)
	// Создадим корень для дерева индексов
	hhkRoot=new HHKNode()

	// Разобъем документ на топики
	Split2Topic(content)
	
	// Создадим файлы проекта
	MakeHHPFiles()
	
	// И напоследок файл зачистки
	var erasefile=fso.CreateTextFile(intDir+inFileName+"del.bat")
	erasefile.WriteLine("@echo off")
	for(var i in CreatedFiles)
		erasefile.WriteLine('del "'+i+'"')
	erasefile.Close()
}

main()
WScript.Quit(0)