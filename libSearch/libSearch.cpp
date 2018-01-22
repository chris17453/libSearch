#include "stdafx.h"



struct list *create_node(char *needle, char *haystack, char *type, int value)
{
    struct list *new_node = (struct list *)memory_allocate(sizeof(*new_node));
    char buffer[1024]; // hack
    memset(new_node, 0, sizeof(*new_node));
    new_node->needle = string_duplicate_and_caseadjust(string_reverse(buffer, needle));
    new_node->value = value;
    new_node->haystack = NULL;
    new_node->type = NULL;

    if(haystack)
        new_node->haystack = string_duplicate(haystack);
    if(type)
        new_node->type = string_duplicate(type);

    return new_node;
}

struct list *build_list_helper(char *path, int mode_filter, struct list *filter_tree);
struct list *build_list_haystack_helper(char *path, int filter_mode, struct list *filter_tree);
char *strtok_r(char *str, const char *delim, char **saveptr);

void free_node(void *node)
{
    free((*(struct list *)node).needle);
    free((*(struct list *)node).haystack);
    free(node);
}

void insert_node(struct list **tree_node, struct list *new_node,
                 void (*free_func)(void *))
{
    if(*tree_node == NULL)
        *tree_node = new_node;
    else if(strcmp((*new_node).needle, (**tree_node).needle) == 0)
        free_func(new_node);
    else if(strcmp((*new_node).needle, (**tree_node).needle) < 0)
        insert_node(&(**tree_node).left, new_node, free_func);
    else
        insert_node(&(**tree_node).right, new_node, free_func);
}

struct list *build_list(char *path)
{
    struct list *wildcards = NULL;
    wildcards = build_list_helper(path, 1, NULL);
    wildcards = build_list_helper(path, -1, wildcards);
    return wildcards;
}

struct list *build_list_with_armies(char *path)
{
    struct list *wildcards = NULL;
    wildcards = build_list_haystack_helper(path, 1, NULL);
    wildcards = build_list_haystack_helper(path, -1, wildcards);
    return wildcards;
}

struct list *build_list_haystack_helper(char *path, int filter_mode,
                                        struct list *filter_tree)
{

    struct list *tree_node = filter_tree;
    FILE *file_p = open_file(path, "r");
    char *dot_needle;

    char line[MAX_LINE_LEN] = {0};

    int mode = 0;
    char *saveptr = NULL;
    char *needle = NULL;
    char *haystack = NULL;
    char *type = NULL;
    char *wildcard = NULL;

    readline(line, MAX_LINE_LEN, file_p);

    while(strlen(line) != 0)
    {

        if((needle = strtok_r(line, DELIMITER_STR, &saveptr)) == NULL)
            error_exit("file not properly formatted, exiting.");
        if((haystack = strtok_r(NULL, DELIMITER_STR, &saveptr)) == NULL)
            error_exit("file not properly formatted, exiting.");
        if((type = strtok_r(NULL, DELIMITER_STR, &saveptr)) == NULL)
            error_exit("file not properly formatted, exiting.");
        if((wildcard = strtok_r(NULL, DELIMITER_STR, &saveptr)) == NULL)
            error_exit("file not properly formatted, exiting.");

        if(*wildcard == 't')
            mode = 1;
        else if(*wildcard == 'f')
            mode = 0;
        else
        {
            error_exit("file not properly formatted, exiting.");
            exit(EXIT_FAILURE);
        }

        if(filter_tree == NULL)
        {
            if(filter_mode < 0 || filter_mode == mode)
            {
                if(mode > 0 && line[0] != '.')
                {
                    if((dot_needle = (char *)malloc(strlen(needle) + 2)) == NULL)
                    {
                        fprintf(stderr, "Malloc failure.\n");
                        exit(EXIT_FAILURE);
                    }
                    strcpy(dot_needle + 1, needle);
                    dot_needle[0] = '.';
                    insert_node(&tree_node, create_node(dot_needle, haystack, type, mode), free);
                    free(dot_needle);
                }
                else
                    insert_node(&tree_node, create_node(needle, haystack, type, mode), free);
            }
        }
        else
        {
            if(!list_match(filter_tree, line))
                insert_node(&tree_node, create_node(line, haystack, type, mode), free);
        }
        readline(line, MAX_LINE_LEN, file_p);
    }
    close_file(file_p);

    return tree_node;
}


struct list *build_list_helper(char *path, int filter_mode,
                               struct list *filter_tree)
{
    struct list *tree_node = filter_tree;
    FILE *file_p = open_file(path, "r");
    int mode;
    char *dot_line;

