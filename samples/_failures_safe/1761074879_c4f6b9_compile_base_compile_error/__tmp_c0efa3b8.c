//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
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
        
        if (is_valid_identifier_start(c)) {
            token.type = TOKEN_IDENTIFIER;
            int i = 0;
            while (**input != '\0' && i < 63 && is_valid_identifier_char(**input)) {
                token.value[i++] = **input;
                (*input)++;
                (*column)++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (isdigit(c)) {
            token.type = TOKEN_NUMBER;
            int i = 0;
            int has_dot = 0;
            while (**input != '\0' && i < 63 && (isdigit(**input) || (**input == '.' && !has_dot))) {
                if (**input == '.') has_dot = 1;
                token.value[i++] = **input;
                (*input)++;
                (*column)++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (c == '"') {
            token.type = TOKEN_STRING;
            int i = 0;
            (*input)++;
            (*column)++;
            
            while (**input != '\0' && i < 63) {
                if (**input == '"') {
                    (*input)++;
                    (*column)++;
                    token.value[i] = '\0';
                    return token;
                }
                if (**input == '\\' && *(*input + 1) != '\0') {
                    (*input)++;
                    (*column)++;
                }
                token.value[i++] = **input;
                (*input)++;
                (*column)++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (is_operator_char(c)) {
            token.type = TOKEN_OPERATOR;
            int len = get_operator_length(*input);
            if (len > 0 && len < 64) {
                strncpy(token.value, *input, len);
                token.value[len] = '\0';
                *input += len;
                *column += len;
                return token;
            }
        }
        
        token.type = TOKEN_ERROR;
        token.value[0] = c;
        token.value[1] = '\0';
        (*input)++;
        (*column)++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
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
    
    printf("Enter code to tokenize (max 1023 characters):\n");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';