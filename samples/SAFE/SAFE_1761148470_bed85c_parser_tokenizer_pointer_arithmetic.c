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

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

Token get_number_token(const char **input) {
    Token token = {TOKEN_NUMBER, ""};
    char *ptr = token.value;
    
    while (**input && isdigit(**input)) {
        if (ptr - token.value < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
        }
        (*input)++;
    }
    *ptr = '\0';
    return token;
}

Token get_identifier_token(const char **input) {
    Token token = {TOKEN_IDENTIFIER, ""};
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

Token get_operator_token(const char **input) {
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

Token get_string_token(const char **input) {
    Token token = {TOKEN_STRING, ""};
    char *ptr = token.value;
    
    if (**input == '"') {
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
    }
    *ptr = '\0';
    return token;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        skip_whitespace(&ptr);
        
        if (!*ptr) {
            break;
        }
        
        if (isdigit(*ptr)) {
            tokens[token_count++] = get_number_token(&ptr);
        } else if (isalpha(*ptr) || *ptr == '_') {
            tokens[token_count++] = get_identifier_token(&ptr);
        } else if (is_operator_char(*ptr)) {
            tokens[token_count++] = get_operator_token(&ptr);
        } else if (*ptr == '"') {
            tokens[token_count++] = get_string_token(&ptr);
        } else {
            Token error_token = {TOKEN_ERROR, ""};
            char *error_ptr = error_token.value;
            if (error_ptr - error_token.value < MAX_TOKEN_LEN - 1) {
                *error_ptr++ = *ptr;
            }
            *error_ptr = '\0';
            tokens[token_count++] = error_token;
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
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        print_token_type(tokens[i].type);
        printf("\t\t%s\n", tokens[i].value);
    }
    
    return 0;
}