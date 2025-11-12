//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

void tokenize(const char* input) {
    const char* ptr = input;
    token_count = 0;
    
    while (*ptr && token_count < MAX_TOKENS) {
        while (*ptr && strchr(DELIMITERS, *ptr)) ptr++;
        if (!*ptr) break;
        
        Token token = {TOKEN_UNKNOWN, ""};
        size_t len = 0;
        
        if (isalpha(*ptr) || *ptr == '_') {
            token.type = TOKEN_IDENTIFIER;
            while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
        } else if (isdigit(*ptr)) {
            token.type = TOKEN_NUMBER;
            while (isdigit(*ptr) && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
        } else if (*ptr == '"') {
            token.type = TOKEN_STRING;
            ptr++;
            while (*ptr && *ptr != '"' && len < MAX_TOKEN_LEN - 1) {
                token.value[len++] = *ptr++;
            }
            if (*ptr == '"') ptr++;
        } else if (is_operator(*ptr)) {
            token.type = TOKEN_OPERATOR;
            token.value[len++] = *ptr++;
            if (is_operator(*ptr) && len < MAX_TOKEN_LEN - 1) {
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
    
    tokens[token_count].type = TOKEN_EOF;
    strcpy(tokens[token_count].value, "");
}

Token next_token(void) {
    if (current_token < token_count) {
        return tokens[current_token++];
    }
    Token eof = {TOKEN_EOF, ""};
    return eof;
}

Token peek_token(void) {
    if (current_token < token_count) {
        return tokens[current_token];
    }
    Token eof = {TOKEN_EOF, ""};
    return eof;
}

void reset_parser(void) {
    current_token = 0;
}

void print_token(Token token) {
    const char* type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("%-12s: %s\n", type_names[token.type], token.value);
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    tokenize(input);
    
    printf("\nTokens:\n");
    for (size_t i = 0; i < token_count; i++) {
        printf("%2zu: ", i + 1);
        print_token(tokens[i]);
    }
    
    printf("\nParsing demonstration:\n");
    reset_parser();
    Token token;
    while ((token = next_token()).type != TOKEN_EOF) {
        printf("Parsed: ");
        print_token(token);
    }
    
    return 0;
}