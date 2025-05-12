/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */




struct Renderer_Linux {
    Display *display;
    Window window;
};

#define LINUX_LOAD_RENDERER(NAME) Renderer *NAME(Display *display, Window window)
typedef LINUX_LOAD_RENDERER(Linux_Load_Renderer);

struct Linux_Renderer_Function_Table {
    Linux_Load_Renderer *load_renderer;
    Renderer_End_Frame *end_frame;
};

global const char *linux_renderer_function_table_names[] = {
    "linux_load_renderer",
    "linux_end_frame",
};
