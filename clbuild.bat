@echo off

::------------------------------
::
:: Relase / Debug
::
::------------------------------
SET /A RELEASE_BUILD=1


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
:: Requires Clang
::
::------------------------------

:: Compile & Link Options
::------------------------------
:: /TC                  Compile as C code.
:: /TP                  Compile as C++ code.
:: /Oi                  Enable intrinsic functions.
:: /Od 	                Disables optimization.
:: /Ot                  Favor fast code (over small code).
:: /Ob2                 Enable full inline expansion. [ cfarvin::NOTE ] Debugging impact.
:: /Z7	                Full symbolic debug info. No pdb. (See /Zi, /Zl).
:: /GS	                Detect buffer overruns.
:: /MD	                Multi-thread specific, DLL-specific runtime lib. (See /MDd, /MT, /MTd, /LD, /LDd).
:: /EHsc                No exception handling. (See /W1).
:: /I<arg>              Specify include directory.
:: /link                Invoke microsoft linker options.
:: /NXCOMPAT            Comply with Windows Data Execution Prevention.
:: /MACHINE:<arg>       Declare machine arch (should match vcvarsall env setting).
:: /NODEFAULTLIB:<arg>  Ignore a library.
:: /LIBPATH:<arg>       Specify library directory/directories.

:: General Parameters
SET GeneralParameters=/Oi /EHsc /nologo /Ot /TC -fuse-ld=lld -flto -fsanitize=undefined,cfi

:: Debug Paramters
SET DebugParameters=/Od /MTd /W4 /WX /D__UE_debug__#1

:: Release Parameters
SET ReleaseParameters=/MT /O2 /W4 /WX /Ob2

:: Include Parameters
SET IncludeParameters=/Iengine_tools ^
/Iengine_tools/vulkan_tools ^
/Idata_structures ^
/Iwin ^
/Inix ^
/Itests ^
/I%VULKAN_SDK_PATH%\Include

:: Link Parameters
SET LinkParameters=/SUBSYSTEM:CONSOLE ^
/NXCOMPAT ^
/MACHINE:x64 ^
/NODEFAULTLIB:MSVCRTD ^
/LIBPATH:%VULKAN_SDK_PATH%\Lib\ ^
user32.lib ^
vulkan-1.lib


:: Compiler Invocation
::------------------------------
@SET "INVOKE_RELEASE=clang-cl %ReleaseParameters% %SCRIPT_DIR%\\%APP_NAME%.c %GeneralParameters% %IncludeParameters% /link %LinkParameters%"
@SET "INVOKE_DEBUG=clang-cl %DebugParameters% %SCRIPT_DIR%\\%APP_NAME%.c %GeneralParameters% %IncludeParameters% /link %LinkParameters%"

IF /I "%RELEASE_BUILD%" EQU "1" (echo UE Release build...) else (echo UE Debug build...)
IF /I "%RELEASE_BUILD%" EQU "1" (%INVOKE_RELEASE%) else (%INVOKE_DEBUG%)
IF %ERRORLEVEL% NEQ 0 GOTO :exit
echo Done.
echo.
GOTO :exit


:SHADER_COMP_ERR
echo.
echo Unable to compile shaders. Did you install the Vulkan SDK to the default location?
echo This build script requires SDK version:  %VULKAN_SDK_VERSION%
echo.
GOTO :exit

:exit
