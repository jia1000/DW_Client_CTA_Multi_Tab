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
::得到程序根目录
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
cd /d %~dp0
cd ..
SET AppDir=%cd%
cd install

::
::开机自动启动
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo 正在设置开机自动启动...

::start %cd%\BrowserApp.exe   %启动Dr. Wise Client%
::setlocal EnableDelayedExpansion 

reg add "HKLM\Software\Microsoft\Windows\CurrentVersion\Run" /v "Dr. Wise Client" /t REG_SZ /d "%AppDir%\BrowserApp.exe" /f   %将程序写进注册表,开机自启动%
::reg add "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run" /v "Dr. Wise Client" /t REG_SZ /d "%AppDir%\BrowserApp.exe" /f   %将程序写进注册表,开机自启动%

::
::将echo输出的内容写入到p.reg
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo 正在设置客户端自定义协议...

echo Windows Registry Editor Version 5.00 >p.reg
echo.
echo [HKEY_CLASSES_ROOT\dpclient] >>p.reg
echo @="URL:dpclient Protocol Handler" >>p.reg
echo "URL Protocol"="" >>p.reg
echo [HKEY_CLASSES_ROOT\dpclient\shell] >>p.reg
echo [HKEY_CLASSES_ROOT\dpclient\shell\open] >>p.reg
echo [HKEY_CLASSES_ROOT\dpclient\shell\open\command] >>p.reg
echo @="%AppDir:\=\\%\\BrowserApp.exe -risurl=%%1" >>p.reg

::
::执行p.reg文件
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo 正在写入注册表信息...

regedit /s p.reg

::
:: 删除临时生成的文件
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
del /q p.reg

echo 安装完成，按回车键关闭窗口......

::
:: 暂停查看echo的输出信息
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
pause>nul %>nul的作用是不显示请按任意键继续%