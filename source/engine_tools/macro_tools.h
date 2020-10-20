#ifndef __UE_MACRO_TOOLS_H___
#define __UE_MACRO_TOOLS_H___

#if __linux__
#    define P_OPEN(command, mode) popen(command, mode)
#    define P_CLOSE(pipe)         pclose(pipe)
#else
#    define P_OPEN(command, mode) _popen(command, mode)
#    define P_CLOSE(pipe)         _pclose(pipe)
#endif // __linux__

//
// [ begin ] Decorators
#ifndef __UE_call__
#    define __UE_call__ __cdecl
#endif // __UE_call__

#ifndef _mut_
#    define _mut_ /* NOTICE: MUTABLE */
#endif            // _mut_

// Scopes functions to the current translation unit
#ifndef __UE_internal__
#    define __UE_internal__ static
#endif // __UE_internal__

// Scopes functions globally
#ifndef __UE_external__
#    define __UE_external__ extern
#endif // __UE_external__

// Scopes variables to the current translation unit scope
#ifndef __UE_global__
#    define __UE_global__ /* NOTICE: GLOBAL */
#endif                    // __UE_global__

// Scopes variables to the local file
#ifndef __UE_local__
#    define __UE_local__ static
#endif // __UE_local__

// scopes Singletons to global scope
#ifndef __UE_singleton__
#    define __UE_singleton__ const
#endif // __UE_singleton__
// [ end ] Decorators
//

//
// [ begin ]Passifiers
#ifdef _WIN32
#    define restrict __restrict
#endif // WIN32
// [ end ]Passifiers
//

#endif // __UE_MACRO_TOOLS_H___
