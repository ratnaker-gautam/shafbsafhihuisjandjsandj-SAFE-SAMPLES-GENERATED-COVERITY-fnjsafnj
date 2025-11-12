//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

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
    char value[MAX_TOKEN_LEN];
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

int tokenize_string(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    int i = 0;
    int input_len = strlen(input);
    
    while (i < input_len && token_count < max_tokens) {
        while (i < input_len && isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
        }
        
        if (i >= input_len) break;
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit(input[i])) {
            int j = 0;
            while (i < input_len && (isdigit(input[i]) || input[i] == '.')) {
                if (j < MAX_TOKEN_LEN - 1) {
                    tokens[token_count].value[j++] = input[i];
                }
                i++;
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (is_valid_identifier_start(input[i])) {
            int j = 0;
            while (i < input_len && is_valid_identifier_char(input[i])) {
                if (j < MAX_TOKEN_LEN - 1) {
                    tokens[token_count].value[j++] = input[i];
                }
                i++;
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (input[i] == '"') {
            int j = 0;
            i++;
            column++;
            while (i < input_len && input[i] != '"') {
                if (j < MAX_TOKEN_LEN - 1) {
                    tokens[token_count].value[j++] = input[i];
                }
                i++;
                column++;
            }
            if (i < input_len && input[i] == '"') {
                i++;
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        } else if (is_operator_char(input[i])) {
            int j = 0;
            while (i < input_len && is_operator_char(input[i]) && j < 2) {
                if (j < MAX_TOKEN_LEN - 1) {
                    tokens[token_count].value[j++] = input[i];
                }
                i++;
                column++;
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        } else {
            tokens[token_count].value[0] = input[i];
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_UNKNOWN;
            token_count++;
            i++;
            column++;
        }
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

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("Line %d, Column %d: %s ('%s')\n", 
           token.line, token.column, type_names[token.type], token.value);
}

void print_tokens(struct Token *tokens, int count) {
    printf("Tokenized output:\n");
    for (int i = 0; i < count; i++) {
        print_token(tokens[i]);
        if (tokens[i].type == TOKEN_EOF) break;
    }
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 1000) return 0;
    return 1;
}

int main(void) {
    char input[1001];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize (max 1000 characters):\n");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error