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
    if (!input || !tokens || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos && token_count < max_tokens) {
        while (isspace(*pos)) pos++;
        if (!*pos) break;
        
        Token* current = &tokens[token_count];
        
        if (isdigit(*pos)) {
            current->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*pos) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (isalpha(*pos) || *pos == '_') {
            current->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(*pos) || *pos == '_') && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (*pos == '"') {
            current->type = TOKEN_STRING;
            int i = 0;
            pos++;
            while (*pos && *pos != '"' && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            current->value[i] = '\0';
            if (*pos == '"') pos++;
            token_count++;
        }
        else if (is_operator_char(*pos)) {
            current->type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(*pos) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else {
            current->type = TOKEN_UNKNOWN;
            current->value[0] = *pos++;
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
    if (!tokens || !handlers || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        if (tokens[i].type >= 0 && tokens[i].type < TOKEN_UNKNOWN) {
            handlers[tokens[i].type](&tokens[i]);
        } else {
            handlers[TOKEN_UNKNOWN](&tokens[i]);
        }
    }
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count == 0) {
        fprintf(stderr, "No tokens found\n");
        return 1;
    }
    
    TokenHandler handlers[] = {
        handle_number,
        handle_identifier,
        handle_operator,
        handle_string,
        handle_unknown,
        handle_unknown
    };
    
    printf("\nToken analysis:\n");
    process_tokens(tokens, token_count, handlers);
    
    return 0;
}