/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */




template<typename T>
struct Queue {
    T data[256];
    size_t front;
    size_t count;
};

template<typename T>
T dequeue(Queue<T> *q) {
    ASSERT(q->count > 0);

    T result = q->data[q->front++];
    q->front = (q->front) % arraycount(q->data);
    --q->count;
    return result;
}

template<typename T>
void enqueue(Queue<T> *q, T item) {
    ASSERT(q->count < arraycount(q->data));

    size_t idx = ((q->front + q->count) % arraycount(q->data));
    q->data[idx] = item;
    ++q->count;
}

template<typename T>
bool empty(Queue<T> *q) {
    if (q->count == 0) return true;
    return false;
}

template<typename T>
void clear(Queue<T> *q) {
    q->count = 0;
}

template<typename T>
T peek(Queue<T> *q) {
    ASSERT(q->count > 0);
    return q->data[q->front];
}
