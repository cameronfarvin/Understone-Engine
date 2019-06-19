@echo off



@where cl >nul 2>nul
:: If cl was not found in path, initialize for x64 (Community Edition 2017)
rem IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall" x64 >nul

:: If cl was not found in path, initialize for x86 (Enterprise Edition 2017)
rem IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall" x64 >nul

:: If cl was not found in path, initialize for x86 (Community Edition 2019)
IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64 > nul


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

mkdir msvc_landfill >nul 2>nul
pushd msvc_landfill >nul

cl %cd%\..\engine.c  /W4 /WX ^
%cd%\..\win\win_platform.c ^
%cd%\..\engine_tools\ogl_tools.c ^
%cd%\..\engine_tools\type_tools.c ^
%cd%\..\engine_tools\image_tools.c ^
%cd%\..\tests\tests.c ^
/I%cd%\.. ^
/I%cd%\..\engine_tools ^
/I%cd%\..\win ^
/I%cd%\..\renderers ^
/I%cd%\..\data_structures ^
/I%cd%\..\tests ^
/I%cd%\..\practice ^
/Z7 /GL /GS /MD /EHsc /nologo ^
/link /SUBSYSTEM:CONSOLE /NXCOMPAT /MACHINE:x64 /NODEFAULTLIB:MSVCRTD ^
OpenGL32.lib ^
user32.lib ^
gdi32.lib ^
shell32.lib ^
odbccp32.lib

xcopy /y engine.exe ..\ >null
popd >null

echo [ COMPILATION COMPLETE ]
echo.

rem %cd%\..\renderers\triangle_renderer.c ^
rem %cd%\..\renderers\texture_test_renderer.c ^
rem %cd%\..\renderers\test_bitmap_renderer.c ^
rem %cd%\..\practice\practice.c ^
