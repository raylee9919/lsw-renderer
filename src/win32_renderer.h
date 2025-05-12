/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */

struct Renderer_Win32 {
    HWND hwnd;
};

#define WIN32_LOAD_RENDERER(NAME) Renderer *NAME(HWND hwnd)
typedef WIN32_LOAD_RENDERER(Win32_Load_Renderer);

struct Win32_Renderer_Function_Table {
    Win32_Load_Renderer *load_renderer;
    Renderer_End_Frame *end_frame;
};

global const char *win32_renderer_function_table_names[] = {
    "win32_load_renderer",
    "win32_end_frame",
};
