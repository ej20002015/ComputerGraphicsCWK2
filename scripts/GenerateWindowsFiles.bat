@echo off
pushd %~dp0\..\
call premake\bin\windows\premake5.exe vs2019
popd
PAUSE