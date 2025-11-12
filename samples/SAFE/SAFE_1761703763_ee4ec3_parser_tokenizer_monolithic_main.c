//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LENGTH 256

enum TokenType {
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LENGTH];
    int line;
    int column;
};

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        case TOKEN_UNKNOWN: printf("UNKNOWN"); break;
    }
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    const char* ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (*ptr == ' ' || *ptr == '\t') {
            ptr++;
            column++;
        }
        
        if (*ptr == '\0') break;
        
        if (is_operator(*ptr)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].value[0] = *ptr;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].line = line;
            tokens[token_count].column = column;
            token_count++;
            ptr++;
            column++;
        } else if (*ptr == '"') {
            int value_index = 0;
            tokens[token_count].type = TOKEN_STRING;
            tokens[token_count].value[value_index++] = *ptr;
            ptr++;
            column++;
            
            while (*ptr != '\0' && *ptr != '"' && value_index < MAX_TOKEN_LENGTH - 1) {
                tokens[token_count].value[value_index++] = *ptr;
                ptr++;
                column++;
            }
            
            if (*ptr == '"') {
                tokens[token_count].value[value_index++] = *ptr;
                ptr++;
                column++;
            }
            
            tokens[token_count].value[value_index] = '\0';
            tokens[token_count].line = line;
            tokens[token_count].column = column - value_index;
            token_count++;
        } else if (isalnum((unsigned char)*ptr)) {
            int value_index = 0;
            tokens[token_count].type = TOKEN_UNKNOWN;
            
            while ((isalnum((unsigned char)*ptr) || *ptr == '_') && value_index < MAX_TOKEN_LENGTH - 1) {
                tokens[token_count].value[value_index++] = *ptr;
                ptr++;
                column++;
            }
            
            tokens[token_count].value[value_index] = '\0';
            tokens[token_count].line = line;
            tokens[token_count].column = column - value_index;
            token_count++;
        } else {
            tokens[token_count].type = TOKEN_UNKNOWN;
            tokens[token_count].value[0] = *ptr;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].line = line;
            tokens[token_count].column = column;
            token_count++;
            ptr++;
            column++;
        }
        
        if (*ptr == '\n') {
            line++;
            column = 1;
            ptr++;
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

int main() {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter input to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: ", i);
        print_token_type(tokens[i].type);
        printf(" '%s' at line %d, column %d\n", tokens[i].value, tokens[i].line, tokens[i].column);
    }
    
    return 0;
}