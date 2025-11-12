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
    printf("NUMBER: %s\n", token->value);
}

void handle_identifier(Token* token) {
    if (token == NULL) return;
    printf("IDENTIFIER: %s\n", token->value);
}

void handle_operator(Token* token) {
    if (token == NULL) return;
    printf("OPERATOR: %s\n", token->value);
}

void handle_string(Token* token) {
    if (token == NULL) return;
    printf("STRING: %s\n", token->value);
}

void handle_unknown(Token* token) {
    if (token == NULL) return;
    printf("UNKNOWN: %s\n", token->value);
}

void handle_eof(Token* token) {
    if (token == NULL) return;
    printf("EOF\n");
}

TokenHandler get_handler(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return handle_number;
        case TOKEN_IDENTIFIER: return handle_identifier;
        case TOKEN_OPERATOR: return handle_operator;
        case TOKEN_STRING: return handle_string;
        case TOKEN_EOF: return handle_eof;
        default: return handle_unknown;
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        Token* current = &tokens[token_count];
        
        if (isdigit(*ptr)) {
            current->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (isalpha(*ptr) || *ptr == '_') {
            current->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (is_operator_char(*ptr)) {
            current->type = TOKEN_OPERATOR;
            int i = 0;
            while (is_operator_char(*ptr) && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
            current->value[i] = '\0';
            token_count++;
        }
        else if (*ptr == '"') {
            current->type = TOKEN_STRING;
            int i = 0;
            ptr++;
            while (*ptr != '"' && *ptr != '\0' && i < MAX_TOKEN_LEN - 1) {
                current->value[i++] = *ptr++;
            }
            current->value[i] = '\0';
            if (*ptr == '"') ptr++;
            token_count++;
        }
        else {
            current->type = TOKEN_UNKNOWN;
            current->value[0] = *ptr++;
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

void process_tokens(Token* tokens, int count) {
    if (tokens == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        TokenHandler handler = get_handler(tokens[i].type);
        if (handler != NULL) {
            handler(&tokens[i]);
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
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens