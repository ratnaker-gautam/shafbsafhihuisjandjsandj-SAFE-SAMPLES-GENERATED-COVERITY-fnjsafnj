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
    
    if (isalpha(str[0])) {
        for (size_t i = 0; i < strlen(str); i++) {
            if (!isalnum(str[i])) return TOKEN_UNKNOWN;
        }
        return TOKEN_IDENTIFIER;
    }
    
    if (is_operator_char(str[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (str[0] == '"' && str[strlen(str)-1] == '"') {
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
        
        const char* start = ptr;
        size_t len = 0;
        
        if (isdigit(*ptr)) {
            while (isdigit(*ptr)) {
                ptr++;
                len++;
            }
        } else if (isalpha(*ptr)) {
            while (isalnum(*ptr)) {
                ptr++;
                len++;
            }
        } else if (is_operator_char(*ptr)) {
            ptr++;
            len++;
        } else if (*ptr == '"') {
            ptr++;
            len++;
            while (*ptr != '"' && *ptr != '\0') {
                ptr++;
                len++;
            }
            if (*ptr == '"') {
                ptr++;
                len++;
            }
        } else {
            ptr++;
            len++;
        }
        
        if (len > 0 && len < MAX_TOKEN_LEN) {
            strncpy(tokens[token_count].value, start, len);
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = classify_token(tokens[token_count].value);
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
        if (tokens[i].type >= 0 && tokens[i].type <= TOKEN_UNKNOWN) {
            if (handlers[tokens[i].type] != NULL) {
                handlers[tokens[i].type](&tokens[i]);
            }
        }
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    TokenHandler handlers[TOKEN_UNKNOWN + 1] = {NULL};
    
    handlers[TOKEN_NUMBER] = handle_number;
    handlers[TOKEN_IDENTIFIER] = handle_identifier;
    handlers[TOKEN_OPERATOR] = handle_operator;
    handlers[TOKEN_STRING] = handle_string;
    handlers[TOKEN_UNKNOWN] = handle_unknown;
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (