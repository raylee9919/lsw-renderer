/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */




#include <sys/mman.h>

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

#if 1
#  define RENDERER_DLL "vulkan.dll"
#else
#  define RENDERER_DLL "opengl.dll"
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

    os.alloc = linux_alloc;
    os.free  = linux_free;

    Image images[3];
    images[0].id = 2;
    images[0].data = stbi_load("texture.jpg", (int *)&images[0].width, (int *)&images[0].height, 0, 4);
    images[1].id = 3;
    images[1].data = stbi_load("doggo.png", (int *)&images[1].width, (int *)&images[1].height, 0, 4);
    images[2].id = 4;
    images[2].data = stbi_load("doggo2.png", (int *)&images[2].width, (int *)&images[2].height, 0, 4);

    while (g_running) {
        while (XPending(display)) {
            XEvent event{};
            XNextEvent(display, &event);
        }
    }
}
