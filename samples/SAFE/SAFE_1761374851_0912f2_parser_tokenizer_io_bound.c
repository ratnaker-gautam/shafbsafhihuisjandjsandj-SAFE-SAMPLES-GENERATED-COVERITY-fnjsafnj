//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
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
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int line = 1;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) {
            if (*ptr == '\n') line++;
            ptr++;
        }
        
        if (!*ptr) break;
        
        if (isdigit(*ptr)) {
            int len = 0;
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].line = line;
            token_count++;
        }
        else if (isalpha(*ptr) || *ptr == '_') {
            int len = 0;
            while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            tokens[token_count].line = line;
            token_count++;
        }
        else if (is_operator_char(*ptr)) {
            int len = 0;
            while (is_operator_char(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].line = line;
            token_count++;
        }
        else if (*ptr == '"') {
            int len = 0;
            ptr++;
            while (*ptr != '"' && *ptr != '\0' && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            if (*ptr == '"') {
                ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            tokens[token_count].line = line;
            token_count++;
        }
        else {
            ptr++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        token_count++;
    }
    
    return token_count;
}

void print_token(struct Token token) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("Line %d: %s", token.line, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" '%s'", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize (max %d chars):\n", (int)sizeof(input) - 1);
    
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
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        printf("Warning: Input too large, truncated to %d tokens\n", MAX_TOKENS);
    }
    
    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        print_token(tokens[i]);
    }
    
    return 0;
}