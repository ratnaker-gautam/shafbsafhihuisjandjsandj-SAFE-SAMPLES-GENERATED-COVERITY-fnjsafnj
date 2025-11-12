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
    printf("Found number: %s\n", token->value);
}

void handle_identifier(Token* token) {
    printf("Found identifier: %s\n", token->value);
}

void handle_operator(Token* token) {
    printf("Found operator: %s\n", token->value);
}

void handle_string(Token* token) {
    printf("Found string: %s\n", token->value);
}

void handle_unknown(Token* token) {
    printf("Unknown token: %s\n", token->value);
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    const char* p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        Token* current = &tokens[token_count];
        
        if (isdigit(*p)) {
            current->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *p++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (isalpha(*p) || *p == '_') {
            current->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *p++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (is_operator_char(*p)) {
            current->type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(*p) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *p++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (*p == '"') {
            current->type = TOKEN_STRING;
            int i = 0;
            p++;
            while (*p != '"' && *p != '\0' && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *p++;
            }
            current->value[i] = '\0';
            if (*p == '"') p++;
            token_count++;
        }
        else {
            current->type = TOKEN_UNKNOWN;
            current->value[0] = *p++;
            current->value[1] = '\0';
            token_count++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    for (int i = 0; i < count; i++) {
        TokenType type = tokens[i].type;
        if (type >= 0 && type <= TOKEN_UNKNOWN && handlers[type]) {
            handlers[type](&tokens[i]);
        }
    }
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    TokenHandler handlers[] = {
        handle_number,
        handle_identifier,
        handle_operator,
        handle_string,
        NULL,
        handle_unknown
    };
    
    printf("Tokens found:\n");
    process_tokens(tokens, token_count, handlers);
    
    return 0;
}