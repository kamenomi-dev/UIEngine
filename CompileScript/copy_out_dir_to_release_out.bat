REM binary out.
echo f | xcopy ..\x64\Release\UIEngine.dll ..\ReleaseOut\bin\UIEngine.dll /Y /F
echo f | xcopy ..\x64\Release\UIEngine.pdb ..\ReleaseOut\bin\UIEngine.pdb /Y /F

REM lib
echo f | xcopy ..\x64\Release\UIEngine.lib ..\ReleaseOut\lib\UIEngine.lib /Y /F

REM header
echo d | xcopy ..\UIEngine\*.h  ..\ReleaseOut\includes\ /Y /F
echo d | xcopy ..\UIEngine\components\*.h  ..\ReleaseOut\includes\components\ /Y /F
