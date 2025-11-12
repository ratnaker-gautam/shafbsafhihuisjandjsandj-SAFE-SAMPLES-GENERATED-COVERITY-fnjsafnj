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
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_punctuation(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ',' || c == '.';
}

TokenType classify_token(const char* str) {
    if (isdigit(str[0])) {
        for (size_t i = 1; str[i]; i++) {
            if (!isdigit(str[i])) {
                return TOKEN_UNKNOWN;
            }
        }
        return TOKEN_NUMBER;
    }
    
    if (is_operator(str[0]) && str[1] == '\0') {
        return TOKEN_OPERATOR;
    }
    
    if (is_punctuation(str[0]) && str[1] == '\0') {
        return TOKEN_PUNCTUATION;
    }
    
    if (isalpha(str[0])) {
        for (size_t i = 1; str[i]; i++) {
            if (!isalnum(str[i])) {
                return TOKEN_UNKNOWN;
            }
        }
        return TOKEN_WORD;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input) {
    if (!input) return 0;
    
    char buffer[MAX_TOKEN_LEN];
    size_t buf_pos = 0;
    size_t input_len = strlen(input);
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (strchr(DELIMITERS, c) || c == '\0') {
            if (buf_pos > 0) {
                if (buf_pos >= MAX_TOKEN_LEN) {
                    return -1;
                }
                buffer[buf_pos] = '\0';
                
                if (token_count >= MAX_TOKENS) {
                    return -1;
                }
                
                tokens[token_count].type = classify_token(buffer);
                strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                token_count++;
                buf_pos = 0;
            }
        } else if (is_operator(c) || is_punctuation(c)) {
            if (buf_pos > 0) {
                if (buf_pos >= MAX_TOKEN_LEN) {
                    return -1;
                }
                buffer[buf_pos] = '\0';
                
                if (token_count >= MAX_TOKENS) {
                    return -1;
                }
                
                tokens[token_count].type = classify_token(buffer);
                strncpy(tokens[token_count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
                token_count++;
                buf_pos = 0;
            }
            
            if (token_count >= MAX_TOKENS) {
                return -1;
            }
            
            tokens[token_count].value[0] = c;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = classify_token(tokens[token_count].value);
            token_count++;
        } else {
            if (buf_pos >= MAX_TOKEN_LEN - 1) {
                return -1;
            }
            buffer[buf_pos++] = c;
        }
    }
    
    return 0;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_WORD: return "WORD";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter text to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (tokenize(input) != 0) {
        fprintf(stderr, "Tokenization failed - input too long\n");
        return 1;
    }
    
    printf("Tokens found: %zu\n", token_count);
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: %s (%s)\n", i + 1, tokens[i].value, token_type_name(tokens[i].type));
    }
    
    return 0;
}