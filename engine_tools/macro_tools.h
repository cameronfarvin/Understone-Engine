#ifndef __UE_MACRO_TOOLS_H___
#define __UE_MACRO_TOOLS_H___



//
// Decorators
//
#ifndef __UE_call__
#define __UE_call__ /* NOTICE: CALL */
#endif // __UE_call__

#ifndef _mut_
#define _mut_ /* NOTICE: MUTABLE */
#endif // _mut_

#ifndef __UE_internal__
#define __UE_internal__ static
#endif // __UE_internal__

#ifndef __UE_external__
#define __UE_external__ extern // functions
#endif // __UE_external__



//
// Passifiers
//
#ifdef _WIN32
#define restrict __restrict
#endif // WIN32

#endif // __UE_MACRO_TOOLS_H___
