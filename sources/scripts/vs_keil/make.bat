@echo off

@echo .
@echo %TIME%   Making Visual Studio Keil Debug ...

cd ../..
rmdir "generated/Keil" /s /q
cd scripts/vs_keil
cmake ../../CMakeLists.txt -B../../generated/Keil > cmake_out.txt

@echo %TIME%   Complete
