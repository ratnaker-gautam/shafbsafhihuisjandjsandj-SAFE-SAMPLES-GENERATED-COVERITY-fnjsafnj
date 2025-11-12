//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_EOF;
    
    if (isdigit(str[0])) {
        for (size_t i = 0; str[i] != '\0'; i++) {
            if (!isdigit(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(str[0])) {
        for (size_t i = 0; str[i] != '\0'; i++) {
            if (!isalnum(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (is_operator_char(str[0])) {
        if (str[1] == '\0') return TOKEN_OPERATOR;
        return TOKEN_UNKNOWN;
    }
    
    if (str[0] == '"') {
        size_t len = strlen(str);
        if (len < 2 || str[len-1] != '"') return TOKEN_UNKNOWN;
        for (size_t i = 1; i < len-1; i++) {
            if (str[i] == '"') return TOKEN_UNKNOWN;
        }
        return TOKEN_STRING;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos && token_count < max_tokens) {
        while (isspace(*pos)) pos++;
        if (!*pos) break;
        
        const char* start = pos;
        size_t token_len = 0;
        
        if (is_operator_char(*pos)) {
            token_len = 1;
        } else if (*pos == '"') {
            const char* end_quote = strchr(pos + 1, '"');
            if (!end_quote) return -1;
            token_len = (end_quote - pos) + 1;
        } else {
            while (*pos && !isspace(*pos) && !is_operator_char(*pos)) {
                pos++;
                token_len++;
            }
            pos = start;
        }
        
        if (token_len >= MAX_TOKEN_LEN) return -1;
        
        strncpy(tokens[token_count].value, start, token_len);
        tokens[token_count].value[token_len] = '\0';
        
        tokens[token_count].type = classify_token(tokens[token_count].value);
        if (tokens[token_count].type == TOKEN_UNKNOWN) return -1;
        
        token_count++;
        pos += token_len;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void print_tokens(const Token* tokens, int count) {
    if (!tokens || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        const char* type_str = "UNKNOWN";
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_STRING: type_str = "STRING"; break;
            case TOKEN_EOF: type_str = "EOF"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("Token %d: %s -> '%s'\n", i, type_str, tokens[i].value);
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0 || len >= sizeof(input)) {
        printf("Invalid input length\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count < 0) {
        printf("Error during tokenization\n");
        return 1;
    }
    
    printf("\nTokenization result (%d tokens):\n", token_count);
    print_tokens(tokens, token_count);
    
    return 0;
}