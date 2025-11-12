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
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

void add_token(TokenType type, const char* value) {
    if (token_count >= MAX_TOKENS) return;
    Token* token = &tokens[token_count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token_count++;
}

int is_punctuation(char c) {
    return c == '.' || c == ',' || c == ';' || c == ':' || c == '!' || c == '?' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}';
}

void tokenize(const char* input) {
    const char* ptr = input;
    char buffer[MAX_TOKEN_LEN];
    size_t buf_index = 0;
    
    while (*ptr && token_count < MAX_TOKENS) {
        if (isspace((unsigned char)*ptr)) {
            ptr++;
            continue;
        }
        
        if (is_punctuation(*ptr)) {
            buffer[0] = *ptr;
            buffer[1] = '\0';
            add_token(TOKEN_PUNCTUATION, buffer);
            ptr++;
            continue;
        }
        
        if (isdigit((unsigned char)*ptr)) {
            buf_index = 0;
            while (isdigit((unsigned char)*ptr) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *ptr++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_NUMBER, buffer);
            continue;
        }
        
        if (isalpha((unsigned char)*ptr)) {
            buf_index = 0;
            while (isalnum((unsigned char)*ptr) && buf_index < MAX_TOKEN_LEN - 1) {
                buffer[buf_index++] = *ptr++;
            }
            buffer[buf_index] = '\0';
            add_token(TOKEN_WORD, buffer);
            continue;
        }
        
        ptr++;
    }
    
    add_token(TOKEN_EOF, "");
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_WORD: return "WORD";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens() {
    printf("Tokens found:\n");
    for (size_t i = 0; i < token_count; i++) {
        printf("Token %zu: Type=%s, Value='%s'\n", 
               i + 1, token_type_to_string(tokens[i].type), tokens[i].value);
    }
}

int main() {
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
    
    tokenize(input);
    print_tokens();
    
    return 0;
}