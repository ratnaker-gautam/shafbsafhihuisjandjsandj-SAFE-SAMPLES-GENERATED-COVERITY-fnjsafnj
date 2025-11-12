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

int parse_number(const char *input, int *pos, int len, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
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
    return 1;
}

int parse_string(const char *input, int *pos, int len, struct Token *token) {
    if (input[*pos] != '"') return 0;
    (*pos)++;
    
    int start = *pos;
    while (*pos < len && input[*pos] != '"') {
        if (input[*pos] == '\\' && *pos + 1 < len) (*pos)++;
        (*pos)++;
    }
    
    if (*pos >= len || input[*pos] != '"') return 0;
    
    int value_len = *pos - start;
    if (value_len > 63) return 0;
    
    strncpy(token->value, &input[start], value_len);
    token->value[value_len] = '\0';
    (*pos)++;
    token->type = TOKEN_STRING;
    return 1;
}

int parse_symbol(const char *input, int *pos, int len, struct Token *token) {
    if (!is_symbol_char(input[*pos])) return 0;
    
    int start = *pos;
    while (*pos < len && is_symbol_char(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos - start > 63) return 0;
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_SYMBOL;
    return 1;
}

int get_next_token(const char *input, int len, int *pos, int *line, int *column, struct Token *token) {
    while (*pos < len) {
        char c = input[*pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                (*line)++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*pos)++;
            continue;
        }
        
        token->line = *line;
        token->column = *column;
        
        if (parse_number(input, pos, len, token)) {
            *column += strlen(token->value);
            return 1;
        }
        
        if (parse_string(input, pos, len, token)) {
            *column += strlen(token->value) + 2;
            return 1;
        }
        
        if (parse_symbol(input, pos, len, token)) {
            *column += strlen(token->value);
            return 1;
        }
        
        return 0;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    token->line = *line;
    token->column = *column;
    return 1;
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    struct Token token;
    
    printf("Tokens:\n");
    printf("Type      | Value               | Line | Column\n");
    printf("----------|---------------------|------|-------\n");
    
    while (get_next_token(input, len, &pos, &line, &column, &token)) {
        if (token.type == TOKEN_EOF) break;
        
        const char *type_str;
        switch (token.type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_SYMBOL: type_str = "SYMBOL"; break;
            default: type_str = "UNKNOWN"; break;
        }
        
        printf("%-9s | %-19s | %4d | %6d\n", 
               type_str, token.value, token.line, token.column);
    }
    
    if (pos < len) {
        fprintf(stderr, "Error: Unexpected character at position %d\n