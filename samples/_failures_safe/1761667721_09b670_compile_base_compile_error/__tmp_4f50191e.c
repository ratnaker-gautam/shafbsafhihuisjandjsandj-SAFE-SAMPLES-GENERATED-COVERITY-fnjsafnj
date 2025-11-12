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
    
    strncpy(token->value, &input[start], len);
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
    
    if (input[*pos] != '"') return 0;
    
    int copy_len = len;
    if (copy_len > 63) copy_len = 63;
    strncpy(token->value, &input[start], copy_len);
    token->value[copy_len] = '\0';
    token->type = TOKEN_STRING;
    token->line = line;
    token->column = col;
    
    (*pos)++;
    return 1;
}

int parse_symbol(const char *input, int *pos, int line, int col, struct Token *token) {
    if (!is_symbol_char(input[*pos])) return 0;
    
    int start = *pos;
    while (input[*pos] != '\0' && is_symbol_char(input[*pos])) {
        (*pos)++;
    }
    
    int len = *pos - start;
    if (len > 63) len = 63;
    
    strncpy(token->value, &input[start], len);
    token->value[len] = '\0';
    token->type = TOKEN_SYMBOL;
    token->line = line;
    token->column = col;
    
    return 1;
}

int get_next_token(const char *input, int *pos, int *line, int *col, struct Token *token) {
    if (input == NULL || pos == NULL || line == NULL || col == NULL || token == NULL) return 0;
    
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
            *col += (int)strlen(token->value);
            return 1;
        }
        
        if (parse_string(input, pos, *line, *col, token)) {
            *col += (int)strlen(token->value) + 2;
            return 1;
        }
        
        if (parse_symbol(input, pos, *line, *col, token)) {
            *col += (int)strlen(token->value);
            return 1;
        }
        
        (*pos)++;
        (*col)++;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    token->line = *line;
    token->column = *col;
    return 1;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {"NUMBER", "STRING", "SYMBOL", "EOF"};
    if (token->type >= TOKEN_NUMBER && token->type <= TOKEN_EOF) {
        printf("Line %d, Col %d: %s", token->line, token->column, type_names[token->type]);
        if (token->type != TOKEN_EOF) {
            printf(" '%s'", token->value);
        }
        printf("\n");
    }
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
        input[len - 1]