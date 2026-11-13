@echo off
rem Starts FS Lap full screen at the display's own resolution, so Windows display scaling can't push the picture off the screen.
rem Takes about a minute, several if the disk is busy or the PC has just restarted.
rem -LiveCoding=false: no Live Coding console window next to the game (a window popping up can blank a full-screen game for a moment).
rem UE_ROOT: your Unreal Engine 5.8 folder, if it is not D:\UE\UE_5.8.
if not defined UE_ROOT set "UE_ROOT=D:\UE\UE_5.8"
start "" "%UE_ROOT%\Engine\Binaries\Win64\UnrealEditor.exe" "%~dp0FSLapUE\FSLapUE.uproject" -game -fullscreen -LiveCoding=false %*
