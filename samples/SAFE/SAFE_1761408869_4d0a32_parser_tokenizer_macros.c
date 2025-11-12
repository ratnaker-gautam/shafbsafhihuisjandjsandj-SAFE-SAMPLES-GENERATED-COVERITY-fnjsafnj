//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"
#define IS_QUOTE(c) ((c) == '\"' || (c) == '\'')

typedef enum {
    TOKEN_WORD,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_SYMBOL,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

size_t tokenize(const char *input, Token *tokens) {
    size_t token_count = 0;
    const char *ptr = input;
    
    while (*ptr && token_count < MAX_TOKENS) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        if (IS_QUOTE(*ptr)) {
            char quote = *ptr;
            ptr++;
            size_t len = 0;
            tokens[token_count].type = TOKEN_STRING;
            
            while (*ptr && *ptr != quote && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            
            if (*ptr == quote) ptr++;
            token_count++;
        } else if (isdigit(*ptr)) {
            size_t len = 0;
            tokens[token_count].type = TOKEN_NUMBER;
            
            while (*ptr && (isdigit(*ptr) || *ptr == '.') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            token_count++;
        } else if (isalpha(*ptr) || *ptr == '_') {
            size_t len = 0;
            tokens[token_count].type = TOKEN_WORD;
            
            while (*ptr && (isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            token_count++;
        } else {
            tokens[token_count].type = TOKEN_SYMBOL;
            tokens[token_count].value[0] = *ptr++;
            tokens[token_count].value[1] = '\0';
            token_count++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    return token_count;
}

void print_tokens(const Token *tokens, size_t count) {
    const char *type_names[] = {"WORD", "NUMBER", "STRING", "SYMBOL", "EOF"};
    
    for (size_t i = 0; i < count && tokens[i].type != TOKEN_EOF; i++) {
        printf("Token %zu: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t token_count = tokenize(input, tokens);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Warning: Input too large, some tokens may be truncated\n");
    }
    
    printf("\nTokens found: %zu\n", token_count);
    print_tokens(tokens, token_count);
    
    return 0;
}