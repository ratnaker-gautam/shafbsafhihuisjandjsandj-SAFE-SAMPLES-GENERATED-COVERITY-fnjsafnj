//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType { NUMBER, IDENTIFIER, OPERATOR, DELIMITER, UNKNOWN };

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int parse_number(const char **input, char *buffer) {
    char *buf_ptr = buffer;
    
    while (**input && isdigit(**input)) {
        if (buf_ptr - buffer >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *buf_ptr++ = **input;
        (*input)++;
    }
    
    *buf_ptr = '\0';
    return buf_ptr - buffer > 0;
}

int parse_identifier(const char **input, char *buffer) {
    char *buf_ptr = buffer;
    
    if (**input && (isalpha(**input) || **input == '_')) {
        if (buf_ptr - buffer >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *buf_ptr++ = **input;
        (*input)++;
        
        while (**input && (isalnum(**input) || **input == '_')) {
            if (buf_ptr - buffer >= MAX_TOKEN_LEN - 1) {
                return 0;
            }
            *buf_ptr++ = **input;
            (*input)++;
        }
        
        *buf_ptr = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char **input, char *buffer) {
    char *buf_ptr = buffer;
    
    if (**input && is_operator(**input)) {
        if (buf_ptr - buffer >= MAX_TOKEN_LEN - 1) {
            return 0;
        }
        *buf_ptr++ = **input;
        (*input)++;
        
        if (**input && is_operator(**input)) {
            if (buf_ptr - buffer >= MAX_TOKEN_LEN - 1) {
                return 0;
            }
            *buf_ptr++ = **input;
            (*input)++;
        }
        
        *buf_ptr = '\0';
        return 1;
    }
    return 0;
}

int parse_delimiter(const char **input, char *buffer) {
    if (**input && is_delimiter(**input)) {
        buffer[0] = **input;
        buffer[1] = '\0';
        (*input)++;
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    struct Token *token_ptr = tokens;
    const char *input_ptr = input;
    
    while (*input_ptr && (token_ptr - tokens) < max_tokens) {
        skip_whitespace(&input_ptr);
        
        if (!*input_ptr) {
            break;
        }
        
        if (parse_number(&input_ptr, token_ptr->value)) {
            token_ptr->type = NUMBER;
        } else if (parse_identifier(&input_ptr, token_ptr->value)) {
            token_ptr->type = IDENTIFIER;
        } else if (parse_operator(&input_ptr, token_ptr->value)) {
            token_ptr->type = OPERATOR;
        } else if (parse_delimiter(&input_ptr, token_ptr->value)) {
            token_ptr->type = DELIMITER;
        } else {
            if (token_ptr->value - tokens->value >= MAX_TOKEN_LEN - 1) {
                return -1;
            }
            token_ptr->value[0] = *input_ptr;
            token_ptr->value[1] = '\0';
            token_ptr->type = UNKNOWN;
            input_ptr++;
        }
        
        token_ptr++;
    }
    
    return token_ptr - tokens;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case NUMBER: printf("NUMBER"); break;
        case IDENTIFIER: printf("IDENTIFIER"); break;
        case OPERATOR: printf("OPERATOR"); break;
        case DELIMITER: printf("DELIMITER"); break;
        case UNKNOWN: printf("UNKNOWN"); break;
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count < 0) {
        fprintf(stderr, "Tokenization error\n