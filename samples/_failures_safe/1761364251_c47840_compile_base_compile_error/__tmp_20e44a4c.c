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
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!' ||
           c == '&' || c == '|';
}

int get_operator_length(const char* str) {
    if (str[0] == '\0') return 0;
    if (str[1] == '\0') return 1;
    
    if ((str[0] == '&' && str[1] == '&') ||
        (str[0] == '|' && str[1] == '|') ||
        (str[0] == '=' && str[1] == '=') ||
        (str[0] == '!' && str[1] == '=') ||
        (str[0] == '<' && str[1] == '=') ||
        (str[0] == '>' && str[1] == '=')) {
        return 2;
    }
    return 1;
}

struct Token get_next_token(const char** input, int* line, int* column) {
    struct Token token = {TOKEN_ERROR, "", *line, *column};
    
    while (**input != '\0') {
        char c = **input;
        
        if (isspace(c)) {
            if (c == '\n') {
                (*line)++;
                *column = 1;
            } else {
                (*column)++;
            }
            (*input)++;
            continue;
        }
        
        token.line = *line;
        token.column = *column;
        
        if (isdigit(c)) {
            int i = 0;
            while (isdigit(**input) && i < 31) {
                token.value[i++] = **input;
                (*input)++;
                (*column)++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (isalpha(c) || c == '_') {
            int i = 0;
            while ((isalnum(**input) || **input == '_') && i < 31) {
                token.value[i++] = **input;
                (*input)++;
                (*column)++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_IDENTIFIER;
            return token;
        }
        
        if (c == '"') {
            int i = 0;
            (*input)++;
            (*column)++;
            
            while (**input != '\0' && **input != '"' && i < 31) {
                token.value[i++] = **input;
                (*input)++;
                (*column)++;
            }
            
            if (**input == '"') {
                (*input)++;
                (*column)++;
                token.value[i] = '\0';
                token.type = TOKEN_STRING;
                return token;
            } else {
                token.type = TOKEN_ERROR;
                return token;
            }
        }
        
        if (is_operator_char(c)) {
            int len = get_operator_length(*input);
            if (len > 0 && len <= 31) {
                strncpy(token.value, *input, len);
                token.value[len] = '\0';
                token.type = TOKEN_OPERATOR;
                *input += len;
                *column += len;
                return token;
            }
        }
        
        token.type = TOKEN_ERROR;
        return token;
    }
    
    token.type = TOKEN_EOF;
    return token;
}

void print_token(const struct Token* token) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    
    if (token->type != TOKEN_EOF && token->type != TOKEN_ERROR) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
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
    
    if (len == 0 || (len == 1 && input_buffer[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    const char* input_ptr = input_buffer;
    int line = 1;
    int column = 1;
    
    printf("\nTokens:\n");
    
    while (1) {
        struct Token token = get_next_token(&input_ptr, &line, &column);
        print_token(&token);
        
        if (token.type == TOKEN_EOF || token.type ==