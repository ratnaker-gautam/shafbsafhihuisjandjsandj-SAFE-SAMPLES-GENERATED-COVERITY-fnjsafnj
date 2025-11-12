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

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            (*pos)++;
        } else if (input[*pos] == '.' && !has_dot) {
            has_dot = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    if (*pos - start > 63) {
        return 0;
    }
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    int start = *pos;
    (*pos)++;
    int length = 0;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
            (*pos) += 2;
            length += 2;
        } else {
            (*pos)++;
            length++;
        }
        
        if (length >= 63) {
            return 0;
        }
    }
    
    if (input[*pos] != '"') {
        return 0;
    }
    
    (*pos)++;
    
    if (*pos - start - 2 > 63) {
        return 0;
    }
    
    strncpy(token->value, &input[start + 1], *pos - start - 2);
    token->value[*pos - start - 2] = '\0';
    token->type = TOKEN_STRING;
    return 1;
}

int parse_symbol(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    while (input[*pos] != '\0' && is_symbol_char(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos == start) {
        return 0;
    }
    
    if (*pos - start > 63) {
        return 0;
    }
    
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_SYMBOL;
    return 1;
}

int get_next_token(const char *input, int *pos, int *line, int *column, struct Token *token) {
    while (input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') {
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
        
        if (isdigit(input[*pos]) || (input[*pos] == '.' && isdigit(input[*pos + 1]))) {
            if (parse_number(input, pos, token)) {
                *column += (int)strlen(token->value);
                return 1;
            }
            return 0;
        }
        
        if (input[*pos] == '"') {
            int start_col = *column;
            if (parse_string(input, pos, token)) {
                *column = start_col + (*pos - (*pos - strlen(token->value) - 2));
                return 1;
            }
            return 0;
        }
        
        if (is_symbol_char(input[*pos])) {
            if (parse_symbol(input, pos, token)) {
                *column += (int)strlen(token->value);
                return 1;
            }
            return 0;
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    struct Token token;
    
    printf("Tokens:\n");
    printf("Type      | Value               | Line | Column\n");
    printf("----------|---------------------|------|-------\n");
    
    while (get_next_token(input, &pos, &line, &column, &token)) {
        const char *type_str;
        switch (token.type) {
            case