//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[32];
} Token;

typedef void (*TokenHandler)(Token*);

void handle_number(Token* token) {
    printf("NUMBER: %s\n", token->value);
}

void handle_identifier(Token* token) {
    printf("IDENTIFIER: %s\n", token->value);
}

void handle_operator(Token* token) {
    printf("OPERATOR: %s\n", token->value);
}

void handle_string(Token* token) {
    printf("STRING: %s\n", token->value);
}

void skip_whitespace(const char** input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int parse_number(const char** input, Token* token) {
    int i = 0;
    while (**input && isdigit(**input)) {
        if (i < 31) {
            token->value[i++] = **input;
        }
        (*input)++;
    }
    token->value[i] = '\0';
    return i > 0;
}

int parse_identifier(const char** input, Token* token) {
    int i = 0;
    if (**input && (isalpha(**input) || **input == '_')) {
        token->value[i++] = **input;
        (*input)++;
        while (**input && (isalnum(**input) || **input == '_')) {
            if (i < 31) {
                token->value[i++] = **input;
            }
            (*input)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char** input, Token* token) {
    int i = 0;
    if (**input && is_operator_char(**input)) {
        token->value[i++] = **input;
        (*input)++;
        while (**input && is_operator_char(**input) && i < 31) {
            token->value[i++] = **input;
            (*input)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char** input, Token* token) {
    int i = 0;
    if (**input == '"') {
        (*input)++;
        while (**input && **input != '"') {
            if (i < 31) {
                token->value[i++] = **input;
            }
            (*input)++;
        }
        if (**input == '"') {
            (*input)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    return 0;
}

int get_next_token(const char** input, Token* token) {
    skip_whitespace(input);
    
    if (!**input) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, token)) {
        token->type = TOKEN_NUMBER;
        return 1;
    }
    
    if (parse_identifier(input, token)) {
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    
    if (parse_operator(input, token)) {
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    
    if (parse_string(input, token)) {
        token->type = TOKEN_STRING;
        return 1;
    }
    
    return 0;
}

void process_tokens(const char* input, TokenHandler* handlers) {
    Token token;
    const char* current = input;
    
    while (get_next_token(&current, &token)) {
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        if (token.type >= 0 && token.type <= 3) {
            handlers[token.type](&token);
        }
    }
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    TokenHandler handlers[4] = {
        handle_number,
        handle_identifier,
        handle_operator,
        handle_string
    };
    
    printf("Tokens:\n");
    process_tokens(input, handlers);
    
    return 0;
}