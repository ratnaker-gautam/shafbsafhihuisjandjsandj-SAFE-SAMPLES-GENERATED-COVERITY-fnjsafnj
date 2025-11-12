//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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

void tokenize(const char* input, struct Token* tokens, int* token_count) {
    int i = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[i] != '\0' && *token_count < MAX_TOKENS) {
        while (isspace((unsigned char)input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
        }
        
        if (input[i] == '\0') break;
        
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        
        if (isdigit((unsigned char)input[i])) {
            int j = 0;
            while (isdigit((unsigned char)input[i]) && j < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[j++] = input[i++];
                column++;
            }
            tokens[*token_count].value[j] = '\0';
            tokens[*token_count].type = TOKEN_NUMBER;
            (*token_count)++;
            continue;
        }
        
        if (is_valid_identifier_start(input[i])) {
            int j = 0;
            while (is_valid_identifier_char(input[i]) && j < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[j++] = input[i++];
                column++;
            }
            tokens[*token_count].value[j] = '\0';
            tokens[*token_count].type = TOKEN_IDENTIFIER;
            (*token_count)++;
            continue;
        }
        
        if (input[i] == '"') {
            int j = 0;
            i++;
            column++;
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[j++] = input[i++];
                column++;
            }
            if (input[i] == '"') {
                i++;
                column++;
            }
            tokens[*token_count].value[j] = '\0';
            tokens[*token_count].type = TOKEN_STRING;
            (*token_count)++;
            continue;
        }
        
        if (is_operator_char(input[i])) {
            int j = 0;
            while (is_operator_char(input[i]) && j < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[j++] = input[i++];
                column++;
            }
            tokens[*token_count].value[j] = '\0';
            tokens[*token_count].type = TOKEN_OPERATOR;
            (*token_count)++;
            continue;
        }
        
        tokens[*token_count].value[0] = input[i];
        tokens[*token_count].value[1] = '\0';
        tokens[*token_count].type = TOKEN_INVALID;
        (*token_count)++;
        i++;
        column++;
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        tokens[*token_count].value[0] = '\0';
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        (*token_count)++;
    }
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    int token_count = 0;
    
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
    
    tokenize(input, tokens, &token_count);
    
    printf("\nTokens found:\n");
    printf("%-12s %-12s %-8s %-8s\n", "Type", "Value", "Line