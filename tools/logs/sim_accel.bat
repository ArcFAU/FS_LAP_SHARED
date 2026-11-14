@echo off
rem Builds sim_accel (no Unreal needed; with the ADTM if its code is generated). The exe is data\logs\sim_accel.exe;
rem tools\logs\validate.py runs it.
setlocal
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -property installationPath`) do set VS=%%i
call "%VS%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>nul
set ROOT=%~dp0..\..
set CORE=%ROOT%\FSLapUE\Plugins\HoMoLapPhysics\Source\HoMoLapPhysics
set OUT=%ROOT%\data\logs
if not exist "%OUT%\obj" mkdir "%OUT%\obj"
cl /nologo /EHsc /O2 /W3 /std:c++17 /DHOMOLAPPHYSICS_API= /DHOMOLAP_WITH_ADTM=1 /I"%CORE%\Public" /I"%CORE%\Private" /I"%CORE%\Private\ADTM" "%~dp0sim_accel.cpp" "%CORE%\Private\HoMoLapCore.cpp" "%CORE%\Private\HoMoLapAdtm.cpp" "%CORE%\Private\ADTM\*.cpp" /Fe"%OUT%\sim_accel.exe" /Fo"%OUT%\obj\\" >"%OUT%\sim_accel_build.log"
if errorlevel 1 (type "%OUT%\sim_accel_build.log" & exit /b 1)
