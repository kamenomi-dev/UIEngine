REM binary out.
xcopy ..\x64\Release\UIEngine.dll ..\ReleaseOut\bin\UIEngine.dll /S /I /Y
xcopy ..\x64\Release\UIEngine.pdb ..\ReleaseOut\bin\UIEngine.pdb /S /I /Y

REM lib
xcopy ..\x64\Release\UIEngine.lib ..\ReleaseOut\lib\UIEngine.lib /S /I /Y

REM header
copy ..\UIEngine\*.h  ..\ReleaseOut\includes\ /Y
copy ..\UIEngine\components\*.h  ..\ReleaseOut\includes\components\ /Y
