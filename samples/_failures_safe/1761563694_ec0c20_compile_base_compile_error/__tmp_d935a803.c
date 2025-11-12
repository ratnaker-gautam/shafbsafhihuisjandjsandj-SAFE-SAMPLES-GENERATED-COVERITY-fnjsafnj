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
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int get_operator_length(const char *input) {
    if (input[0] == '\0') return 0;
    if (input[0] == '=' && input[1] == '=') return 2;
    if (input[0] == '!' && input[1] == '=') return 2;
    if (input[0] == '<' && input[1] == '=') return 2;
    if (input[0] == '>' && input[1] == '=') return 2;
    if (input[0] == '&' && input[1] == '&') return 2;
    if (input[0] == '|' && input[1] == '|') return 2;
    return 1;
}

struct Token get_next_token(const char **input, int *line, int *column) {
    struct Token token = {TOKEN_ERROR, "", *line, *column};
    
    while (**input && isspace(**input)) {
        if (**input == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*input)++;
    }
    
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    token.line = *line;
    token.column = *column;
    
    if (isdigit(**input)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (**input && (isdigit(**input) || **input == '.')) {
            if (i < 63) {
                token.value[i++] = **input;
            }
            (*input)++;
            (*column)++;
        }
        token.value[i] = '\0';
    } else if (isalpha(**input) || **input == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (**input && (isalnum(**input) || **input == '_')) {
            if (i < 63) {
                token.value[i++] = **input;
            }
            (*input)++;
            (*column)++;
        }
        token.value[i] = '\0';
    } else if (**input == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        (*input)++;
        (*column)++;
        while (**input && **input != '"') {
            if (i < 63) {
                token.value[i++] = **input;
            }
            (*input)++;
            (*column)++;
        }
        if (**input == '"') {
            (*input)++;
            (*column)++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        int len = get_operator_length(*input);
        strncpy(token.value, *input, len);
        token.value[len] = '\0';
        *input += len;
        *column += len;
    } else {
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
        (*column)++;
    }
    
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
    char input_buffer[1024];
    
    printf("Enter text to tokenize (max 1023 characters):\n");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    const char *input_ptr = input_buffer;
    int line = 1;
    int column = 1;
    
    printf("\nTokens:\n");
    printf("%-12s %-15s %-6s %-6s\n", "TYPE", "VALUE", "LINE", "COL");
    printf("----------------------------------------\n");
    
    while (1) {
        struct Token token = get_next_token(&input_ptr, &line, &column);
        
        printf("%-12s %-15s %-6d %-6d\n", 
               token_type_to_string(token.type),
               token.value,
               token.line,
               token.column);
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            break;