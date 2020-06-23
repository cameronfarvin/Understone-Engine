@echo off
@SET SCRIPT_DIR=%cd%
@SET APP_NAME="Understone"
@SET APP_ARCH=x64
@SET SHADER_DIR=%SCRIPT_DIR%\shaders
@SET SHADER_SRC_DIR=%SHADER_DIR%\source
@SET SHADER_BIN_DIR=%SHADER_DIR%\bin
rem @SET "VULKAN_SDK_PATH=C:\VulkanSDK\1.1.126.0\"
@SET "VULKAN_SDK_PATH=C:\VulkanSDK\1.2.141.2\"

::------------------------------
::
:: Compile Shaders
::
::------------------------------
echo Compiling shaders....
%VULKAN_SDK_PATH%\Bin\glslc.exe %SHADER_SRC_DIR%\vkTriangle.vert -o %SHADER_BIN_DIR%\vkTriangle_vert.spv -Werror
%VULKAN_SDK_PATH%\Bin\glslc.exe %SHADER_SRC_DIR%\vkTriangle.frag -o %SHADER_BIN_DIR%\vkTriangle_frag.spv -Werror
IF %ERRORLEVEL% NEQ 0 GOTO :exit
echo.


::------------------------------
::
:: Compile Engine
::
::------------------------------
:: Initialize cl.exe for correct environment.
@where cl >nul 2>nul
:: VS 2017 Professional Edition
rem IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall" %APP_ARCH% >nul

:: VS 2019 Community Edition
IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" %APP_ARCH% >nul



:: Compile & Link Options
::------------------------------
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

:: Store msvc clutter elsewhere
IF %ERRORLEVEL% NEQ 0 GOTO :exit
mkdir msvc_landfill >nul 2>nul
pushd msvc_landfill >nul


:: Engine compilation settings
::------------------------------

:: General Parameters
SET GeneralParameters=/Oi /Qpar /EHsc /GL /nologo /Ot

:: Debug Paramters
SET DebugParameters=/Od /MTd /W4 /WX

:: Release Parameters
SET ReleaseParameters=/MT /O2 /W4 /WX /Ob2

:: Include Parameters
SET IncludeParameters=/I%cd%\.. ^
/I%cd%\..\engine_tools ^
/I%cd%\..\win ^
/I%cd%\..\data_structures ^
/I%cd%\..\tests ^
/I%cd%\..\practice ^
/I%VULKAN_SDK_PATH%\Include

:: Link Parameters
SET LinkParameters=/SUBSYSTEM:CONSOLE ^
/NXCOMPAT ^
/MACHINE:x64 ^
/NODEFAULTLIB:MSVCRTD ^
/LIBPATH:%VULKAN_SDK_PATH%\Lib\ ^
OpenGL32.lib ^
user32.lib ^
gdi32.lib ^
shell32.lib ^
odbccp32.lib ^
vulkan-1.lib


:: Compiler Invocation
::------------------------------
cl %SCRIPT_DIR%\\%APP_NAME%.c ^
%GeneralParameters% %DebugParameters% %IncludeParameters% /link %LinkParameters%

IF %ERRORLEVEL% NEQ 0 GOTO :exit
xcopy /y Understone.exe ..\ >null
popd >null

IF %ERRORLEVEL% NEQ 0 GOTO :exit
rem engine.exe :: auto-run after build

::------------------------------
::
:: Tag Analysis
::
::------------------------------
python TagAnalysis.py --emacs

:exit
