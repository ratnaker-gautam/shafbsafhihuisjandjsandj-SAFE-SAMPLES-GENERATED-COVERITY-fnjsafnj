//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int parse_number(const char **input, struct Token *token) {
    int i = 0;
    while (**input && (isdigit(**input) || **input == '.')) {
        if (i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
        }
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = TOKEN_NUMBER;
    return i > 0;
}

int parse_identifier(const char **input, struct Token *token) {
    int i = 0;
    if (**input && (isalpha(**input) || **input == '_')) {
        token->value[i++] = **input;
        (*input)++;
        while (**input && (isalnum(**input) || **input == '_')) {
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = **input;
            }
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char **input, struct Token *token) {
    int i = 0;
    if (**input && is_operator_char(**input)) {
        token->value[i++] = **input;
        (*input)++;
        while (**input && is_operator_char(**input) && i < MAX_TOKEN_LEN - 1) {
            token->value[i++] = **input;
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_OPERATOR;
        return 1;
    }
    return 0;
}

int parse_string(const char **input, struct Token *token) {
    int i = 0;
    if (**input == '"') {
        (*input)++;
        while (**input && **input != '"') {
            if (i < MAX_TOKEN_LEN - 1) {
                token->value[i++] = **input;
            }
            (*input)++;
        }
        if (**input == '"') {
            (*input)++;
        }
        token->value[i] = '\0';
        token->type = TOKEN_STRING;
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *ptr = input;
    
    while (*ptr && token_count < max_tokens) {
        skip_whitespace(&ptr);
        if (!*ptr) break;
        
        struct Token *current = &tokens[token_count];
        
        if (parse_number(&ptr, current) ||
            parse_identifier(&ptr, current) ||
            parse_operator(&ptr, current) ||
            parse_string(&ptr, current)) {
            token_count++;
        } else {
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

void print_token_type(enum TokenType type) {
    const char *names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    if (type >= TOKEN_NUMBER && type <= TOKEN_EOF) {
        printf("%s", names[type]);
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
    
    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Token list:\n");
    
    int i = 0;
    while (i < token_count) {
        printf("  ");
        print_token_type(tokens[i].type);
        printf(": '%s'\n", tokens[i].value);
        i++;
    }
    
    for (int j = 0; j < token_count; j++) {
        if (tokens[j].type == TOKEN_EOF) {
            break;
        }
    }
    
    int k = 0;
    do {
        if (k < token