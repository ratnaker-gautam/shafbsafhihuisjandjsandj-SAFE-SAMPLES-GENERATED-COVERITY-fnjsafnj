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
    return isalnum(c) || c == '_' || c == '-' || c == '.';
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
    
    int len = *pos - start;
    if (len > 63) len = 63;
    
    strncpy(token->value, input + start, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    token->line = line;
    token->column = col;
    
    return 1;
}

int parse_string(const char *input, int *pos, int line, int col, struct Token *token) {
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    int start = *pos;
    int len = 0;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
            (*pos) += 2;
            len += 2;
        } else {
            (*pos)++;
            len++;
        }
        if (len >= 63) break;
    }
    
    if (input[*pos] != '"') {
        token->type = TOKEN_ERROR;
        strcpy(token->value, "Unterminated string");
        token->line = line;
        token->column = col;
        return 0;
    }
    
    len = *pos - start;
    if (len > 63) len = 63;
    
    strncpy(token->value, input + start, len);
    token->value[len] = '\0';
    token->line = line;
    token->column = col;
    token->type = TOKEN_STRING;
    (*pos)++;
    
    return 1;
}

int parse_symbol(const char *input, int *pos, int line, int col, struct Token *token) {
    if (!is_symbol_char(input[*pos])) return 0;
    
    int start = *pos;
    while (is_symbol_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 63) len = 63;
    
    strncpy(token->value, input + start, len);
    token->value[len] = '\0';
    token->type = TOKEN_SYMBOL;
    token->line = line;
    token->column = col;
    
    return 1;
}

int get_next_token(const char *input, int *pos, int *line, int *col, struct Token *token) {
    if (input == NULL || pos == NULL || line == NULL || col == NULL || token == NULL) {
        return 0;
    }
    
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
        
        if (input[*pos] == '"') {
            return parse_string(input, pos, *line, *col, token);
        }
        
        if (isdigit(input[*pos]) || (input[*pos] == '.' && isdigit(input[*pos + 1]))) {
            return parse_number(input, pos, *line, *col, token);
        }
        
        if (is_symbol_char(input[*pos])) {
            return parse_symbol(input, pos, *line, *col, token);
        }
        
        token->type = TOKEN_ERROR;
        snprintf(token->value, sizeof(token->value), "Unexpected character '%c'", input[*pos]);
        token->line = *line;
        token->column = *col;
        (*pos)++;
        (*col)++;
        return 1;
    }
    
    token->type = TOKEN_EOF;
    strcpy(token->value, "EOF");
    token->line = *line;
    token->column = *col;
    return 1;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_SYMBOL: return "SYMBOL";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading