
@echo off

Rem Compressed files
SET "BUILD_ZIP=%~dp0_build.zip"
SET "RESOURCES_ZIP=%~dp0_resources.zip"
SET "THIRDPARTY_ZIP=%~dp0_thirdparty.zip"

Rem Uncompressed directories
SET "BUILD_DIR=%~dp0_build"
SET "RESOURCES_DIR=%~dp0_resources"
SET "THIRDPARTY_DIR=%~dp0_thirdparty"

Rem Destination directories
SET "DEST_PARENT_BUILD_DIR=%~dp0cpp-projects"
SET "DEST_PARENT_RESOURCES_DIR=%~dp0cpp-projects"
SET "DEST_PARENT_THIRDPARTY_DIR=%~dp0cpp-projects"
SET "DEST_BUILD_DIR=%DEST_PARENT_BUILD_DIR%\_build"
SET "DEST_RESOURCES_DIR=%DEST_PARENT_RESOURCES_DIR%\_resources"
SET "DEST_THIRDPARTY_DIR=%DEST_PARENT_THIRDPARTY_DIR%\_thirdparty"

SET "TOOLBOX_DIR=%~dp0toolbox"
SET "TOOLBOX_SCRIPT=%TOOLBOX_DIR%\install_dependencies.cmd"

Rem Tools
SET "UNZIP_EXE=%TOOLBOX_DIR%\tools\unzip.exe"




Rem Debug paths
rem echo %BUILD_ZIP%
rem echo %RESOURCES_ZIP%
rem echo %THIRDPARTY_ZIP%
rem echo %UNZIP_EXE%
rem echo %DEST_BUILD_DIR%
rem echo %DEST_RESOURCES_DIR%
rem echo %DEST_THIRDPARTY_DIR%

Rem Check if destination directories exist
IF NOT EXIST %DEST_PARENT_BUILD_DIR% (
	echo "Cannot find %DEST_PARENT_BUILD_DIR% directory"
	exit /b %ERRORLEVEL%
)
IF NOT EXIST %DEST_PARENT_RESOURCES_DIR% (
	echo "Cannot find %DEST_PARENT_RESOURCES_DIR% directory"
	exit /b %ERRORLEVEL%
)
IF NOT EXIST %DEST_PARENT_THIRDPARTY_DIR% (
	echo "Cannot find %DEST_PARENT_THIRDPARTY_DIR% directory"
	exit /b %ERRORLEVEL%
)

Rem Remove zip files if already existing
IF EXIST %BUILD_ZIP% (
	echo "Remove %BUILD_ZIP% existing zip file"
	DEL /F %BUILD_ZIP%
)
IF EXIST %RESOURCES_ZIP% (
	echo "Remove %RESOURCES_ZIP% existing zip file"
	DEL /F %RESOURCES_ZIP%
)
IF EXIST %THIRDPARTY_ZIP% (
	echo "Remove %THIRDPARTY_ZIP% existing zip file"
	DEL /F %THIRDPARTY_ZIP%
)

Rem Remove extracted directories if already existing
IF EXIST %BUILD_DIR% (
	echo "Remove %BUILD_DIR% existing directory"
	RMDIR /S /Q %BUILD_DIR%
)
IF EXIST %RESOURCES_DIR% (
	echo "Remove %RESOURCES_DIR% existing directory"
	RMDIR /S /Q %RESOURCES_DIR%
)
IF EXIST %THIRDPARTY_DIR% (
	echo "Remove %THIRDPARTY_DIR% existing directory"
	RMDIR /S /Q %THIRDPARTY_DIR%
)

echo "Download zip files."
bitsadmin.exe /transfer "Download build hierarchy" https://github.com/FlorianLance/exvr/releases/download/Dependencies/_build.zip "%BUILD_ZIP%"
bitsadmin.exe /transfer "Download resources" https://github.com/FlorianLance/exvr/releases/download/Dependencies/_resources.zip "%RESOURCES_ZIP%"
bitsadmin.exe /transfer "Download thirdparty" https://github.com/FlorianLance/exvr/releases/download/Dependencies/_thirdparty.zip "%THIRDPARTY_ZIP%"

IF EXIST %BUILD_ZIP% (
	echo "%BUILD_ZIP% successfully downloaded"
)
IF EXIST %RESOURCES_ZIP% (
	echo "%RESOURCES_ZIP% successfully downloaded"
)
IF EXIST %THIRDPARTY_ZIP% (
	echo "%THIRDPARTY_ZIP% successfully downloaded"
)

echo "Uncompress directories."
%UNZIP_EXE% %BUILD_DIR%
%UNZIP_EXE% %RESOURCES_DIR%
%UNZIP_EXE% %THIRDPARTY_DIR%

IF EXIST %BUILD_DIR% (
	echo "%BUILD_DIR% successfully uncompressed"
)
IF EXIST %RESOURCES_DIR% (
	echo "%RESOURCES_DIR% successfully uncompressed"
)
IF EXIST %THIRDPARTY_DIR% (
	echo "%THIRDPARTY_DIR% successfully uncompressed"
)

echo "Copy directories."
xcopy %BUILD_DIR% %DEST_BUILD_DIR%\ /e /h /y /k 
xcopy %RESOURCES_DIR% %DEST_RESOURCES_DIR%\ /e /h /y /k 
xcopy %THIRDPARTY_DIR% %DEST_THIRDPARTY_DIR%\ /e /h /y /k 

echo "Remove zip files."
IF EXIST %BUILD_ZIP% (
	echo "Remove %BUILD_ZIP%  zip file"
	DEL /F %BUILD_ZIP%
)
IF EXIST %RESOURCES_ZIP% (
	echo "Remove %RESOURCES_ZIP%  zip file"
	DEL /F %RESOURCES_ZIP%
)
IF EXIST %THIRDPARTY_ZIP% (
	echo "Remove %THIRDPARTY_ZIP%  zip file"
	DEL /F %THIRDPARTY_ZIP%
)

echo "Remove extracted directories."
IF EXIST %BUILD_DIR% (
	echo "Remove %BUILD_DIR% directory"
	RMDIR /S /Q %BUILD_DIR%
)
IF EXIST %RESOURCES_DIR% (
	echo "Remove %RESOURCES_DIR% directory"
	RMDIR /S /Q %RESOURCES_DIR%
)
IF EXIST %THIRDPARTY_DIR% (
	echo "Remove %THIRDPARTY_DIR% directory"
	RMDIR /S /Q %THIRDPARTY_DIR%
)


echo "ExVR dependencies installation finished."

Rem Toolbox dependencies installation
cd %TOOLBOX_DIR%
%TOOLBOX_SCRIPT%
