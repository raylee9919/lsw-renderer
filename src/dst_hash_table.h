/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */






function umm
hash(u32 key) {
    return key;
}

template<typename K, typename V>
struct Hash_Table_Node {
    K key;
    V value;
    Hash_Table_Node *next;
};

template<typename V>
struct Hash_Get_Result {
    b32 found;
    V value;
};

template<typename K, typename V>
struct Hash_Table {
    Hash_Table_Node<K, V> *sentinels;
    umm sentinel_count;

    void init(umm sentinel_count_) {
        sentinel_count = sentinel_count_;
        sentinels = (Hash_Table_Node<K, V> *)os.alloc(sizeof(Hash_Table_Node<K, V>) * sentinel_count);
        for (umm i = 0; i < sentinel_count; ++i) {
            sentinels[i].next = sentinels + i;
        }
    }

    Hash_Table_Node<K, V> *get_sentinel(K key) {
        if (sentinel_count == 0) {
            init(32);
        }
        umm bucket = hash(key) % sentinel_count;
        return sentinels + bucket;
    }

    void insert(K key, V value) {
        Hash_Table_Node<K, V> *sentinel = get_sentinel(key);
        Hash_Table_Node<K, V> *node = sentinel->next;

        while (1) {
            if (node->next == sentinel) {
                Hash_Table_Node<K, V> *new_node = (Hash_Table_Node<K, V> *)os.alloc(sizeof(Hash_Table_Node<K, V>));
                new_node->key = key;
                new_node->value = value;
                new_node->next = sentinel;

                node->next = new_node;
                break;
            }

            node = node->next;
        }
    }

    Hash_Get_Result<V> get(K key) {
        Hash_Get_Result<V> result{};

        Hash_Table_Node<K, V> *sentinel = get_sentinel(key);
        Hash_Table_Node<K, V> *node = sentinel->next;

        while (1) {
            if (node == sentinel) {
                break;
            }

            if (node->key == key) {
                result.found = true;
                result.value = node->value;
                break;
            }

            node = node->next;
        }

        return result;
    }
};
