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
    if (token == NULL) return;
    printf("Number: %s\n", token->value);
}

void handle_identifier(Token* token) {
    if (token == NULL) return;
    printf("Identifier: %s\n", token->value);
}

void handle_operator(Token* token) {
    if (token == NULL) return;
    printf("Operator: %s\n", token->value);
}

void handle_string(Token* token) {
    if (token == NULL) return;
    printf("String: %s\n", token->value);
}

void handle_unknown(Token* token) {
    if (token == NULL) return;
    printf("Unknown: %s\n", token->value);
}

void handle_eof(Token* token) {
    if (token == NULL) return;
    printf("End of input\n");
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

TokenType classify_token(const char* value) {
    if (value == NULL || value[0] == '\0') return TOKEN_UNKNOWN;
    
    if (isdigit(value[0])) return TOKEN_NUMBER;
    
    if (isalpha(value[0]) || value[0] == '_') return TOKEN_IDENTIFIER;
    
    if (value[0] == '"') return TOKEN_STRING;
    
    if (is_operator_char(value[0])) return TOKEN_OPERATOR;
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* pos = input;
    
    while (*pos != '\0' && token_count < max_tokens) {
        while (isspace(*pos)) pos++;
        
        if (*pos == '\0') break;
        
        Token* current = &tokens[token_count];
        current->type = TOKEN_UNKNOWN;
        memset(current->value, 0, MAX_TOKEN_LEN);
        
        if (isdigit(*pos)) {
            int i = 0;
            while (isdigit(*pos) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            current->type = TOKEN_NUMBER;
        }
        else if (isalpha(*pos) || *pos == '_') {
            int i = 0;
            while ((isalnum(*pos) || *pos == '_') && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            current->type = TOKEN_IDENTIFIER;
        }
        else if (*pos == '"') {
            int i = 0;
            current->value[i++] = *pos++;
            while (*pos != '"' && *pos != '\0' && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            if (*pos == '"' && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            current->type = TOKEN_STRING;
        }
        else if (is_operator_char(*pos)) {
            int i = 0;
            while (is_operator_char(*pos) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *pos++;
            }
            current->type = TOKEN_OPERATOR;
        }
        else {
            current->value[0] = *pos++;
            current->type = TOKEN_UNKNOWN;
        }
        
        if (current->value[0] != '\0') {
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
    if (tokens == NULL || handlers == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        TokenType type = tokens[i].type;
        if (type >= 0 && type <= TOKEN_UNKNOWN && handlers[type] != NULL) {
            handlers[type](&tokens[i]);
        }
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len ==