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

void process_tokens(Token* tokens, int count, TokenHandler* handlers) {
    for (int i = 0; i < count; i++) {
        if (tokens[i].type >= 0 && tokens[i].type < 5) {
            handlers[tokens[i].type](&tokens[i]);
        }
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int tokenize(const char* input, Token* tokens, int max_tokens) {
    int token_count = 0;
    int pos = 0;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace(input[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        if (isdigit(input[pos])) {
            int start = pos;
            while (pos < len && isdigit(input[pos])) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len < MAX_TOKEN_LEN) {
                tokens[token_count].type = TOKEN_NUMBER;
                strncpy(tokens[token_count].value, &input[start], token_len);
                tokens[token_count].value[token_len] = '\0';
                token_count++;
            }
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            int start = pos;
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_')) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len < MAX_TOKEN_LEN) {
                tokens[token_count].type = TOKEN_IDENTIFIER;
                strncpy(tokens[token_count].value, &input[start], token_len);
                tokens[token_count].value[token_len] = '\0';
                token_count++;
            }
        } else if (is_operator_char(input[pos])) {
            int start = pos;
            while (pos < len && is_operator_char(input[pos])) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len < MAX_TOKEN_LEN) {
                tokens[token_count].type = TOKEN_OPERATOR;
                strncpy(tokens[token_count].value, &input[start], token_len);
                tokens[token_count].value[token_len] = '\0';
                token_count++;
            }
        } else if (input[pos] == '"') {
            int start = pos;
            pos++;
            while (pos < len && input[pos] != '"') {
                pos++;
            }
            if (pos < len && input[pos] == '"') {
                pos++;
            }
            int token_len = pos - start;
            if (token_len < MAX_TOKEN_LEN) {
                tokens[token_count].type = TOKEN_STRING;
                strncpy(tokens[token_count].value, &input[start], token_len);
                tokens[token_count].value[token_len] = '\0';
                token_count++;
            }
        } else {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
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
    
    TokenHandler handlers[5] = {
        handle_number,
        handle_identifier,
        handle_operator,
        handle_string,
        NULL
    };
    
    printf("Tokens found:\n");
    process_tokens(tokens, token_count, handlers);
    
    return 0;
}