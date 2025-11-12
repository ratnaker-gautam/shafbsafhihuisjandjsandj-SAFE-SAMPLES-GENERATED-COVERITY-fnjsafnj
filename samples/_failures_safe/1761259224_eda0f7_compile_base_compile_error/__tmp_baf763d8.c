//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

void tokenizer_init(const char* input, int* pos, int* line, int* col) {
    *pos = 0;
    *line = 1;
    *col = 1;
}

struct Token get_next_token(const char* input, int* pos, int* line, int* col) {
    struct Token token = {TOKEN_ERROR, "", *line, *col};
    int start_pos = *pos;
    int start_line = *line;
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
        token.type = TOKEN_EOF;
        token.value[0] = '\0';
        token.line = *line;
        token.column = *col;
        return token;
    }
    
    start_pos = *pos;
    start_line = *line;
    start_col = *col;
    
    if (isdigit(input[*pos])) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
            if (i < 63) {
                token.value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
        token.line = start_line;
        token.column = start_col;
        return token;
    }
    
    if (input[*pos] == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        (*pos)++;
        (*col)++;
        while (input[*pos] != '\0' && input[*pos] != '"') {
            if (i < 63) {
                token.value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
            if (input[*pos] == '\n') {
                (*line)++;
                *col = 1;
            }
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
        token.line = start_line;
        token.column = start_col;
        return token;
    }
    
    if (is_valid_identifier_start(input[*pos])) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (input[*pos] != '\0' && is_valid_identifier_char(input[*pos])) {
            if (i < 63) {
                token.value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
        token.line = start_line;
        token.column = start_col;
        return token;
    }
    
    if (is_operator_char(input[*pos])) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (input[*pos] != '\0' && is_operator_char(input[*pos]) && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[i] = '\0';
        token.line = start_line;
        token.column = start_col;
        return token;
    }
    
    token.type = TOKEN_ERROR;
    token.value[0] = input[*pos];
    token.value[1] = '\0';
    (*pos)++;
    (*col)++;
    token.line = start_line;
    token.column = start_col;
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }