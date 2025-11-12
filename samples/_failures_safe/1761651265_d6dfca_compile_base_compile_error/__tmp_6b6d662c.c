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
    
    if (str[0] == '"' && strlen(str) >= 2 && str[strlen(str)-1] == '"') {
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
        
        if (is_operator_char(*ptr)) {
            len = 1;
            ptr++;
        } else if (*ptr == '"') {
            ptr++;
            start = ptr;
            while (*ptr != '\0' && *ptr != '"') {
                ptr++;
                len++;
            }
            if (*ptr == '"') ptr++;
        } else {
            while (*ptr != '\0' && !isspace(*ptr) && !is_operator_char(*ptr)) {
                ptr++;
                len++;
            }
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

void process_tokens(Token* tokens, int count) {
    if (tokens == NULL || count <= 0) return;
    
    TokenHandler handlers[] = {
        handle_number,
        handle_identifier,
        handle_operator,
        handle_string,
        handle_unknown,
        handle_unknown
    };
    
    for (int i = 0; i < count; i++) {
        if (tokens[i].type >= 0 && tokens[i].type < (int)(sizeof(handlers)/sizeof(handlers[0]))) {
            handlers[tokens[i].type](&tokens[i]);
        } else {
            handle_unknown(&tokens[i]);
        }
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    if (token_count == 0) {
        fprintf(stderr, "No tokens found\n");
        return 1;
    }