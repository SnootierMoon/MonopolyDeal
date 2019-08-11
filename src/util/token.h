#pragma once

typedef struct _token_node_struct * token_node;

struct _token_node_struct {
    const char * data;
    token_node next;
    unsigned pos;
};

void token_free(token_node tokens);
int token_size(token_node tokens);
token_node tokenize(const char * string);
token_node tokenize_with_delimiters(const char * string, const char * delimiters);

