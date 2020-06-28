@echo off

::------------------------------
::
:: Relase / Debug
::
::------------------------------
SET /A RELEASE_BUILD=0


::------------------------------
::
:: Environment Settings
:: Vulkan SDK Version, app name, architecture, shader directories
::
::------------------------------
@SET SCRIPT_DIR=%cd%
@SET APP_NAME="Understone"
@SET APP_ARCH=x64
@SET SHADER_DIR=%SCRIPT_DIR%\shaders
@SET SHADER_SRC_DIR=%SHADER_DIR%\source
@SET SHADER_BIN_DIR=%SHADER_DIR%\bin
rem @SET "VULKAN_SDK_PATH=C:\VulkanSDK\1.1.126.0\"
@SET VULKAN_SDK_VERSION=1.2.141.2
@SET "VULKAN_SDK_PATH=C:\VulkanSDK\%VULKAN_SDK_VERSION%\"


::------------------------------
::
:: Compile Shaders
::
::------------------------------
mkdir %SHADER_BIN_DIR% 2>nul
echo Compiling shaders...
%VULKAN_SDK_PATH%\Bin\glslc.exe %SHADER_SRC_DIR%\vkTriangle.vert -o %SHADER_BIN_DIR%\vkTriangle_vert.spv -Werror
%VULKAN_SDK_PATH%\Bin\glslc.exe %SHADER_SRC_DIR%\vkTriangle.frag -o %SHADER_BIN_DIR%\vkTriangle_frag.spv -Werror
IF %ERRORLEVEL% NEQ 0 GOTO :SHADER_COMP_ERR
echo Done.
echo.


::------------------------------
::
:: Compile Engine
:: Requires Visual Studio 2019
::
::------------------------------
where cl >nul 2>null
IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" %APP_ARCH% >nul
IF %ERRORLEVEL% NEQ 0 GOTO :VS_NOT_FOUND

:: Store msvc clutter elsewhere
mkdir msvc_landfill >nul 2>nul
pushd msvc_landfill >nul


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

:: General Parameters
SET GeneralParameters=/Oi /Qpar /EHsc /GL /nologo /Ot

:: Debug Paramters
SET DebugParameters=/Od /MTd /W4 /WX /D__UE_debug__#1

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
@SET "INVOKE_RELEASE=cl %ReleaseParameters% %SCRIPT_DIR%\\%APP_NAME%.c %GeneralParameters% %IncludeParameters% /link %LinkParameters%"
@SET "INVOKE_DEBUG=cl %DebugParameters% %SCRIPT_DIR%\\%APP_NAME%.c %GeneralParameters% %IncludeParameters% /link %LinkParameters%"

IF /I "%RELEASE_BUILD%" EQU "1" (echo UE Release build...) else (echo UE Debug build...)
IF /I "%RELEASE_BUILD%" EQU "1" (%INVOKE_RELEASE%) else (%INVOKE_DEBUG%)
IF %ERRORLEVEL% NEQ 0 GOTO :exit
xcopy /y Understone.exe ..\ >null
popd >null
echo Done.
echo.

::------------------------------
::
:: Tag Analysis
::
::------------------------------
echo Performing tag analysis...
python TagAnalysis.py --emacs
echo Done.
echo.
GOTO :exit

:VS_NOT_FOUND
echo.
echo Unable to find vcvarsall.bat. Did you install Visual Studio to the default location?
echo This build script requries Visual Studio 2019; with the standard C/C++ toolset.
echo.
GOTO :exit


:SHADER_COMP_ERR
echo.
echo Unable to compile shaders. Did you install the Vulkan SDK to the default location?
echo This build script requires SDK version:  %VULKAN_SDK_VERSION%
echo.
GOTO :exit

:exit
