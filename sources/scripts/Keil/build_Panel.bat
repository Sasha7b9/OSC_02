@echo off

echo Compile Panel     ... %TIME%

c:\Keil_v5\UV4\UV4 -b ..\..\Panel\PanelOSC.uvprojx -j0 -z -o PanelOSC.out
if ERRORLEVEL 0 goto Exit

echo ERROR!!! Device    failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop ..\..\Panel\PanelOSC.out -n1000 -x0 -y0

:Exit
