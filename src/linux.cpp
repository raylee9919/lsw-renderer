/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */




#include <sys/mman.h>
#include <dlfcn.h>

#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>


#include "core.h"
#include "intrinsics.h"
#include "math.h"

#include "platform.h"

Os os;

#include "dst.h"

#include "renderer.h"
#include "linux_renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

global b32 g_running = true;

// @TODO: Build filepath from executable path.
#if 1
#  define RENDERER_SO "../build/renderer_vulkan.so"
#else
#  define RENDERER_SO "opengl.so"
#endif
    

// @SPEC: ZII
function
OS_ALLOC(linux_alloc) {
    void *result = mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    return result;
}

function
OS_FREE(linux_free) {
    if (memory) {
        // @TODO:
    }
}

function f32
rand01(void) {
    return ((f32)rand() / RAND_MAX);
}


int main(void) {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        INVALID_CODE_PATH;
    }

    Window window = XCreateSimpleWindow(display, XDefaultRootWindow(display), 0, 0, 800, 600, 0, 0, 0);

    XMapWindow(display, window);
    XSync(display, False);


    Linux_Renderer_Function_Table renderer_function_table{};


    const char *renderer_so_filepath = "/home/raylee9919/porn/renderer/build/renderer_vulkan.so";
    void *renderer_so = dlopen(renderer_so_filepath, RTLD_NOW | RTLD_LOCAL);
    if (!renderer_so) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        INVALID_CODE_PATH;
    }
    for (int i = 0; i < arraycount(linux_renderer_function_table_names); ++i) {
        ((void **)&renderer_function_table)[i] = dlsym(renderer_so, linux_renderer_function_table_names[i]);
        if (!((void **)&renderer_function_table)[i]) {
            fprintf(stderr, "dlsym failed: %s\n", dlerror());
            INVALID_CODE_PATH;
        }
    }

    g_renderer = renderer_function_table.load_renderer(display, window);

    os.alloc = linux_alloc;
    os.free  = linux_free;

    Image images[3];
    images[0].id = 2;
    images[0].data = stbi_load("../data/texture.jpg", (int *)&images[0].width, (int *)&images[0].height, 0, 4);
    images[1].id = 3;
    images[1].data = stbi_load("../data/doggo.png", (int *)&images[1].width, (int *)&images[1].height, 0, 4);
    images[2].id = 4;
    images[2].data = stbi_load("../data/doggo2.png", (int *)&images[2].width, (int *)&images[2].height, 0, 4);

    while (g_running) {
        while (XPending(display)) {
            XEvent event{};
            XNextEvent(display, &event);
        }

        XWindowAttributes attr{};
        XGetWindowAttributes(display, window, &attr);
        f32 w = (f32)attr.width;
        f32 h = (f32)attr.height;

        for (u32 i = 0; i < 256; ++i) {
            u32 max_image_index = arraycount(images) - 1;
            u32 image_index = (u32)(rand01()*max_image_index + 0.5f);
            v2 center = {rand01()*w, rand01()*h};
            v2 dim = {50, 50};
            draw_textured_quad(center, dim, images[image_index]);
        }

        renderer_bubblesort();
        renderer_fill_drawcall_vertex_count();

        renderer_function_table.end_frame();
    }
}
