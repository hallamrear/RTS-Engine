@ECHO OFF

set arg1=%1
cd %1
echo Resource Folder: %CD%
set fragDir=%1*.frag
echo frag Folder: %fragDir%
set vertDir=%1*.vert
echo vert Folder: %vertDir%
echo.
echo All .frags in resource folder:
cd %fragDir%
for /r %%f in (%fragDir%) do (
if "%%~xf"==".frag" (call:compile %%f %%~pf %%~nf))
echo.
echo All .verts in resource folder:
for /R %%f in (%vertDir%) do (
if "%%~xf"==".vert" (call:compile %%f %%~pf %%~nf))

goto:closeprogram

:compile
 SETLOCAL ENABLEDELAYEDEXPANSION
		SET fullpath=%~1
        SET folderpath=%~2
		SET filename=%~3
		SET output=%folderpath%%filename%.spv
				
		%VULKAN_SDK%\Bin\glslc.exe %fullpath% -o %output%
		if %errorlevel% == 0 (echo %filename% compiled to %filename%.spv) else goto:closeprogram
    ENDLOCAL
EXIT /B 0

:closeprogram
pause
exit