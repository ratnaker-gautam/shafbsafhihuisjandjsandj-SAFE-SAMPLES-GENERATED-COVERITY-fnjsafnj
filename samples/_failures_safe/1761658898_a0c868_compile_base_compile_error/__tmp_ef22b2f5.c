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
    printf("STRING: \"%s\"\n", token->value);
}

void handle_unknown(Token* token) {
    if (token == NULL) return;
    printf("UNKNOWN: %s\n", token->value);
}

void handle_eof(Token* token) {
    if (token == NULL) return;
    printf("EOF\n");
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

TokenType classify_token(const char* str) {
    if (str == NULL || strlen(str) == 0) return TOKEN_UNKNOWN;
    
    if (isdigit(str[0])) {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isdigit(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (is_operator_char(str[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (str[0] == '"') {
        return TOKEN_STRING;
    }
    
    return TOKEN_UNKNOWN;
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    if (input == NULL || tokens == NULL || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        if (*ptr == '\0') break;
        
        int token_len = 0;
        char current_token[MAX_TOKEN_LEN] = {0};
        
        if (is_operator_char(*ptr)) {
            current_token[token_len++] = *ptr++;
        } else if (*ptr == '"') {
            current_token[token_len++] = *ptr++;
            while (*ptr != '"' && *ptr != '\0' && token_len < MAX_TOKEN_LEN - 1) {
                current_token[token_len++] = *ptr++;
            }
            if (*ptr == '"') {
                current_token[token_len++] = *ptr++;
            }
        } else {
            while (*ptr != '\0' && !isspace(*ptr) && !is_operator_char(*ptr) && 
                   token_len < MAX_TOKEN_LEN - 1) {
                current_token[token_len++] = *ptr++;
            }
        }
        
        if (token_len > 0) {
            current_token[token_len] = '\0';
            tokens[token_count].type = classify_token(current_token);
            strncpy(tokens[token_count].value, current_token, MAX_TOKEN_LEN - 1);
            tokens[token_count].value[MAX_TOKEN_LEN - 1] = '\0';
            token_count++;
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
    if (tokens == NULL || handlers == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        if (tokens[i].type >= 0 && tokens[i].type < 6) {
            handlers[tokens[i].type](&tokens[i]);
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
    
    Token tokens[MAX_TO