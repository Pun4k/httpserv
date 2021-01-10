#ifndef HASHTAB_H
#define HASHTAB_H

#include "../binary_tree/binarytree.h"

typedef struct HashTab {
    struct 
    {
        vtype_tree_t key;
        vtype_tree_t value;
    } type;
    size_t size;
    Tree **table;
} HashTab;

extern HashTab *new_hashtab(size_t size, vtype_tree_t tkey, vtype_tree_t tvalue);
extern void free_hashtab(HashTab *hashtab);

extern void print_hashtab(HashTab *hashtab);

extern value_tree_t get_hashtab(HashTab *hashtab, void *key);
extern void set_hashtab(HashTab *hashtab, void *key, void *value);
extern void del_hashtab(HashTab *hashtab, void *key);
extern _Bool in_hashtab(HashTab *hashtab, void *key);

#endif //!HASHTAB_H