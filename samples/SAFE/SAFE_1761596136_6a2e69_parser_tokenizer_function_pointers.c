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
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

typedef void (*TokenHandler)(Token*);

void handle_number(Token* token) {
    printf("Number: %s\n", token->value);
}

void handle_identifier(Token* token) {
    printf("Identifier: %s\n", token->value);
}

void handle_operator(Token* token) {
    printf("Operator: %s\n", token->value);
}

void handle_string(Token* token) {
    printf("String: %s\n", token->value);
}

void handle_eof(Token* token) {
    printf("End of input\n");
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
        
        if (isdigit(*ptr)) {
            int len = 0;
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (isalpha(*ptr) || *ptr == '_') {
            int len = 0;
            while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (*ptr == '"') {
            int len = 0;
            ptr++;
            while (*ptr != '"' && *ptr && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            token_count++;
        } else if (is_operator_char(*ptr)) {
            int len = 0;
            while (is_operator_char(*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        } else {
            ptr++;
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
        if (tokens[i].type >= 0 && tokens[i].type <= TOKEN_EOF) {
            handlers[tokens[i].type](&tokens[i]);
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
        handle_eof
    };
    
    printf("Tokens found:\n");
    process_tokens(tokens, token_count, handlers);
    
    return 0;
}