    char line[MAX_LINE_LEN];
    readline(line, MAX_LINE_LEN, file_p);
    while(strlen(line) != 0)
    {
        line[strlen(line) - 1] = 0;
        char *value = strrchr(line, DELIMITER);
        if(value == NULL)
            error_exit("File not properly formatted, exiting.");
        *value++ = 0;
        mode = atoi(value);
        if (filter_tree == NULL)
        {
            if (filter_mode < 0 || filter_mode == mode)
            {
                if (mode > 0 && line[0] != '.')
                {
                    dot_line = (char *)malloc(strlen(line)+2);
                    if (dot_line)
                    {
                        strcpy(dot_line+1, line);
                        dot_line[0] = '.';
                        insert_node(&tree_node, create_node(dot_line, NULL, NULL, mode), free);
                        free(dot_line);
                    }
                    else     // better than nothing
                        insert_node(&tree_node, create_node(line, NULL, NULL, mode), free);
                }
                else
                    insert_node(&tree_node, create_node(line, NULL, NULL, mode), free);
            }
        }
        else
        {
            if (!list_match(filter_tree, line))
                insert_node(&tree_node, create_node(line, NULL, NULL, mode), free);
        }
        readline(line, MAX_LINE_LEN, file_p);
    }
    close_file(file_p);

    return tree_node;
}

int bld_match(struct list *tree_node, char *needle)
{

    if(tree_node == NULL)
        return 0;

    int compare = strcmp(needle, (*tree_node).needle);
    if(compare == 0)
        return 1;
    if((*tree_node).value == 1 &&
            strncmp(needle, (*tree_node).needle, strlen((*tree_node).needle)) == 0)
        return 1;
    if(compare < 0)
        return bld_match((*tree_node).left, needle);
    else
        return bld_match((*tree_node).right, needle);
}

int bld_match_get_haystack(struct list *tree_node, char *needle,
                           char *haystack, char *type)
{

    if(tree_node == NULL)
        return 0;

    int compare = strcmp(needle, (*tree_node).needle);
    if(compare == 0)
    {
        if(tree_node->haystack == NULL)
            haystack[0] = '\0';
        else
        {
            strncpy(haystack, (*tree_node).haystack, MAX_LINE_LEN);
            haystack[MAX_LINE_LEN - 1] = '\0';
        }

        if(tree_node->type == NULL)
            type[0] = '\0';
        else
        {
            strncpy(type, (*tree_node).type, MAX_LINE_LEN - 1);
            type[MAX_LINE_LEN - 1] = '\0';
        }

        return 1;
    }
    if((*tree_node).value == 1 &&
            strncmp(needle, (*tree_node).needle, strlen((*tree_node).needle)) == 0)
    {

        if(tree_node->haystack == NULL)
            haystack[0] = '\0';
        else
        {
            strncpy(haystack, (*tree_node).haystack, MAX_LINE_LEN - 1);
            haystack[MAX_LINE_LEN - 1] = '\0';
        }

        if(tree_node->type == NULL)
            type[0] = '\0';
        else
        {
            strncpy(type, (*tree_node).type, MAX_LINE_LEN - 1);
            type[MAX_LINE_LEN - 1] = '\0';
        }

        return 1;
    }
    if(compare < 0)
        return bld_match_get_haystack((*tree_node).left, needle, haystack, type);
    else
        return bld_match_get_haystack((*tree_node).right, needle, haystack, type);
}

int dst_match(struct list *tree_node, char *needle)
{
    int match = 0;
    char *dupe = string_duplicate_and_caseadjust(needle);

    string_in_place_reverse(dupe);
    match = bld_match(tree_node, dupe);
    free(dupe);

    return match;
}

int list_match_get_haystack(struct list *tree_node, char *needle,
                            char *haystack, char *type)
{
    int match = 0;
    char *dupe = string_duplicate_and_caseadjust(needle);
    string_in_place_reverse(dupe);
    match = bld_match_get_haystack(tree_node, dupe, haystack, type);
    free(dupe);

    return match;
}

void print_list_(struct list *tree_node)
{
    if(tree_node == NULL)
        return;
    if((*tree_node).left != NULL)
        print_list_((*tree_node).left);
    if((*tree_node).right != NULL)
        print_list_((*tree_node).right);
    printf("needle: %s value: %d\n", (*tree_node).needle, (*tree_node).value);
    fflush(stdout);
}

void print_list(struct list *tree_node)
{
    printf("printing list...\n");
    print_list(tree_node);
    printf("finished printing list\n");
}

void free_list(struct list *tree_node, void (*free_func)(void *))
{
    if(tree_node == NULL)
        return;
    if((*tree_node).left != NULL)
        free_list((*tree_node).left, free_func);
    if((*tree_node).right != NULL)
        free_list((*tree_node).right, free_func);
    free_func(tree_node);
}

