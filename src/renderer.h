/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */


#define RENDERER_END_FRAME(NAME) void NAME()
typedef RENDERER_END_FRAME(Renderer_End_Frame);


struct Image {
    u32 id;
    u8 *data;
    u32 width;
    u32 height;
};

function umm
hash(Image image) {
    return image.id;
}

function b32
operator == (Image a, Image b) {
    return a.id == b.id;
}

struct Vertex {
    v2 position;
    v4 color;
    v2 uv;
};

struct Sort_Key {
    u32 image_id;
};

struct Renderer {
    void *platform;
    void *backend;

    Hash_Table<Image, int> image_hash_table;
    Queue<Image> image_create_queue;

    Dynamic_Array<Sort_Key>     sort_keys;
    Dynamic_Array<Vertex>       vertices;
    Dynamic_Array<u32>          drawcall_vertex_count;
};

global Renderer *g_renderer;

function void
push_sort_key_and_triangle(Sort_Key sort_key, Vertex a, Vertex b, Vertex c) {
    g_renderer->sort_keys.push(sort_key);
    g_renderer->vertices.push(a);
    g_renderer->vertices.push(b);
    g_renderer->vertices.push(c);
}

function void
draw_triangle(v2 a, v2 b, v2 c, v4 color) {
    ASSERT(g_renderer);
    Vertex v[3];
    v[0] = Vertex{a, v4{1,1,1,1}, v2{1,1}};
    v[1] = Vertex{b, v4{1,1,1,1}, v2{1,1}};
    v[2] = Vertex{c, v4{1,1,1,1}, v2{1,1}};
    push_sort_key_and_triangle({0}, v[0], v[1], v[2]);
}

function void
draw_textured_quad(v2 center, v2 dim, Image image) {
    if (!g_renderer->image_hash_table.get(image).found) {
        g_renderer->image_hash_table.insert(image, 0);
        enqueue(&g_renderer->image_create_queue, image);
    }

    f32 w = 0.5f*dim.x;
    f32 h = 0.5f*dim.y;

    Vertex v[4];
    v[0] = {center + v2{-w, h}, v4{1,1,1,1}, v2{0,1}};
    v[1] = {center + v2{ w, h}, v4{1,1,1,1}, v2{1,1}};
    v[2] = {center + v2{-w,-h}, v4{1,1,1,1}, v2{0,0}};
    v[3] = {center + v2{ w,-h}, v4{1,1,1,1}, v2{1,0}};

    push_sort_key_and_triangle({image.id}, v[0], v[1], v[2]);
    push_sort_key_and_triangle({image.id}, v[2], v[1], v[3]);
}

function b32
compare_sort_key(Sort_Key a, Sort_Key b) {
    if (a.image_id > b.image_id) return true;
    return false;
}

function b32
sort_key_is_same(Sort_Key a, Sort_Key b) {
    if (a.image_id == b.image_id) return true;
    return false;
}

function void
renderer_bubblesort() {
    umm length = g_renderer->sort_keys.count;
    b32 terminate = false;
    while (!terminate) {
        terminate = true;
        for (u32 i = 0; i < length - 1; ++i) {
            u32 j = i + 1;
            if (compare_sort_key(g_renderer->sort_keys.data[i], g_renderer->sort_keys.data[j])) {
                Sort_Key ktmp = g_renderer->sort_keys.data[i];
                g_renderer->sort_keys.data[i] = g_renderer->sort_keys.data[j];
                g_renderer->sort_keys.data[j] = ktmp;

                for (u32 k = 0; k < 3; ++k) {
                    Vertex vtmp = g_renderer->vertices.data[3*i + k];
                    g_renderer->vertices.data[3*i + k] = g_renderer->vertices.data[3*j + k];
                    g_renderer->vertices.data[3*j + k] = vtmp;
                }
                terminate = false;
            }
        }
    }
}

function void
renderer_fill_drawcall_vertex_count() {
    u32 current_end = 0;
    Sort_Key current_sort_key = g_renderer->sort_keys.data[0];

    for (u32 i = 0; i < g_renderer->sort_keys.count; ++i) {
        if (!sort_key_is_same(current_sort_key, g_renderer->sort_keys.data[i])) {
            current_sort_key = g_renderer->sort_keys.data[i];
            u32 numtri = i - current_end;
            g_renderer->drawcall_vertex_count.push(3*numtri);
            current_end = i;
        }
    }

    u32 numtri = (u32)g_renderer->sort_keys.count - current_end;
    g_renderer->drawcall_vertex_count.push(3*numtri);
}
