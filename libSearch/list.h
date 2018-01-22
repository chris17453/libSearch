#ifndef list_H
#define list_H
#define DELIMITER '\t'
#define DELIMITER_STR "\t"
#define MAX_LINE_LEN 512

struct list
{
    char *needle;
    char *haystack;
    char *type;
    int value;
    struct list *left;
    struct list *right;
};
struct list *build_list(char *path);
struct list *build_list_with_armies(char *path);
int list_match(struct list *tree_node, char *needle);
int list_match_get_haystack(struct list *tree_node, char *needle,char *haystack, char *type);
struct list *create_node(char *needle, char *haystack, char *type, int value);
void free_node(void *node);
void insert_node(struct list **tree_node, struct list *new_node,void (*free_func)(void *));
void print_list(struct list *tree_node);
void free_list(struct list *tree_node, void (*free_func)(void *));
#endif
