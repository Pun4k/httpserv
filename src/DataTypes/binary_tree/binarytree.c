#include "binarytree.h"

static void set_value_(tree_node *node, vtype_tree_t tvalue, void *value);
static void set_key_(tree_node *node, vtype_tree_t tkey, void *key);


static tree_node *new_node_(vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value) {
    tree_node *node = (tree_node*)malloc(sizeof(tree_node));

    set_key_(node, tkey, key);
    set_value_(node, tvalue, value);

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

static void set_key_(tree_node *node, vtype_tree_t tkey, void *key) {
    switch (tkey) {
    case DECIMAL_ELEM:
        node->data.key.decimal = (i64)key;
        break;
    case STRING_ELEM:
        node->data.key.string = (u8*)key;
        break;
    default:
        fprintf(stderr, "%s\n", "set_key_ func err: key != decimal or string");
        break;
    }
}

static void set_value_(tree_node *node, vtype_tree_t tvalue, void *value) {
    switch (tvalue) {
    case DECIMAL_ELEM:
        node->data.value.decimal = (i64)value;
        break;
    case REAL_ELEM:
        node->data.value.real = *(double*)value;
        free((double*)value);
        break;
    case STRING_ELEM:
        node->data.value.string = (u8*)value;
        break;
    default:
        break;
    }
}

static void set_tree_(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value) {
    i32 cond;
    switch (tkey) {
    case DECIMAL_ELEM:
        if((i64)key > node->data.key.decimal) {
            if(node->right == NULL) {
                node->right = new_node_(tkey, tvalue, key, value);
                node->right->parent = node;
            } else {
                set_tree_(node->right, tkey, tvalue, key, value);
            }
        } else if((i64)key < node->data.key.decimal) {
            if(node->left == NULL) {
                node->left = new_node_(tkey, tvalue, key, value);
                node->left->parent = node;
            } else {
                set_tree_(node->left, tkey, tvalue, key, value);
            }
        } else {
            set_value_(node, tvalue, value);
        }
        break;
    case STRING_ELEM:
        cond = strcmp((u8*)key, node->data.key.string);
        if(cond > 0) {
            if(node->right == NULL) {
                node->right = new_node_(tkey, tvalue, key, value);
                node->right->parent = node;
            } else {
                set_tree_(node->right, tkey, tvalue, key, value);
            }
        } else if(cond < 0) {
            if(node->left == NULL) {
                node->left = new_node_(tkey, tvalue, key, value);
                node->left->parent = node;
            } else {
                set_tree_(node->left, tkey, tvalue, key, value);
            }
        } else {
            set_value_(node, tvalue, value);
        }
        break;
    default:
        fprintf(stderr, "%s\n", "_set_tree func err: key != decimal or string");
        break;
    }
}

static void print_tree_elem_(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue) {
    switch (tkey) {
    case DECIMAL_ELEM:
        printf(" [%ld => ", node->data.key.decimal);
        switch(tvalue) {
                case DECIMAL_ELEM:
                    printf("%ld] ", node->data.value.decimal);
                break;
                case REAL_ELEM:
                    printf("%lf] ", node->data.value.real);
                break;
                case STRING_ELEM:
                    printf("'%s'] ", node->data.value.string);
                break;
            }
        break;
    case STRING_ELEM:
        printf(" ['%s' => ", node->data.key.string);
        switch(tvalue) {
                case DECIMAL_ELEM:
                    printf("%ld] ", node->data.value.decimal);
                break;
                case REAL_ELEM:
                    printf("%lf] ", node->data.value.real);
                break;
                case STRING_ELEM:
                    printf("'%s'] ", node->data.value.string);
                break;
            }
    default:
        break;
    }
}

static void print_tree_(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue) {
    if(node == NULL) {
        printf("#NULL#");
        return;
    }
    printf("{ ");
    print_tree_(node->left, tkey, tvalue);
    print_tree_elem_(node, tkey, tvalue);
    print_tree_(node->right, tkey, tvalue);
    printf(" } ");
}

static void print_tree_as_list_(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue) {
    if(node == NULL) {
        return;
    }
    print_tree_as_list_(node->left, tkey, tvalue);
    print_tree_elem_(node, tkey, tvalue);
    print_tree_as_list_(node->right, tkey, tvalue);
}

static void free_tree_(tree_node *node) {
    if(node == NULL) {
        return;
    }
    free_tree_(node->left);
    free_tree_(node->right);
    free(node);
}

static tree_node *get_tree_(tree_node *node, vtype_tree_t tkey, void *key) {
    if(node == NULL) {
        return NULL;
    }
    i32 cond;
    switch (tkey) {
    case DECIMAL_ELEM:
        if((i64)key > node->data.key.decimal) {
            return get_tree_(node->right, tkey, key);
        } else if((i64)key < node->data.key.decimal) {
            return get_tree_(node->left, tkey, key);
        }
        break;
    case STRING_ELEM:
        cond = strcmp((u8*)key, node->data.key.string);
        if(cond > 0) {
            return get_tree_(node->right, tkey, key);
        } else if(cond < 0) {
            return get_tree_(node->left, tkey, key);
        }
        break;
    default:
        break;
    }

    return node;
}

static tree_node *del_tree_f_(Tree* tree, vtype_tree_t tkey, void *key) {
    tree_node *node = tree->node;
    node = get_tree_(node, tkey, key);
    if(node == NULL) {
        return NULL;
    }
    if(node->left != NULL || node->right != NULL) {
        return node;
    }
    tree_node *parent = node->parent;
    if(parent == NULL) {
        tree->node = NULL;
    } else if (parent->left == node) {
        parent->left = NULL;
    } else {
        parent->right = NULL;
    }
    free(node);
    return NULL;
}

static void del_tree_s_(Tree* tree, tree_node *node) {
    tree_node *parent = node->parent;
    tree_node *temp = NULL;
    if(node->right != NULL) {
        temp = node->right;
    } else {
        temp = node->left;
    }
    if(parent == NULL) {
        tree->node = temp;
    } else if(parent->left == node) {
        parent->left = temp;
    } else {
        parent->right = temp;
    }
    temp->parent = parent;
    free(node);
}

static void del_tree_th_(tree_node *node) {
    tree_node *ptr = node->right;
    while(ptr->left != NULL) {
        ptr = ptr->left;
    }
    node->data.key = ptr->data.key;
    node->data.value = ptr->data.value;
    tree_node *parent = ptr->parent;
    if(parent->left == ptr) {
        parent->left = NULL;
    } else {
        parent->right = NULL;
    }
    free(ptr);
}

extern void *decimal(i64 x) {
    return (void*)x;
}

extern void *real(double x) {
    double *f = (double*)malloc(sizeof(double));
    *f = x;
    return (void*)f;
}

extern void *string(u8 *str) {
    return (void*)str;
}

extern Tree *new_tree(vtype_tree_t key, vtype_tree_t value) {
    switch (key) {
    case DECIMAL_ELEM: case STRING_ELEM:
        break;
    default: {
        fprintf(stderr, "%s\n", "Key type not supported");
        return NULL;
    }
    }
    
    switch (value) {
    case DECIMAL_ELEM: case STRING_ELEM: case REAL_ELEM:
        break;
    default: {
        fprintf(stderr, "%s\n", "Value type not supported");
        return NULL;
    }
    }

    Tree *tree = (Tree*)malloc(sizeof(tree));
    tree->type.key = key;
    tree->type.value = value;
    tree->node = NULL;

    return tree;
}

extern void set_tree(Tree *tree, void *key, void *value) {
    if(tree->node == NULL) {
        tree->node = new_node_(tree->type.key, tree->type.value, key, value);
        return;
    }

    set_tree_(tree->node, tree->type.key, tree->type.value, key, value);
}

extern void print_tree(Tree *tree) {
    print_tree_(tree->node, tree->type.key, tree->type.value);
    putchar('\n');
}

extern void print_tree_as_list(Tree *tree) {
    printf("[ ");
    print_tree_as_list_(tree->node, tree->type.key, tree->type.value);
    printf(" ]\n");
}

extern void free_tree(Tree *tree) {
    free_tree_(tree->node);
    free(tree);
}

extern value_tree_t get_tree(Tree *tree, void *key) {
    tree_node *node = get_tree_(tree->node, tree->type.key, key);
    if(node == NULL) {
        fprintf(stderr, "%s\n", "Value undefined");
        value_tree_t none;
        none.decimal = 0;
        return none;
    }
    return node->data.value;
}

extern _Bool in_tree(Tree *tree, void *key) {
    return get_tree_(tree->node, tree->type.key, key) != NULL;
}

extern void del_tree(Tree *tree, void *key) {
    tree_node *node = del_tree_f_(tree, tree->type.key, key);
    if(node == NULL) {
        return;
    }
    if(node->left != NULL & node->right != NULL) {
        del_tree_th_(node);
        return;
    }
    del_tree_s_(tree, node);
}