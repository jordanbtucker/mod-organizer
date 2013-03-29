call config_translations.cmd

FOR %%F IN (%TRANSLATIONS%) DO ^
wget http://translate.tannin.eu/export/modorganizer/%%F