#ifndef __debug_tools__
#define __debug_tools__ 1

#ifndef functionFired
#define functionFired  printf("[ debug ] function fired: %s\n", __func__);
#endif

#endif // __debug_tools
