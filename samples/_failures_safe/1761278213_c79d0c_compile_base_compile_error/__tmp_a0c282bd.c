//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LINE 1024
#define MAX_TOKENS 256
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
} Token;

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_tokens(const char *input, Token *tokens, int max_tokens, int *token_count) {
    const char *p = input;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (*p && *token_count < max_tokens) {
        while (isspace((unsigned char)*p)) {
            if (*p == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            p++;
        }
        
        if (!*p) break;
        
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        
        if (isalpha((unsigned char)*p) || *p == '_') {
            int len = 0;
            while ((isalnum((unsigned char)*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[len++] = *p;
                p++;
                column++;
            }
            tokens[*token_count].value[len] = '\0';
            tokens[*token_count].type = TOKEN_IDENTIFIER;
            (*token_count)++;
        } else if (isdigit((unsigned char)*p)) {
            int len = 0;
            while (isdigit((unsigned char)*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[len++] = *p;
                p++;
                column++;
            }
            tokens[*token_count].value[len] = '\0';
            tokens[*token_count].type = TOKEN_NUMBER;
            (*token_count)++;
        } else if (*p == '"') {
            int len = 0;
            p++;
            column++;
            while (*p != '"' && *p != '\0' && len < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[len++] = *p;
                p++;
                column++;
            }
            if (*p == '"') {
                p++;
                column++;
            }
            tokens[*token_count].value[len] = '\0';
            tokens[*token_count].type = TOKEN_STRING;
            (*token_count)++;
        } else if (is_operator_char(*p)) {
            int len = 0;
            while (is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[*token_count].value[len++] = *p;
                p++;
                column++;
            }
            tokens[*token_count].value[len] = '\0';
            tokens[*token_count].type = TOKEN_OPERATOR;
            (*token_count)++;
        } else {
            tokens[*token_count].value[0] = *p;
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = TOKEN_UNKNOWN;
            (*token_count)++;
            p++;
            column++;
        }
    }
    
    if (*token_count < max_tokens) {
        tokens[*token_count].type = TOKEN_EOF;
        strcpy(tokens[*token_count].value, "EOF");
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        (*token_count)++;
    }
    
    return 0;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_EOF: printf("EOF"); break;
        case TOKEN_UNKNOWN: printf("UNKNOWN"); break;
    }
}

int main(void) {
    char input[MAX_INPUT_LINE];
    Token tokens[MAX_TOKENS];
    int token_count;
    
    printf("Enter a line of text to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (parse_tokens(input, tokens, MAX_TOKENS, &token_count) != 0) {
        fprintf(stderr, "Error parsing tokens\n");
        return 1;
    }
    
    printf("\nToken analysis:\n");
    printf("Line  Col  Type        Value\n");
    printf("----  ---  ----------  ------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-4d  %-3d  ", tokens[i].line, tokens[i].column);
        print_token_type(tokens[i].type);