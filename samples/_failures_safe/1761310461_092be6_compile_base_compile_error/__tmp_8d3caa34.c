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
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens, int* token_count) {
    if (input == NULL || tokens == NULL || token_count == NULL) {
        return 0;
    }
    
    int line = 1;
    int column = 1;
    int pos = 0;
    int count = 0;
    
    while (input[pos] != '\0' && count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') {
            break;
        }
        
        tokens[count].line = line;
        tokens[count].column = column;
        
        if (isdigit(input[pos])) {
            tokens[count].type = TOKEN_NUMBER;
            int len = 0;
            while (isdigit(input[pos]) && len < MAX_TOKEN_LEN - 1) {
                tokens[count].value[len++] = input[pos++];
                column++;
            }
            tokens[count].value[len] = '\0';
            count++;
        } else if (is_valid_identifier_start(input[pos])) {
            tokens[count].type = TOKEN_IDENTIFIER;
            int len = 0;
            while (is_valid_identifier_char(input[pos]) && len < MAX_TOKEN_LEN - 1) {
                tokens[count].value[len++] = input[pos++];
                column++;
            }
            tokens[count].value[len] = '\0';
            count++;
        } else if (is_operator_char(input[pos])) {
            tokens[count].type = TOKEN_OPERATOR;
            int len = 0;
            while (is_operator_char(input[pos]) && len < MAX_TOKEN_LEN - 1) {
                tokens[count].value[len++] = input[pos++];
                column++;
            }
            tokens[count].value[len] = '\0';
            count++;
        } else if (input[pos] == '"') {
            tokens[count].type = TOKEN_STRING;
            int len = 0;
            tokens[count].value[len++] = input[pos++];
            column++;
            
            while (input[pos] != '"' && input[pos] != '\0' && len < MAX_TOKEN_LEN - 1) {
                tokens[count].value[len++] = input[pos++];
                column++;
            }
            
            if (input[pos] == '"') {
                tokens[count].value[len++] = input[pos++];
                column++;
            }
            tokens[count].value[len] = '\0';
            count++;
        } else {
            pos++;
            column++;
        }
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
        tokens[count].line = line;
        tokens[count].column = column;
        count++;
    }
    
    *token_count = count;
    return 1;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER:
            printf("NUMBER");
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER");
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR");
            break;
        case TOKEN_STRING:
            printf("STRING");
            break;
        case TOKEN_EOF:
            printf("EOF");
            break;
        default:
            printf("UNKNOWN");
            break;
    }
}

void print_tokens(const struct Token* tokens, int count) {
    if (tokens == NULL || count <= 0) {
        return;
    }
    
    printf("Tokens found:\n");
    printf("%-12s %-15s %-6s %-6s\n", "Type", "Value", "Line", "Column");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-12s ", "");
        print_token_type(tokens[i].type);
        printf(" %-15s %-6d %-6d\n", tokens[i].value, tokens[i].line, tokens[i].column);
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter an expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len >