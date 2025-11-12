//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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
    TOKEN_INVALID
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

int get_operator_length(const char* str) {
    if (str == NULL || strlen(str) < 1) return 0;
    if (strlen(str) >= 2) {
        char two_char[3] = {str[0], str[1], '\0'};
        if (strcmp(two_char, "==") == 0 || strcmp(two_char, "!=") == 0 ||
            strcmp(two_char, "<=") == 0 || strcmp(two_char, ">=") == 0 ||
            strcmp(two_char, "&&") == 0 || strcmp(two_char, "||") == 0) {
            return 2;
        }
    }
    return 1;
}

struct Token get_next_token(const char** input, int* line, int* column) {
    struct Token token = {TOKEN_INVALID, "", *line, *column};
    
    if (input == NULL || *input == NULL || line == NULL || column == NULL) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (isspace((unsigned char)**input)) {
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
        return token;
    }
    
    token.line = *line;
    token.column = *column;
    
    if (isdigit((unsigned char)**input)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)**input) && i < 31) {
            token.value[i++] = **input;
            (*input)++;
            (*column)++;
        }
        token.value[i] = '\0';
    } else if (isalpha((unsigned char)**input) || **input == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)**input) || **input == '_') && i < 31) {
            token.value[i++] = **input;
            (*input)++;
            (*column)++;
        }
        token.value[i] = '\0';
    } else if (**input == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        (*input)++;
        (*column)++;
        while (**input != '"' && **input != '\0' && i < 31) {
            token.value[i++] = **input;
            (*input)++;
            (*column)++;
        }
        token.value[i] = '\0';
        if (**input == '"') {
            (*input)++;
            (*column)++;
        }
    } else if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        int len = get_operator_length(*input);
        if (len > 0 && len < 32) {
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
    } else {
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
        (*column)++;
    }
    
    return token;
}

void print_token(const struct Token* token) {
    if (token == NULL) return;
    
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter text to tokenize (max 1023 characters): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    const char* input_ptr = input_buffer;
    int line = 1;
    int column = 1;
    
    printf("\nTokens:\n");
    printf("--------\n");
    
    while (1) {
        struct Token token = get_next_token(&