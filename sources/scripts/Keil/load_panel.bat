@echo off
@echo %TIME%   Load firmware to Panel ...

c:\Keil_v5\UV4\UV4 -f ..\..\Panel\PanelOSC.uvprojx -j0 -o ..\scripts\Keil\out.txt
