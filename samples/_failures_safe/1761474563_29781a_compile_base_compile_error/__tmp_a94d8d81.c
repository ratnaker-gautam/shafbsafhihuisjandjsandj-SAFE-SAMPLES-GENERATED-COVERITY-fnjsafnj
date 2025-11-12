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
} Token;

const char *keywords[] = {
    "if", "else", "while", "for", "return", "int", "char", "void", "float", "double"
};
const int num_keywords = 10;

int is_keyword(const char *str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator_char(char c) {
    return strchr("+-*/%=<>!&|", c) != NULL;
}

int is_punctuation_char(char c) {
    return strchr("(){}[],;", c) != NULL;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *p = input;
    
    while (*p && token_count < max_tokens - 1) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        if (isalpha(*p) || *p == '_') {
            int len = 0;
            while (isalnum(*p) || *p == '_') {
                if (len < MAX_TOKEN_LEN - 1) {
                    tokens[token_count].value[len++] = *p;
                }
                p++;
            }
            tokens[token_count].value[len] = '\0';
            
            if (is_keyword(tokens[token_count].value)) {
                tokens[token_count].type = TOKEN_KEYWORD;
            } else {
                tokens[token_count].type = TOKEN_IDENTIFIER;
            }
            token_count++;
        }
        else if (isdigit(*p)) {
            int len = 0;
            while (isdigit(*p) || *p == '.') {
                if (len < MAX_TOKEN_LEN - 1) {
                    tokens[token_count].value[len++] = *p;
                }
                p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (*p == '"') {
            int len = 0;
            p++;
            while (*p && *p != '"') {
                if (len < MAX_TOKEN_LEN - 1) {
                    tokens[token_count].value[len++] = *p;
                }
                p++;
            }
            if (*p == '"') p++;
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        }
        else if (is_operator_char(*p)) {
            int len = 0;
            while (is_operator_char(*p) && len < 2) {
                if (len < MAX_TOKEN_LEN - 1) {
                    tokens[token_count].value[len++] = *p;
                }
                p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else if (is_punctuation_char(*p)) {
            tokens[token_count].value[0] = *p;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_PUNCTUATION;
            token_count++;
            p++;
        }
        else {
            p++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    
    return token_count;
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
    printf("Enter code to tokenize (max 4095 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t