/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */

template<typename T>
struct Dynamic_Array {
    T *data;
    umm size;
    umm count;

    void init(umm size_) {
        size = size_;
        os.free(data);
        data = (T *)os.alloc(sizeof(T) * size);
        count = 0;
    }

    void push(T item) {
        if (size == 0) {
            size = 32;
            data = (T *)os.alloc(sizeof(T) * size);
            count = 0;
        } else {
            if (count == size) {
                T *old = data;
                size = size + 32;
                data = (T *)os.alloc(sizeof(T) * size);
                copy_array(old, data, count);
                os.free(old);
            }
        }
        data[count++] = item;
    }

    void clear(void) {
        count = 0;
    }

    void free(void) {
        os.free(data);
        size = 0;
        count = 0;
    }
};
