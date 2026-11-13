@echo off
rem Builds and runs the headless ADTM test (no Unreal needed; needs the code from tools\adtm\adtm_codegen.m). Arguments go to adtmlap: [trace.csv]
setlocal
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -property installationPath`) do set VS=%%i
call "%VS%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>nul
set ROOT=%~dp0..\..
set CORE=%ROOT%\FSLapUE\Plugins\HoMoLapPhysics\Source\HoMoLapPhysics
set OUT=%ROOT%\data\physics_test\adtm
if not exist "%CORE%\Private\ADTM\adtm_indices.h" (echo no ADTM code: run tools\adtm\adtm_codegen.m first & exit /b 1)
if not exist "%OUT%" mkdir "%OUT%"
cl /nologo /EHsc /O2 /W3 /std:c++17 /DHOMOLAPPHYSICS_API= /DHOMOLAP_WITH_ADTM=1 /I"%CORE%\Public" /I"%CORE%\Private" /I"%CORE%\Private\ADTM" "%~dp0adtmlap.cpp" "%CORE%\Private\HoMoLapCore.cpp" "%CORE%\Private\HoMoLapAdtm.cpp" "%CORE%\Private\ADTM\*.cpp" /Fe"%OUT%\adtmlap.exe" /Fo"%OUT%\\" >"%OUT%\build.log"
if errorlevel 1 (type "%OUT%\build.log" & exit /b 1)
"%OUT%\adtmlap.exe" %*
