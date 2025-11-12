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
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!' ||
           c == '&' || c == '|' || c == '^' || c == '%' ||
           c == '(' || c == ')' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ';' || c == ',';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    int i = 0;
    
    while (input[i] != '\0' && token_count < max_tokens) {
        if (isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
            continue;
        }
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit(input[i])) {
            int j = 0;
            while (isdigit(input[i]) && j < 63) {
                tokens[token_count].value[j++] = input[i++];
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
            continue;
        }
        
        if (is_valid_identifier_start(input[i])) {
            int j = 0;
            while (is_valid_identifier_char(input[i]) && j < 63) {
                tokens[token_count].value[j++] = input[i++];
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
            continue;
        }
        
        if (input[i] == '"') {
            int j = 0;
            tokens[token_count].value[j++] = input[i++];
            column++;
            
            while (input[i] != '"' && input[i] != '\0' && j < 63) {
                if (input[i] == '\\' && input[i+1] != '\0') {
                    tokens[token_count].value[j++] = input[i++];
                    tokens[token_count].value[j++] = input[i++];
                    column += 2;
                } else {
                    tokens[token_count].value[j++] = input[i++];
                    column++;
                }
            }
            
            if (input[i] == '"') {
                tokens[token_count].value[j++] = input[i++];
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
            continue;
        }
        
        if (is_operator_char(input[i])) {
            int j = 0;
            while (is_operator_char(input[i]) && j < 63) {
                tokens[token_count].value[j++] = input[i++];
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
            continue;
        }
        
        tokens[token_count].value[0] = input[i++];
        tokens[token_count].value[1] = '\0';
        tokens[token_count].type = TOKEN_UNKNOWN;
        token_count++;
        column++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }
    
    return token_count;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

int main(void) {
    char input[1024];
    printf("Enter code to tokenize (max 1023 characters):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (