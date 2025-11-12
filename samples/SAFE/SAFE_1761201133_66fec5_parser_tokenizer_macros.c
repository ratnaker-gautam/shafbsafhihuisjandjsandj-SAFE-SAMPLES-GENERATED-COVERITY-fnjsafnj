//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_WORD,
    TOKEN_NUMBER,
    TOKEN_PUNCTUATION,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;

#define IS_ALPHA(c) (isalpha((unsigned char)(c)))
#define IS_DIGIT(c) (isdigit((unsigned char)(c)))
#define IS_SPACE(c) (isspace((unsigned char)(c)))
#define IS_PUNCT(c) (ispunct((unsigned char)(c)))

TokenType classify_token(const char* str) {
    if (IS_ALPHA(str[0])) return TOKEN_WORD;
    if (IS_DIGIT(str[0])) return TOKEN_NUMBER;
    if (IS_PUNCT(str[0])) return TOKEN_PUNCTUATION;
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input) {
    if (input == NULL) return 0;
    
    char buffer[MAX_TOKEN_LEN];
    size_t input_len = strlen(input);
    if (input_len == 0) return 0;
    
    const char* ptr = input;
    token_count = 0;
    
    while (*ptr != '\0' && token_count < MAX_TOKENS) {
        while (IS_SPACE(*ptr)) ptr++;
        if (*ptr == '\0') break;
        
        size_t len = 0;
        const char* start = ptr;
        
        if (IS_ALPHA(*ptr)) {
            while (IS_ALPHA(*ptr) && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *ptr++;
            }
        } else if (IS_DIGIT(*ptr)) {
            while (IS_DIGIT(*ptr) && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *ptr++;
            }
        } else if (IS_PUNCT(*ptr)) {
            buffer[len++] = *ptr++;
        } else {
            ptr++;
            continue;
        }
        
        if (len > 0) {
            buffer[len] = '\0';
            tokens[token_count].type = classify_token(buffer);
            strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
        }
    }
    
    return token_count;
}

void print_tokens(void) {
    for (size_t i = 0; i < token_count; i++) {
        const char* type_str = "UNKNOWN";
        switch (tokens[i].type) {
            case TOKEN_WORD: type_str = "WORD"; break;
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_PUNCTUATION: type_str = "PUNCTUATION"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Token %zu: [%s] Type: %s\n", i + 1, tokens[i].value, type_str);
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter text to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int count = tokenize(input);
    if (count == 0) {
        printf("No tokens found\n");
        return 1;
    }
    
    printf("Found %d tokens:\n", count);
    print_tokens();
    
    return 0;
}