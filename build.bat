@echo off

@where cl >nul 2>nul
:: If cl was not found in path, initialize for x64
IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall" x64 >nul

:: If cl was not found in path, initialize for x86
REM IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall" x86 >nul


::
:: [ COMPILER OPTIONS ]
::
:: /Z7	   Full symbolic debug info. No pdb. (See /Zi, /Zl).
:: /GS	   Detect buffer overruns.
:: /MD	   Multi-thread specific, DLL-specific runtime lib. (See /MDd, /MT, /MTd, /LD, /LDd).
:: /GL	   Whole program optimization.
:: /EHsc   No exception handling (Unwind semantics requrie vstudio env). (See /W1).
:: /I<arg> Specify include directory.

::
:: [ LINKER OPTIONS ]
::
:: /link                Invoke microsoft linker options.
:: /NXCOMPAT            Comply with Windows Data Execution Prevention.
:: /MACHINE:<arg>       Declare machine arch (should match vcvarsall env setting).
:: /NODEFAULTLIB:<arg>  Ignore a library.
:: /LIBPATH:<arg>       Specify library directory/directories

:: "%~1" prefix the first command line arg with the string "..\..\"
:: and remove quotations before seinding it as an argument to cl.

echo.
echo [ STARTING COMPILATION ]

cl engine.c  /W4 /WX ^
engine_tools/ogl_tools.c win/win_platform.c ^
renderers/triangle_renderer.c ^
renderers/test_bitmap_renderer.c ^
data_structures/uDynamicArray.c ^
tests/tests.c ^
/I. /Iengine_tools /Iwin /Irenderers /Idata_structures /Itests ^
-Z7 /GS /MD /EHsc /nologo ^
/link /SUBSYSTEM:CONSOLE /NXCOMPAT /MACHINE:x64 /NODEFAULTLIB:MSVCRTD ^
opengl32.lib ^
user32.lib ^
gdi32.lib ^
shell32.lib ^
odbccp32.lib

echo [ COMPILATION COMPLETE ]
echo.
