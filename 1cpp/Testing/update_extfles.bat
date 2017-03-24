Scripts\decompile_extfiles.py
cvs -z9 update -P -d -- UnitTests ExtForms 1>UpdateLog.txt
Scripts\compile_extfiles.py
start UpdateLog.txt