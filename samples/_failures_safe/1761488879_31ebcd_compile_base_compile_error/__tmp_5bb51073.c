//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_PUNCTUATION,
    TOKEN_KEYWORD,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
} Token;

const char *keywords[] = {
    "if", "else", "while", "for", "return", "int", "float", "char", "void"
};
const int num_keywords = 9;

int is_keyword(const char *str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    int column = 1;
    const char *p = input;
    
    while (*p != '\0' && token_count < max_tokens - 1) {
        while (isspace(*p)) {
            if (*p == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            p++;
        }
        
        if (*p == '\0') break;
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isalpha(*p) || *p == '_') {
            int len = 0;
            while ((isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = is_keyword(tokens[token_count].value) ? 
                                      TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        }
        else if (isdigit(*p)) {
            int len = 0;
            while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
        }
        else if (*p == '"') {
            int len = 0;
            tokens[token_count].value[len++] = *p++;
            column++;
            while (*p != '"' && *p != '\0' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
                column++;
            }
            if (*p == '"') {
                tokens[token_count].value[len++] = *p++;
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
        }
        else if (is_operator_char(*p)) {
            int len = 0;
            while (is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
        }
        else {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_PUNCTUATION;
            column++;
        }
        
        token_count++;
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    tokens[token_count].line = line;
    tokens[token_count].column = column;
    
    return token_count + 1;
}

const char *token_type_str(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[4096];
    printf("Enter C-like code to tokenize (max 4095 chars):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input,