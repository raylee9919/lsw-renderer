/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */



#define OS_ALLOC(NAME) void *NAME(size_t size)
typedef OS_ALLOC(Os_Alloc);

#define OS_FREE(NAME) void NAME(void *memory)
typedef OS_FREE(Os_Free);

struct Os {
    // @SPEC: allocation must be initted to zero.
    Os_Alloc    *alloc;
    Os_Free     *free;
};
