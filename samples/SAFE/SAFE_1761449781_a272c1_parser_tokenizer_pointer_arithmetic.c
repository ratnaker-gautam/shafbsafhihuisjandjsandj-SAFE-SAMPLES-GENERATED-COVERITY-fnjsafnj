//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, char *output) {
    char *end = output;
    int has_dot = 0;
    
    while (*input && (isdigit(*input) || *input == '.')) {
        if (*input == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        *end++ = *input++;
    }
    *end = '\0';
    return end - output > 0;
}

int parse_identifier(const char *input, char *output) {
    char *end = output;
    
    if (*input && (isalpha(*input) || *input == '_')) {
        *end++ = *input++;
        while (*input && (isalnum(*input) || *input == '_')) {
            *end++ = *input++;
        }
        *end = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, char *output) {
    char *end = output;
    
    if (*input && is_operator_char(*input)) {
        *end++ = *input++;
        if (*input && is_operator_char(*input)) {
            *end++ = *input++;
        }
        *end = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, char *output) {
    char *end = output;
    
    if (*input == '"') {
        *end++ = *input++;
        while (*input && *input != '"') {
            if (*input == '\\' && *(input + 1)) {
                *end++ = *input++;
                *end++ = *input++;
            } else {
                *end++ = *input++;
            }
        }
        if (*input == '"') {
            *end++ = *input++;
            *end = '\0';
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    const char *ptr = input;
    int token_count = 0;
    
    while (*ptr && token_count < max_tokens) {
        while (*ptr && isspace(*ptr)) ptr++;
        
        if (!*ptr) break;
        
        if (parse_number(ptr, tokens[token_count].value)) {
            tokens[token_count].type = TOKEN_NUMBER;
            ptr += strlen(tokens[token_count].value);
            token_count++;
        } else if (parse_identifier(ptr, tokens[token_count].value)) {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            ptr += strlen(tokens[token_count].value);
            token_count++;
        } else if (parse_operator(ptr, tokens[token_count].value)) {
            tokens[token_count].type = TOKEN_OPERATOR;
            ptr += strlen(tokens[token_count].value);
            token_count++;
        } else if (parse_string(ptr, tokens[token_count].value)) {
            tokens[token_count].type = TOKEN_STRING;
            ptr += strlen(tokens[token_count].value);
            token_count++;
        } else {
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token(struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("%-12s: %s\n", type_names[token->type], token->value);
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
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count == 0) {
        fprintf(stderr, "No tokens found\n");
        return 1;
    }
    
    printf("\nTokens:\n");
    for (int i = 0; i < token_count; i++) {
        print_token(&tokens[i]);
    }
    
    return 0;
}