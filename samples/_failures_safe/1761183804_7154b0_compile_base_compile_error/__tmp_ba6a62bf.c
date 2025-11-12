//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_symbol_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (isdigit(input[*pos]) || input[*pos] == '.') {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = col;
    return 1;
}

int parse_string(const char *input, int *pos, int line, int col, struct Token *token) {
    if (input[*pos] != '"') return 0;
    (*pos)++;
    
    int start = *pos;
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') (*pos)++;
        (*pos)++;
    }
    
    if (input[*pos] != '"') return 0;
    
    int length = *pos - start;
    if (length > 63) return 0;
    
    strncpy(token->value, &input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_STRING;
    token->line = line;
    token->column = col;
    (*pos)++;
    return 1;
}

int parse_symbol(const char *input, int *pos, int line, int col, struct Token *token) {
    if (!is_symbol_char(input[*pos])) return 0;
    
    int start = *pos;
    while (is_symbol_char(input[*pos])) {
        (*pos)++;
    }
    
    int length = *pos - start;
    if (length > 63) return 0;
    
    strncpy(token->value, &input[start], length);
    token->value[length] = '\0';
    token->type = TOKEN_SYMBOL;
    token->line = line;
    token->column = col;
    return 1;
}

int get_next_token(const char *input, int *pos, int *line, int *col, struct Token *token) {
    while (input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
                (*line)++;
                *col = 1;
            } else {
                (*col)++;
            }
            (*pos)++;
            continue;
        }
        
        if (parse_number(input, pos, *line, *col, token)) {
            *col += strlen(token->value);
            return 1;
        }
        
        if (parse_string(input, pos, *line, *col, token)) {
            *col += strlen(token->value) + 2;
            return 1;
        }
        
        if (parse_symbol(input, pos, *line, *col, token)) {
            *col += strlen(token->value);
            return 1;
        }
        
        token->type = TOKEN_ERROR;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        token->line = *line;
        token->column = *col;
        (*pos)++;
        (*col)++;
        return 1;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    token->line = *line;
    token->column = *col;
    return 1;
}

void print_token(struct Token *token) {
    const char *type_names[] = {"NUMBER", "STRING", "SYMBOL", "EOF", "ERROR"};
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) printf(" '%s'", token->value);
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
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
    struct Token token;
    
    printf("Tokens:\n");
    while (get_next_token(input, &