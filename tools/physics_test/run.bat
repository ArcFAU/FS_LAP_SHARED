@echo off
rem Builds and runs the headless AI lap test (no Unreal needed). Arguments go to ailap: [arcade|free|city|events] [trace.csv]
setlocal
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -property installationPath`) do set VS=%%i
call "%VS%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>nul
set ROOT=%~dp0..\..
set CORE=%ROOT%\FSLapUE\Plugins\HoMoLapPhysics\Source\HoMoLapPhysics
set OUT=%ROOT%\data\physics_test
set FSLAP_CITY=%ROOT%\FSLapUE\Content\FS\Data\Erlangen.bin
if not exist "%OUT%" mkdir "%OUT%"
cl /nologo /EHsc /O2 /W4 /std:c++17 /DHOMOLAPPHYSICS_API= /I"%CORE%\Public" "%~dp0ailap.cpp" "%CORE%\Private\HoMoLapCore.cpp" /Fe"%OUT%\ailap.exe" /Fo"%OUT%\\" >"%OUT%\build.log"
if errorlevel 1 (type "%OUT%\build.log" & exit /b 1)
"%OUT%\ailap.exe" %*
