//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

Token get_number_token(char **input) {
    Token token = {TOKEN_NUMBER, ""};
    char *start = *input;
    char *ptr = token.value;
    
    while (**input && (isdigit(**input) || **input == '.')) {
        if (ptr - token.value < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
        }
        (*input)++;
    }
    *ptr = '\0';
    return token;
}

Token get_identifier_token(char **input) {
    Token token = {TOKEN_IDENTIFIER, ""};
    char *start = *input;
    char *ptr = token.value;
    
    while (**input && (isalnum(**input) || **input == '_')) {
        if (ptr - token.value < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
        }
        (*input)++;
    }
    *ptr = '\0';
    return token;
}

Token get_string_token(char **input) {
    Token token = {TOKEN_STRING, ""};
    char *ptr = token.value;
    
    (*input)++;
    while (**input && **input != '"') {
        if (ptr - token.value < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
        }
        (*input)++;
    }
    if (**input == '"') {
        (*input)++;
    }
    *ptr = '\0';
    return token;
}

Token get_operator_token(char **input) {
    Token token = {TOKEN_OPERATOR, ""};
    char *ptr = token.value;
    
    while (**input && is_operator_char(**input)) {
        if (ptr - token.value < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
        }
        (*input)++;
    }
    *ptr = '\0';
    return token;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        
        if (!*input) {
            break;
        }
        
        if (isdigit(*input)) {
            tokens[token_count++] = get_number_token(&input);
        } else if (isalpha(*input) || *input == '_') {
            tokens[token_count++] = get_identifier_token(&input);
        } else if (*input == '"') {
            tokens[token_count++] = get_string_token(&input);
        } else if (is_operator_char(*input)) {
            tokens[token_count++] = get_operator_token(&input);
        } else {
            Token error_token = {TOKEN_ERROR, ""};
            char *ptr = error_token.value;
            if (ptr - error_token.value < MAX_TOKEN_LEN - 1) {
                *ptr++ = *input;
            }
            *ptr = '\0';
            tokens[token_count++] = error_token;
            input++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "EOF");
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    if (type >= TOKEN_NUMBER && type <= TOKEN_ERROR) {
        printf("%s", names[type]);
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) {
        *newline = '\0';
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}