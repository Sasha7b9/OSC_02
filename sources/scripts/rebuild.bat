@echo off

cd keil
call rebuild.bat
cd ..

cd vs_keil_debug
call rebuild.bat
cd..

cd vs_win_debug
call rebuild.bat
cd..
