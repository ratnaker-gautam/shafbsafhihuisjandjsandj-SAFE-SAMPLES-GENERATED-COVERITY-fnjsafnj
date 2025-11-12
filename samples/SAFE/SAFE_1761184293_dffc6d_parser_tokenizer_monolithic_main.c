//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_symbol_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

int parse_number(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] != '\0') {
        if (input[*pos] == '.') {
            if (has_dot) break;
            has_dot = 1;
        } else if (!isdigit(input[*pos])) {
            break;
        }
        (*pos)++;
    }
    
    if (*pos - start == 0) return 0;
    
    token->type = TOKEN_NUMBER;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->line = line;
    token->column = col;
    return 1;
}

int parse_string(const char *input, int *pos, int line, int col, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    int start = ++(*pos);
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
            (*pos) += 2;
        } else {
            (*pos)++;
        }
    }
    
    if (input[*pos] != '"') return 0;
    
    int len = *pos - start;
    if (len >= 64) len = 63;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    token->line = line;
    token->column = col;
    (*pos)++;
    return 1;
}

int parse_symbol(const char *input, int *pos, int line, int col, struct Token *token) {
    int start = *pos;
    
    while (input[*pos] != '\0' && is_symbol_char(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos - start == 0) return 0;
    
    int len = *pos - start;
    if (len >= 64) len = 63;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
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
        
        if (parse_number(input, pos, *line, *col, token)) return 1;
        if (parse_string(input, pos, *line, *col, token)) return 1;
        if (parse_symbol(input, pos, *line, *col, token)) return 1;
        
        (*col)++;
        (*pos)++;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    token->line = *line;
    token->column = *col;
    return 1;
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    int pos = 0;
    int line = 1;
    int col = 1;
    struct Token token;
    
    printf("Tokens:\n");
    printf("Type      | Value               | Line | Column\n");
    printf("----------|---------------------|------|-------\n");
    
    while (get_next_token(input, &pos, &line, &col, &token)) {
        const char *type_str;
        switch (token.type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_SYMBOL: type_str = "SYMBOL"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
            default: type_str = "UNKNOWN"; break;
        }
        
        printf("%-9s | %-19s | %4d | %6d\n", 
               type_str, token.value, token.line, token.column);
        
        if (token.type == TOKEN_EOF) break;
    }
    
    return 0;
}