//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

struct Token get_next_token(const char *input, int *pos, int *line, int *col) {
    struct Token token = {TOKEN_ERROR, "", *line, *col};
    int i = *pos;
    int start_col = *col;
    
    while (input[i] != '\0' && isspace(input[i])) {
        if (input[i] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        i++;
    }
    
    if (input[i] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "");
        *pos = i;
        return token;
    }
    
    start_col = *col;
    
    if (isdigit(input[i])) {
        int j = 0;
        while (input[i] != '\0' && (isdigit(input[i]) || input[i] == '.')) {
            if (j < 31) {
                token.value[j++] = input[i];
            }
            i++;
            (*col)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_NUMBER;
        *pos = i;
        return token;
    }
    
    if (is_valid_identifier_start(input[i])) {
        int j = 0;
        while (input[i] != '\0' && is_valid_identifier_char(input[i])) {
            if (j < 31) {
                token.value[j++] = input[i];
            }
            i++;
            (*col)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_IDENTIFIER;
        *pos = i;
        return token;
    }
    
    if (input[i] == '"') {
        int j = 0;
        i++;
        (*col)++;
        while (input[i] != '\0' && input[i] != '"') {
            if (j < 31) {
                token.value[j++] = input[i];
            }
            i++;
            (*col)++;
            if (input[i] == '\0') {
                token.type = TOKEN_ERROR;
                strcpy(token.value, "Unterminated string");
                *pos = i;
                return token;
            }
        }
        if (input[i] == '"') {
            i++;
            (*col)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_STRING;
        *pos = i;
        return token;
    }
    
    if (is_operator_char(input[i])) {
        int j = 0;
        while (input[i] != '\0' && is_operator_char(input[i]) && j < 31) {
            token.value[j++] = input[i];
            i++;
            (*col)++;
        }
        token.value[j] = '\0';
        token.type = TOKEN_OPERATOR;
        *pos = i;
        return token;
    }
    
    token.type = TOKEN_ERROR;
    snprintf(token.value, sizeof(token.value), "Unexpected character '%c'", input[i]);
    i++;
    (*col)++;
    *pos = i;
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
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
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    int col = 1;
    
    printf("\nToken stream:\n");
    printf("%-12s %-15s %-8s %-8s\n", "TYPE", "VALUE", "LINE", "COLUMN");
    printf("------------ --------------- -------- --------\n");
    
    while (1) {
        struct Token token = get_next_token(input, &pos, &line, &col