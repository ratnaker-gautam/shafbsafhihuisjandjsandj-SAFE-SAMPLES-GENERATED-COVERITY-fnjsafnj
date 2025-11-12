//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    
    while (is_operator_char(**input) && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *(*input)++;
    }
    *dest = '\0';
    
    if (dest - token->value > 0) {
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_number(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    
    if (!isdigit(**input)) return 0;
    
    while (isdigit(**input) && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *(*input)++;
    }
    *dest = '\0';
    
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_string(const char **input, Token *token) {
    char *dest = token->value;
    
    if (**input != '"') return 0;
    (*input)++;
    
    while (**input != '"' && **input != '\0' && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *(*input)++;
    }
    
    if (**input == '"') {
        (*input)++;
        *dest = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    return 0;
}

int parse_identifier(const char **input, Token *token) {
    const char *start = *input;
    char *dest = token->value;
    
    if (!isalpha(**input) && **input != '_') return 0;
    
    *dest++ = *(*input)++;
    
    while ((isalnum(**input) || **input == '_') && (size_t)(dest - token->value) < MAX_TOKEN_LEN - 1) {
        *dest++ = *(*input)++;
    }
    *dest = '\0';
    
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

void skip_whitespace(const char **input) {
    while (isspace(**input)) {
        (*input)++;
    }
}

int get_next_token(const char **input, Token *token) {
    skip_whitespace(input);
    
    if (**input == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "EOF");
        return 1;
    }
    
    if (parse_string(input, token)) return 1;
    if (parse_number(input, token)) return 1;
    if (parse_operator(input, token)) return 1;
    if (parse_identifier(input, token)) return 1;
    
    token->type = TOKEN_ERROR;
    token->value[0] = **input;
    token->value[1] = '\0';
    (*input)++;
    return 1;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    if (type >= TOKEN_IDENTIFIER && type <= TOKEN_ERROR) {
        printf("%s", names[type]);
    }
}

int main(void) {
    char input_buffer[1024];
    Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter expression to tokenize: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    const char *input_ptr = input_buffer;
    
    while (token_count < MAX_TOKENS) {
        if (!get_next_token(&input_ptr, &tokens[token_count])) {
            break;
        }
        
        if (tokens[token_count].type == TOKEN_EOF) {
            break;
        }
        
        token_count++;
        
        if (tokens[token_count - 1].type == TOKEN_ERROR) {
            printf("Invalid character encountered\n");
            break;
        }
    }
    
    if (token_count >= MAX_TOKENS) {
        printf("Maximum token count reached\n");
    }
    
    printf("\nTokens found (%d):\n", token_count);
    for (int i = 0; i