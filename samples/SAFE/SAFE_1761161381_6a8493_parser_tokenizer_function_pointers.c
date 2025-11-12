//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
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

typedef void (*TokenHandler)(Token*);

void handle_number(Token* token) {
    printf("NUMBER: %s\n", token->value);
}

void handle_identifier(Token* token) {
    printf("IDENTIFIER: %s\n", token->value);
}

void handle_operator(Token* token) {
    printf("OPERATOR: %s\n", token->value);
}

void handle_string(Token* token) {
    printf("STRING: %s\n", token->value);
}

void handle_unknown(Token* token) {
    printf("UNKNOWN: %s\n", token->value);
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (!*ptr) break;
        
        Token token = {TOKEN_UNKNOWN, ""};
        int len = 0;
        
        if (isdigit(*ptr)) {
            token.type = TOKEN_NUMBER;
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
        } else if (isalpha(*ptr) || *ptr == '_') {
            token.type = TOKEN_IDENTIFIER;
            while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
        } else if (*ptr == '"') {
            token.type = TOKEN_STRING;
            ptr++;
            while (*ptr != '"' && *ptr && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
        } else if (is_operator_char(*ptr)) {
            token.type = TOKEN_OPERATOR;
            while (is_operator_char(*ptr) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
        } else {
            token.value[len++] = *ptr++;
        }
        
        token.value[len] = '\0';
        if (len > 0) {
            tokens[token_count++] = token;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    for (int i = 0; i < count; i++) {
        if (tokens[i].type == TOKEN_EOF) break;
        
        if (tokens[i].type >= 0 && tokens[i].type < 6) {
            handlers[tokens[i].type](&tokens[i]);
        } else {
            handlers[TOKEN_UNKNOWN](&tokens[i]);
        }
    }
}

int main() {
    char input[1024];
    Token tokens[MAX_TOKENS];
    TokenHandler handlers[6];
    
    handlers[TOKEN_NUMBER] = handle_number;
    handlers[TOKEN_IDENTIFIER] = handle_identifier;
    handlers[TOKEN_OPERATOR] = handle_operator;
    handlers[TOKEN_STRING] = handle_string;
    handlers[TOKEN_EOF] = handle_unknown;
    handlers[TOKEN_UNKNOWN] = handle_unknown;
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    printf("Tokens:\n");
    process_tokens(tokens, token_count, handlers);
    
    return 0;
}