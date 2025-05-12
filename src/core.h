/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */
// @TODO: Remove this.
#include <stdio.h>

#include "types.h"

#define ASSERT(EXP) if (!(EXP)) {*(volatile int *)0 = 0;}
#define INVALID_CODE_PATH ASSERT(!"InvalidCodePath")
#define INVALID_DEFAULT_CASE default: {ASSERT(!"InvalidDefaultCase");} break
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define arraycount(array) ( sizeof(array) / sizeof(array[0]) )
#define zerostruct(PTR, STRUCT) zerosize((PTR), sizeof(STRUCT))
#define zeroarray(ARR, COUNT) zerosize((ARR), (COUNT) * sizeof(ARR[0]))
function void zerosize(void *ptr, umm size) {
    u8 *byte = (u8 *)ptr;
    while (size--) *byte++ = 0;
}
#define copy_array(src, dst, count) copy((src), (dst), sizeof(*src)*(count))
function void copy(void *src_, void *dst_, umm size) {
    u8 *src = (u8 *)src_;
    u8 *dst = (u8 *)dst_;
    while (size--) {*dst++ = *src++;}
}
#define offset_of(STRUCT, MEMBER) ((size_t)(&(((STRUCT *)0)->MEMBER)))

template <typename F>
struct ScopeExit {
    ScopeExit(F f) : f(f) {}
    ~ScopeExit() { f(); }
    F f;
};
template <typename F>
ScopeExit<F> MakeScopeExit(F f) {
    return ScopeExit<F>(f);
};

#define STRING_JOIN2(arg1, arg2) DO_STRING_JOIN2(arg1, arg2)
#define DO_STRING_JOIN2(arg1, arg2) arg1 ## arg2
#define SCOPE_EXIT(code) \
    auto STRING_JOIN2(scope_exit_, __LINE__) = MakeScopeExit([=](){code;})


function umm
cstring_length(const char *string) {
    umm count = 0;
    while (*string++) {
        count++;
    }
    return count;
}

function b32
cstring_equal(const char *str1, umm len1, const char *str2, umm len2) {
    b32 result = (len1 == len2);

    if (result) {
        result = true;
        for (u32 idx = 0; idx < len1; ++idx) {
            if (str1[idx] != str2[idx]) {
                result = false;
                break;
            }
        }
    }

    return result;
}

function b32
cstring_equal(const char *str1, const char *str2) {
    return cstring_equal(str1, cstring_length(str1), str2, cstring_length(str2));
}


struct Buffer {
    u8 *data;
    mmm size;
};

//@TODO: Remove this.
function Buffer
read_entire_file(const char *filepath) {
    Buffer result = {};
    FILE *file = fopen(filepath, "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        size_t filesize = ftell(file);
        fseek(file, 0, SEEK_SET);
        result.data = (u8 *)malloc(filesize);
        result.size = filesize;
        fread(result.data, filesize, 1, file);
        fclose(file);
    } else {
        fprintf(stderr, "[ERROR] Couldn't open file %s.\n", filepath);
    }
    return result;
}
