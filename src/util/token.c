#include "token.h"
#include <stdlib.h>
#include <string.h>

void token_free(token_node tokens) {
    if (!tokens) {
        return;
    }
    if (tokens->next) {
        token_free(tokens->next);
    }
    free((void *)tokens->data);
}

int token_size(token_node tokens) {
    return tokens ? token_size(tokens->next) + 1 : 0;
}

token_node tokenize(const char * string) {
    return tokenize_with_delimiters(string, " \t");
}

token_node tokenize_with_delimiters(const char * string, const char * delimiters) {
    char * duplicate = strdup(string);
    char * token = duplicate;
    char * end = token;
    token_node head;
    token_node * current = &head;
    while (token) {
        strsep(&end, delimiters);
        if (strlen(token)) {
            *current = (token_node)malloc(sizeof(struct _token_node_struct));
            (*current)->data = strdup(token);
            (*current)->pos = (token - duplicate);
            current = &(*current)->next;
        }
        token = end;
    }
    *current = 0;
    free(duplicate);
    return head;
}

