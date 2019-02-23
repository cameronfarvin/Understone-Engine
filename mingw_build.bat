@echo off
IF EXIST C:\MinGW64\bin\ (goto compile) else (goto dieTerribleDeath)

REM
REM Begin Compilation
REM
:compile

echo.
echo.
echo [ STARTING COMPILATION ]

set PATH=%PATH%C:\MinGw64;C:\MinGW64\bin;C:\MinGW64\lib

C:\MinGW64\bin\gcc.exe -D__USE_MINGW_ANSI_STDIO=1 ^
-g -O0 -Wall -Werror -Wextra -Wshadow -Wcast-align -std=c11 ^
-fno-diagnostics-color -Wno-unknown-pragmas ^
engine.c ^
engine_tools\ogl_tools.c ^
win\win_platform.c ^
renderers\triangle_renderer.c ^
data_structures\uDynamicArray.c ^
tests\tests.c ^
-Inix -Iwin -Iengine_tools -Irenderers -I. -Idata_structures -Itests ^
-lopengl32 -luser32 -lgdi32 -lshell32 -lodbccp32 ^
-o engine

echo [ COMPILATION COMPLETE ]
goto dieHonorableDeath

REM
REM Unalbe to find MinGW
REM
:dieTerribleDeath
echo.
echo.
echo    [ ERROR ] MinGW not found at location C:\MinGW\bin

:dieHonorableDeath
echo.
