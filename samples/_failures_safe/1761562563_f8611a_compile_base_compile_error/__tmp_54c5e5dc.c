//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!' ||
           c == '&' || c == '|' || c == '%' || c == '^';
}

int get_next_token(const char *input, int *pos, int *line, int *col, struct Token *token) {
    int start_pos = *pos;
    int start_col = *col;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = *line;
        token->column = *col;
        return 1;
    }
    
    start_pos = *pos;
    start_col = *col;
    
    if (isdigit(input[*pos])) {
        int len = 0;
        while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
            if (len < 63) {
                token->value[len++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        token->line = *line;
        token->column = start_col;
        return 1;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        int len = 0;
        while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (len < 63) {
                token->value[len++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        token->line = *line;
        token->column = start_col;
        return 1;
    }
    
    if (input[*pos] == '"') {
        int len = 0;
        (*pos)++;
        (*col)++;
        while (input[*pos] != '\0' && input[*pos] != '"') {
            if (len < 63) {
                token->value[len++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
            if (input[*pos] == '\0') {
                return 0;
            }
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_STRING;
        token->line = *line;
        token->column = start_col;
        return 1;
    }
    
    if (is_operator_char(input[*pos])) {
        int len = 0;
        while (input[*pos] != '\0' && is_operator_char(input[*pos]) && len < 2) {
            if (len < 63) {
                token->value[len++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token->value[len] = '\0';
        token->type = TOKEN_OPERATOR;
        token->line = *line;
        token->column = start_col;
        return 1;
    }
    
    token->type = TOKEN_UNKNOWN;
    if (63 > 0) {
        token->value[0] = input[*pos];
        token->value[1] = '\0';
    }
    (*pos)++;
    (*col)++;
    token->line = *line;
    token->column = start_col;
    return 1;
}

void print_token(struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max 1023 characters):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int pos = 0;
    int line = 1;
    int col = 1;
    struct Token token