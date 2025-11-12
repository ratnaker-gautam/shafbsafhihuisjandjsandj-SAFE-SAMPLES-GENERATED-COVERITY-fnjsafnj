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

int get_operator_length(const char* str) {
    if (str[0] == '\0') return 0;
    if (str[1] == '\0') return 1;
    if ((str[0] == '&' && str[1] == '&') || (str[0] == '|' && str[1] == '|') || 
        (str[0] == '=' && str[1] == '=') || (str[0] == '!' && str[1] == '=') ||
        (str[0] == '<' && str[1] == '=') || (str[0] == '>' && str[1] == '=')) {
        return 2;
    }
    return 1;
}

struct Token get_next_token(const char** input, int* line, int* column) {
    struct Token token = {TOKEN_ERROR, "", *line, *column};
    
    if (input == NULL || *input == NULL) {
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
    
    if (isalpha((unsigned char)**input) || **input == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)**input) || **input == '_') && i < 63) {
            token.value[i++] = **input;
            (*input)++;
            (*column)++;
        }
        token.value[i] = '\0';
    } else if (isdigit((unsigned char)**input)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)**input) && i < 63) {
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
        while (**input != '"' && **input != '\0' && i < 63) {
            token.value[i++] = **input;
            (*input)++;
            (*column)++;
        }
        token.value[i] = '\0';
        if (**input == '"') {
            (*input)++;
            (*column)++;
        } else {
            token.type = TOKEN_ERROR;
        }
    } else if (is_operator_char(**input)) {
        token.type = TOKEN_OPERATOR;
        int len = get_operator_length(*input);
        strncpy(token.value, *input, len);
        token.value[len] = '\0';
        *input += len;
        *column += len;
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
        (*column)++;
    }
    
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
    char input_buffer[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    const char* input_ptr = input_buffer;
    int line = 1;
    int column = 1;
    
    printf("\nToken stream:\n");
    printf("%-12s %-12s %-8s %-8s\n", "TYPE", "VALUE", "LINE", "COLUMN");
    printf("------------ ------------ -------- --------\n");
    
    while (1) {