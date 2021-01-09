#include "test.h"
//workd
extern int first_net_test() {
    int listener = listen_net("0.0.0.0:7878");
    if (listener < 0) {
        fprintf(stderr, "%d\n", listener);
    }

    printf("Server is listening...\n");

    close_net(listener);
    printf("Server closed connection...\n");
    return 0;
}
//workd
extern int first_bt_test() {
    Tree *tree = new_tree(DECIMAL_ELEM, DECIMAL_ELEM);
    for(size_t i = 0; i < 10; ++i) {
        set_tree(tree, decimal(rand()%10), decimal(rand()%10));
    }
    print_tree(tree);
    free_tree(tree);
    return 0;
}

extern int second_bt_test() {
    Tree *tree = new_tree(DECIMAL_ELEM, DECIMAL_ELEM);

    set_tree(tree, decimal(6), decimal(rand()%10));
    set_tree(tree, decimal(1), decimal(rand()%10));
    set_tree(tree, decimal(7), decimal(rand()%10));
    set_tree(tree, decimal(2), decimal(rand()%10));
    set_tree(tree, decimal(0), decimal(rand()%10));

    print_tree(tree);
    del_tree(tree, decimal(6));
    print_tree(tree);

    free_tree(tree);
    return 0;
}