@echo off

SetLocal EnableDelayedExpansion

set cppFilenames=
for /R ./src %%f in (*.cpp) do (
	set cppFilenames=!cppFilenames! %%f
)

echo "Files: " %cppFilenames%

set compilerFlags=-std=c++17 -Wall -Werror -Wno-deprecated-declarations
set defines=-DCRT_SECURE_NO_WARNINGS
set includeFlags=-Isrc -ISFML/include
set linkerFlags=-LSFML/lib -lkernel32 -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio

clang++ %cppFilenames% %compilerFlags% -o ./bin/game.exe %defines% %includeFlags% %linkerFlags%