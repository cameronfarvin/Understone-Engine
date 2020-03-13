@echo off
@SET SCRIPT_DIR=%cd%
@SET APP_NAME="engine"
@SET APP_ARCH=x64
@SET "VULKAN_SDK_PATH=C:\VulkanSDK\1.1.126.0\"


::
:: Initialize cl.exe for correct environment.
::
@where cl >nul 2>nul
:: VS 2017 Community Edition
rem IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall" %APP_ARCH% >nul

:: VS 2017 Professional Edition
IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall" %APP_ARCH% >nul

:: VS 2017 Enterprise Edition
rem IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall" %APP_ARCH% >nul

:: VS 2019 Community Edition
rem IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" %APP_ARCH% >nul

:: VS 2019 Enterprise Edition
rem IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" %APP_ARCH% >nul


::
:: Compile & Link
::
:: /TC                  Compile as C code.
:: /TP                  Compile as C++ code.
:: /Oi                  Enable intrinsic functions.
:: /Od 	                Disables optimization.
:: /Qpar                Enable parallel code generation.
:: /Ot                  Favor fast code (over small code).
:: /Ob2                 Enable full inline expansion. [ cfarvin::NOTE ] Debugging impact.
:: /Z7	                Full symbolic debug info. No pdb. (See /Zi, /Zl).
:: /GS	                Detect buffer overruns.
:: /MD	                Multi-thread specific, DLL-specific runtime lib. (See /MDd, /MT, /MTd, /LD, /LDd).
:: /GL	                Whole program optimization.
:: /EHsc                No exception handling (Unwind semantics requrie vstudio env). (See /W1).
:: /I<arg>              Specify include directory.
:: /link                Invoke microsoft linker options.
:: /NXCOMPAT            Comply with Windows Data Execution Prevention.
:: /MACHINE:<arg>       Declare machine arch (should match vcvarsall env setting).
:: /NODEFAULTLIB:<arg>  Ignore a library.
:: /LIBPATH:<arg>       Specify library directory/directories.
IF %ERRORLEVEL% NEQ 0 GOTO :exit
mkdir msvc_landfill >nul 2>nul
pushd msvc_landfill >nul

cl %SCRIPT_DIR%\\%APP_NAME%.c /Oi /Qpar /Ot /W4 /WX /Ob2 /O2 /GL /MD /EHsc /nologo ^
/I%cd%\.. ^
/I%cd%\..\engine_tools ^
/I%cd%\..\win ^
/I%cd%\..\data_structures ^
/I%cd%\..\tests ^
/I%cd%\..\practice ^
/I%VULKAN_SDK_PATH%\Include ^
/Z7 /GL /GS /MD /EHsc /nologo ^
/link /SUBSYSTEM:CONSOLE /NXCOMPAT /MACHINE:x64 /NODEFAULTLIB:MSVCRTD ^
/LIBPATH:%VULKAN_SDK_PATH%\Lib\ ^
OpenGL32.lib ^
user32.lib ^
gdi32.lib ^
shell32.lib ^
odbccp32.lib ^
vulkan-1.lib

IF %ERRORLEVEL% NEQ 0 GOTO :exit
xcopy /y engine.exe ..\ >null
popd >null

IF %ERRORLEVEL% NEQ 0 GOTO :exit
rem engine.exe

:exit
