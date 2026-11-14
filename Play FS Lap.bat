@echo off
rem Starts FS Lap. The game opens as its settings say (menu, SETTINGS): borderless full screen at the desktop's size on a first run.
rem No -fullscreen here: it forces exclusive full screen, which hangs the game when another window takes the focus.
rem Takes about a minute, several if the disk is busy or the PC has just restarted.
rem -LiveCoding=false: no Live Coding console window next to the game (a window popping up can blank a full-screen game for a moment).
rem UE_ROOT: your Unreal Engine 5.8 folder, if it is not D:\UE\UE_5.8.
if not defined UE_ROOT set "UE_ROOT=D:\UE\UE_5.8"
start "" "%UE_ROOT%\Engine\Binaries\Win64\UnrealEditor.exe" "%~dp0FSLapUE\FSLapUE.uproject" -game -LiveCoding=false %*
