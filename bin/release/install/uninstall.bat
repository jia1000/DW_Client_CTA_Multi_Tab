@echo off

::
::获取管理员权限
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo 正在启用超级管理员权限... 
%1 %2
ver|find "5.">nul&&goto :st
mshta vbscript:createobject("shell.application").shellexecute("%~s0","goto :st","","runas",1)(window.close)&goto :eof

:st
copy "%~0" "%windir%\system32\"

::
::卸载，删除注册表信息
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo 正在删除注册表信息...

taskkill /F /IM browserapp.exe 2>nul
reg delete "HKLM\Software\Microsoft\Windows\CurrentVersion\Run" /v "Dr. Wise Client" /f 2>nul
reg delete "HKEY_CLASSES_ROOT\dpclient" /f 2>nul

echo 卸载完成，按回车键关闭窗口...

::
:: 暂停查看echo的输出信息
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
pause>nul %>nul的作用是不显示请按任意键继续%