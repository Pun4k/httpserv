#include "hashtab.h"

static u32 strhash_(u8* str, size_t size) {
    u32 hash_value = 0;
    for(hash_value; *str != '\0'; ++str) {
        hash_value += *str + 31 * hash_value;
    }
    return hash_value % size;
}

extern HashTab *new_hashtab(size_t size, vtype_tree_t tkey, vtype_tree_t tvalue) {
    switch (tkey) {
    case DECIMAL_ELEM: case STRING_ELEM:
        break;
    default:
        fprintf(stderr, "%s\n", "Key type doesnt supported");
        break;
    }

    switch (tvalue)
    {
    case DECIMAL_ELEM: case REAL_ELEM: case STRING_ELEM:
        break;
    default:
        fprintf(stderr, "%s\n", "Value type doesnt supported");
        break;
    }

    HashTab *hashtab = (HashTab*)malloc(sizeof(HashTab));
    hashtab->table = (Tree**)malloc(size * sizeof(Tree));
    for(size_t i = 0; i < size; ++i) {
        hashtab->table[i] = new_tree(tkey, tvalue);
    }
    hashtab->size = size;
    hashtab->type.key = tkey;
    hashtab->type.value = tvalue;
    return hashtab;
}

extern void free_hashtab(HashTab *hashtab) {
    for(size_t i = 0; i < hashtab->size; ++i) {
        free_tree(hashtab->table[i]);
    }
    free(hashtab->table);
    free(hashtab);
}

extern void print_hashtab(HashTab *hashtab) {
    u32 hash;
    printf("{");
    for(size_t i = 0; i < hashtab->size; ++i) {
        if(hashtab->table[i]->node == NULL) {
            continue;
        }
        switch (hashtab->type.key) {
        case DECIMAL_ELEM:
            hash = hashtab->table[i]->node->data.key.decimal % hashtab->size;
            break;
        case STRING_ELEM:
            hash = strhash_(hashtab->table[i]->node->data.key.string, hashtab->size);
            break;
        default:
            break;
        }
        printf("\t%d => ", hash);
        print_tree_as_list(hashtab->table[i]);
    }
    printf("\t}\n");
}

extern void set_hashtab(HashTab *hashtab, void *key, void *value) {
    u32 hash;
    switch (hashtab->type.key) {
    case DECIMAL_ELEM:
        hash = (u64)key % hashtab->size;
        
        break;
    case STRING_ELEM:
        hash = strhash_((u8*)key, hashtab->size);
    default:
        break;
    }
    set_tree(hashtab->table[hash], key, value);
}

extern value_tree_t get_hashtab(HashTab *hashtab, void *key) {
    u32 hash;
    value_tree_t result;
    switch (hashtab->type.key) {
    case DECIMAL_ELEM:
        hash = (u64)key % hashtab->size;

        break;
    case STRING_ELEM:
        hash = strhash_((u8*)key, hashtab->size);
    default:
        break;
    }
    result = get_tree(hashtab->table[hash], key);
    return result;
}

extern _Bool in_hashtab(HashTab *hashtab, void *key) {
    u32 hash;
    _Bool result;
    switch (hashtab->type.key) {
    case DECIMAL_ELEM:
        hash = (u64)key % hashtab->size;

        break;
    case STRING_ELEM:
        hash = strhash_((u8*)key, hashtab->size);
    default:
        break;
    }
    result = in_tree(hashtab->table[hash], key);
    return result;
}

extern void del_hashtab(HashTab *hashtab, void *key) {
    u32 hash;
    switch (hashtab->type.key) {
    case DECIMAL_ELEM:
        hash = (u64)key % hashtab->size;

        break;
    case STRING_ELEM:
        hash = strhash_((u8*)key, hashtab->size);
    default:
        break;
    }
    del_tree(hashtab->table[hash], key);
}