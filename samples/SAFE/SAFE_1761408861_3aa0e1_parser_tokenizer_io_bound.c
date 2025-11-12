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

int parse_tokens(const char *input, struct token *tokens, int max_tokens) {
    int token_count = 0;
    const char *p = input;
    
    while (*p != '\0' && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (*p == '\0') break;
        
        if (isdigit(*p)) {
            int len = 0;
            while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        }
        else if (isalpha(*p)) {
            int len = 0;
            while ((isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *p++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_WORD;
            token_count++;
        }
        else if (is_operator(*p)) {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        }
        else if (is_punctuation(*p)) {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_PUNCTUATION;
            token_count++;
        }
        else {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_UNKNOWN;
            token_count++;
        }
    }
    
    return token_count;
}

const char* token_type_to_string(enum token_type type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_WORD: return "WORD";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[MAX_INPUT_LINE];
    struct token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = parse_tokens(input, tokens, MAX_TOKENS);
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 0;
    }
    
    printf("\nTokens found (%d):\n", token_count);
    printf("Type      | Value\n");
    printf("----------|------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-9s | %s\n", token_type_to_string(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}