//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

struct Token get_next_token(const char *input, int *pos, int *line, int *col) {
    struct Token token = {TOKEN_ERROR, "", *line, *col};
    int start_pos = *pos;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
        start_pos = *pos;
    }
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "");
        token.line = *line;
        token.column = *col;
        return token;
    }
    
    if (is_valid_identifier_start(input[*pos])) {
        int len = 0;
        while (input[*pos] != '\0' && is_valid_identifier_char(input[*pos]) && len < 63) {
            token.value[len++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[len] = '\0';
        token.type = TOKEN_IDENTIFIER;
        token.line = *line;
        token.column = *col - len;
        return token;
    }
    
    if (isdigit(input[*pos])) {
        int len = 0;
        while (input[*pos] != '\0' && isdigit(input[*pos]) && len < 63) {
            token.value[len++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[len] = '\0';
        token.type = TOKEN_NUMBER;
        token.line = *line;
        token.column = *col - len;
        return token;
    }
    
    if (input[*pos] == '"') {
        int len = 0;
        token.value[len++] = input[*pos];
        (*pos)++;
        (*col)++;
        
        while (input[*pos] != '\0' && input[*pos] != '"' && len < 63) {
            token.value[len++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        
        if (input[*pos] == '"') {
            token.value[len++] = input[*pos];
            (*pos)++;
            (*col)++;
            token.value[len] = '\0';
            token.type = TOKEN_STRING;
            token.line = *line;
            token.column = *col - len;
            return token;
        } else {
            token.type = TOKEN_ERROR;
            strcpy(token.value, "Unterminated string");
            return token;
        }
    }
    
    if (is_operator_char(input[*pos])) {
        int len = 0;
        while (input[*pos] != '\0' && is_operator_char(input[*pos]) && len < 63) {
            token.value[len++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token.value[len] = '\0';
        token.type = TOKEN_OPERATOR;
        token.line = *line;
        token.column = *col - len;
        return token;
    }
    
    token.type = TOKEN_ERROR;
    token.value[0] = input[*pos];
    token.value[1] = '\0';
    (*pos)++;
    (*col)++;
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" '%s'", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter input to tokenize (max 1023 characters):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("\nTokens:\n");
    
    while (1) {