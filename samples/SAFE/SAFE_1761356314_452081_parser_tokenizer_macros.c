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
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_punctuation(char c) {
    return strchr(".,;:()[]{}", c) != NULL;
}

TokenType classify_token(const char* token) {
    if (token[0] == '\0') return TOKEN_UNKNOWN;
    
    if (isdigit(token[0])) {
        for (int i = 1; token[i] != '\0'; i++) {
            if (!isdigit(token[i]) && token[i] != '.') return TOKEN_WORD;
        }
        return TOKEN_NUMBER;
    }
    
    if (IS_QUOTE(token[0])) {
        if (token[strlen(token)-1] == token[0] && strlen(token) > 1) {
            return TOKEN_STRING;
        }
    }
    
    if (is_operator(token[0])) return TOKEN_OPERATOR;
    if (is_punctuation(token[0])) return TOKEN_PUNCTUATION;
    
    return TOKEN_WORD;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    char buffer[MAX_TOKEN_LEN];
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        if (IS_QUOTE(*ptr)) {
            char quote = *ptr;
            int i = 0;
            buffer[i++] = *ptr++;
            
            while (*ptr && *ptr != quote && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *ptr++;
            }
            
            if (*ptr == quote && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *ptr++;
            }
            buffer[i] = '\0';
        } else if (is_operator(*ptr) || is_punctuation(*ptr)) {
            buffer[0] = *ptr++;
            buffer[1] = '\0';
        } else {
            int i = 0;
            while (*ptr && !strchr(DELIMITERS, *ptr) && 
                   !is_operator(*ptr) && !is_punctuation(*ptr) && 
                   i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *ptr++;
            }
            buffer[i] = '\0';
        }
        
        if (strlen(buffer) > 0) {
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            tokens[token_count].type = classify_token(buffer);
            token_count++;
        }
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    switch (type) {
        case TOKEN_WORD: printf("WORD"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_PUNCTUATION: printf("PUNCTUATION"); break;
        default: printf("UNKNOWN"); break;
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int count = tokenize(input, tokens, MAX_TOKENS);
    if (count < 0) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("Found %d tokens:\n", count);
    for (int i = 0; i < count; i++) {
        printf("Token %d: '%-15s' Type: ", i + 1, tokens[i].value);
        print_token_type(tokens[i].type);
        printf("\n");
    }
    
    return 0;
}