#ifndef __UE_MACRO_TOOLS_H___
#define __UE_MACRO_TOOLS_H___

#if __linux__
#define P_OPEN(command, mode) popen(command, mode)
#define P_CLOSE(pipe)         pclose(pipe)
#else
#define P_OPEN(command, mode) _popen(command, mode)
#define P_CLOSE(pipe)         _pclose(pipe)
#endif // __linux__

//
// [ begin ]Passifiers
#ifdef _WIN32
#define restrict __restrict
#endif // WIN32
// [ end ]Passifiers
//

#endif // __UE_MACRO_TOOLS_H___
