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

struct Token get_next_token(const char *input, int *pos, int *line, int *column) {
    struct Token token = {TOKEN_ERROR, "", *line, *column};
    int start_pos = *pos;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        token.value[0] = '\0';
        return token;
    }
    
    start_pos = *pos;
    token.line = *line;
    token.column = *column;
    
    if (is_valid_identifier_start(input[*pos])) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (input[*pos] != '\0' && is_valid_identifier_char(input[*pos]) && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (isdigit(input[*pos])) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.') && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (input[*pos] == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        (*pos)++;
        (*column)++;
        while (input[*pos] != '\0' && input[*pos] != '"' && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    if (is_operator_char(input[*pos])) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (input[*pos] != '\0' && is_operator_char(input[*pos]) && i < 63) {
            token.value[i++] = input[*pos];
            (*pos)++;
            (*column)++;
        }
        token.value[i] = '\0';
        return token;
    }
    
    token.type = TOKEN_ERROR;
    token.value[0] = input[*pos];
    token.value[1] = '\0';
    (*pos)++;
    (*column)++;
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int column = 1;
    
    printf("\nToken stream:\n");
    printf("%-12s %-15s %-6s %-6s\n", "TYPE", "VALUE", "LINE", "COL");
    printf("------------ --------------- ------ ------\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &column);
        
        printf("%-12s %-15s %-6d %-6d\n", 
               token_type_to_string(token.type), 
               token.value,
               token