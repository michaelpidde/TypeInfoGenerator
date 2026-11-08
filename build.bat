@echo off

rmdir /q /s build
mkdir build
pushd build

cl -nologo -std:c++20 -W4 -EHsc -Zi ../*.cpp -FeTypeInfoGenerator ^
-link -nologo -opt:ref -subsystem:console -out:TypeInfoGenerator.exe

popd
