//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LINE 1024
#define MAX_TOKENS 256
#define MAX_TOKEN_LEN 64

enum token_type {
    TOKEN_NUMBER,
    TOKEN_WORD,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_UNKNOWN
};

struct token {
    enum token_type type;
    char value[MAX_TOKEN_LEN];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_punctuation(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';' || c == ':';
}

int classify_token(const char *str) {
    if (str[0] == '\0') return TOKEN_UNKNOWN;
    
    if (isdigit((unsigned char)str[0])) {
        for (int i = 0; str[i] != '\0'; i++) {
            if (!isdigit((unsigned char)str[i])) {
                return TOKEN_UNKNOWN;
            }
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha((unsigned char)str[0])) {
        for (int i = 0; str[i] != '\0'; i++) {
            if (!isalnum((unsigned char)str[i])) {
                return TOKEN_UNKNOWN;
            }
        }
        return TOKEN_WORD;
    }
    
    if (strlen(str) == 1) {
        if (is_operator(str[0])) return TOKEN_OPERATOR;
        if (is_punctuation(str[0])) return TOKEN_PUNCTUATION;
    }
    
    return TOKEN_UNKNOWN;
}

const char* token_type_name(enum token_type type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_WORD: return "WORD";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        default: return "UNKNOWN";
    }
}

int tokenize(const char *input, struct token *tokens, int max_tokens) {
    int token_count = 0;
    int pos = 0;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace((unsigned char)input[pos])) {
            pos++;
        }
        
        if (pos >= len) break;
        
        int token_start = pos;
        int token_len = 0;
        
        if (is_operator(input[pos]) || is_punctuation(input[pos])) {
            token_len = 1;
        } else {
            while (pos < len && !isspace((unsigned char)input[pos]) && 
                   !is_operator(input[pos]) && !is_punctuation(input[pos])) {
                pos++;
                token_len++;
            }
        }
        
        if (token_len > 0) {
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            
            strncpy(tokens[token_count].value, &input[token_start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = classify_token(tokens[token_count].value);
            token_count++;
            
            if (is_operator(input[token_start]) || is_punctuation(input[token_start])) {
                pos = token_start + 1;
            }
        }
    }
    
    return token_count;
}

int main(void) {
    char input[MAX_INPUT_LINE];
    struct token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 1) {
        printf("Empty input\n");
        return 0;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("\nTokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12s\t%s\n", token_type_name(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}