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
    return isalnum(c) || c == '_' || c == '-';
}

int parse_number(const char *str, int *pos, int len, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (*pos < len) {
        char c = str[*pos];
        if (isdigit(c)) {
            (*pos)++;
        } else if (c == '.' && !has_dot) {
            has_dot = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    if (*pos - start > 63) {
        return 0;
    }
    
    strncpy(token->value, &str[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char *str, int *pos, int len, struct Token *token) {
    if (str[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    int escape = 0;
    
    while (*pos < len) {
        char c = str[*pos];
        if (escape) {
            escape = 0;
            (*pos)++;
            continue;
        }
        
        if (c == '\\') {
            escape = 1;
            (*pos)++;
            continue;
        }
        
        if (c == '"') {
            (*pos)++;
            break;
        }
        
        (*pos)++;
    }
    
    if (*pos - start - 2 > 63) {
        return 0;
    }
    
    strncpy(token->value, &str[start + 1], *pos - start - 2);
    token->value[*pos - start - 2] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_symbol(const char *str, int *pos, int len, struct Token *token) {
    int start = *pos;
    
    while (*pos < len && is_symbol_char(str[*pos])) {
        (*pos)++;
    }
    
    if (*pos == start) return 0;
    
    if (*pos - start > 63) {
        return 0;
    }
    
    strncpy(token->value, &str[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_SYMBOL;
    return 1;
}

struct Token get_next_token(const char *str, int *pos, int len, int *line, int *column) {
    struct Token token = {TOKEN_ERROR, "", *line, *column};
    
    while (*pos < len) {
        char c = str[*pos];
        
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
        
        token.line = *line;
        token.column = *column;
        
        if (isdigit(c)) {
            if (parse_number(str, pos, len, &token)) {
                *column += (*pos - (*pos - strlen(token.value)));
                return token;
            }
        } else if (c == '"') {
            if (parse_string(str, pos, len, &token)) {
                *column += (*pos - (*pos - strlen(token.value) - 2));
                return token;
            }
        } else if (is_symbol_char(c)) {
            if (parse_symbol(str, pos, len, &token)) {
                *column += strlen(token.value);
                return token;
            }
        }
        
        token.value[0] = c;
        token.value[1] = '\0';
        token.type = TOKEN_ERROR;
        (*pos)++;
        (*column)++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
    return token;
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("Tokens:\n");
    printf("Type      | Value     | Line | Column\n");
    printf("----------|-----------|------|-------\n");
    
    while (pos < len) {
        struct Token token = get_next_token(input, &pos, len, &line, &column);
        
        const char *type_str;
        switch (token.type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break