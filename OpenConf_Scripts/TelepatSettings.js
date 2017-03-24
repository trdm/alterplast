/* ===================================================================================
	Скрипт: TelepatSettings.js
	Версия: $Revision: 1.4 $
	 
	Обеспечивает раздельное хранение настроек Телепата для каждой информационной
 базы (ИБ).

	Настройки хранятся в каталоге базы в текстовом файле telepat.prm.

	Глобальные настройки (которые используются по умолчанию в тех случаях, когда
для текущей базы файла настроек не существует) хранятся в файле telepat.prm в 
каталоге bin\config каталога установки 1С.

	При открытии конфигуратора для какой-либо ИБ сначала производится
попытка загрузить настройки Телепата, заданные для текущей базы. Если настройки
для текущей ИБ не заданы (не существует файла telepat.prm в каталоге ИБ), то
загружаются глобальные настройки.

	Для сохранения и загрузки настроек Телепата используются следующие макросы:

	SaveSettings()
		сохранить настройки Телепата для текущей ИБ

	LoadSettings()
		загрузить настройки Телепата для текущей ИБ
	                    
	SaveGlobalSettings()
		сохранить глобальные настройки Телепата

	LoadGlobalSettings()
		загрузить глобальные настройки Телепата

	Кроме того, по умолчанию скрипт автоматически сохраняет настройки Телепата
при закрытии конфигуратора. Если Вас не устраивает такое поведение, то 
установите значение переменной AutoSaveSettingsOnQuit (см. ниже) в значение false.

	Спасибо за использование скрипта :-)
		
	Автор: Александр Кунташов <kuntashov@yandex.ru>

=================================================================================== */

// сохранять настройки Телепата при закрытии конфигуратора
// true - сохранять, false - не сохранять

var AutoSaveSettingsOnQuit = true

var CoreObj = {

	telepat : null, 
	
	settings : ['Components', 'Language', 'UseStdMethodDlg', 'NoOrderMethodDlg', 'FilterMethodDlg', 
				'AutoParamInfo', 'ParamInfoAddMethDescr', 'ParamInfoAddParamDescr', 
				'AutoActiveCountSymbols', 'DisableTemplateInRemString', 'AddTemplate'],
			  
	Init : function () {
		this.telepat = Plugins('Телепат');
		if (!this.telepat) {
			// Телепат не установлен, выгружаем себя
			Scripts.Unload(SelfScript.Name);
		}
		// если не существует "локального" файла настроек, то 
		// используем глобальный
		if (!this.LoadSettings(IBDir)) {
			this.LoadSettings(BinDir + "config");
		}
	},

	Unload : function ()
	{
		this.telepat = null;
	},
	
	SaveSettings : function (dir)
	{		
		var file = this.OpenSettingsFile(dir, true);
		for (var i=0; i<this.settings.length; i++) {
			file.WriteLine('telepat.' + this.settings[i] + " = " + this.telepat[this.settings[i]]);
		}		
		file.Close();
	},

	LoadSettings : function (dir)
	{
		var file = this.OpenSettingsFile(dir, false);
		if (!file) return false;
		with (this) eval(file.ReadAll());
		file.Close();
		return true;
	},

	OpenSettingsFile : function (dir, forWriting)
	{
		with (new ActiveXObject('Scripting.FileSystemObject')) {
			var path = BuildPath(dir, 'telepat.prm');
			if (!(FileExists(path) || forWriting)) {
				return null;
			}
			return OpenTextFile(path, forWriting?2:1, true);
		}		
	}
}

function Configurator::OnQuit()
{
	if (AutoSaveSettingsOnQuit) {
		SaveGlobalSettings();
	}
	CoreObj.Unload();
}

function SaveSettings() { CoreObj.SaveSettings(IBDir) }
function LoadSettings() { CoreObj.LoadSettings(IBDir) }

function SaveGlobalSettings() { CoreObj.SaveSettings(BinDir) }
function LoadGlobalSettings() { CoreObj.LoadSettings(BinDir) }

CoreObj.Init()
