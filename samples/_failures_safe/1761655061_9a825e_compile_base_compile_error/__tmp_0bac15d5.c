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
    "if", "else", "while", "for", "return", "int", "char", "float", "double", "void"
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
    return strchr("(),;{}[]", c) != NULL;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *p = input;
    
    while (*p && token_count < max_tokens - 1) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        if (isalpha(*p) || *p == '_') {
            int len = 0;
            char buffer[MAX_TOKEN_LEN];
            
            while ((isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *p++;
            }
            buffer[len] = '\0';
            
            tokens[token_count].type = is_keyword(buffer) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
        else if (isdigit(*p)) {
            int len = 0;
            char buffer[MAX_TOKEN_LEN];
            
            while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *p++;
            }
            buffer[len] = '\0';
            
            tokens[token_count].type = TOKEN_NUMBER;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
        else if (*p == '"') {
            int len = 0;
            char buffer[MAX_TOKEN_LEN];
            
            p++;
            while (*p != '"' && *p && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *p++;
            }
            if (*p == '"') p++;
            buffer[len] = '\0';
            
            tokens[token_count].type = TOKEN_STRING;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
        else if (is_operator_char(*p)) {
            int len = 0;
            char buffer[MAX_TOKEN_LEN];
            
            while (is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *p++;
            }
            buffer[len] = '\0';
            
            tokens[token_count].type = TOKEN_OPERATOR;
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
        else if (is_punctuation_char(*p)) {
            tokens[token_count].type = TOKEN_PUNCTUATION;
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            token_count++;
        }
        else {
            p++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    
    return token_count + 1;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "PUNCTUATION", "KEYWORD", "EOF"
    };
    printf("%s", names[type]);
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
    
    Token tokens[MAX_TOKENS