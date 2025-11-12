//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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

void skip_whitespace(char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int parse_number(char **input, struct Token *token) {
    char *start = *input;
    char *end = *input;
    
    while (*end && isdigit(*end)) {
        end++;
    }
    
    if (end - start > MAX_TOKEN_LEN - 1) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_identifier(char **input, struct Token *token) {
    char *start = *input;
    char *end = *input;
    
    if (!isalpha(*end) && *end != '_') {
        return 0;
    }
    
    end++;
    while (*end && (isalnum(*end) || *end == '_')) {
        end++;
    }
    
    if (end - start > MAX_TOKEN_LEN - 1) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = end;
    return 1;
}

int parse_operator(char **input, struct Token *token) {
    char *start = *input;
    char *end = *input;
    
    if (!is_operator_char(*end)) {
        return 0;
    }
    
    end++;
    if (is_operator_char(*end)) {
        end++;
    }
    
    if (end - start > MAX_TOKEN_LEN - 1) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = end;
    return 1;
}

int parse_string(char **input, struct Token *token) {
    char *start = *input;
    char *end = *input;
    
    if (*end != '"') {
        return 0;
    }
    
    end++;
    while (*end && *end != '"') {
        if (*end == '\\' && *(end + 1)) {
            end += 2;
        } else {
            end++;
        }
    }
    
    if (!*end) {
        return 0;
    }
    
    end++;
    
    if (end - start > MAX_TOKEN_LEN - 1) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_STRING;
    *input = end;
    return 1;
}

int tokenize(char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        
        if (!*input) {
            break;
        }
        
        struct Token *current = tokens + token_count;
        
        if (parse_number(&input, current) ||
            parse_identifier(&input, current) ||
            parse_operator(&input, current) ||
            parse_string(&input, current)) {
            token_count++;
        } else {
            return -1;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
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
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) {
        *newline = '\0';
    }
    
    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count < 0) {
        fprintf(stderr, "Error: Invalid token encountered\n");
        return 1;
    }
    
    printf("\nTokens:\n");
    for (int i = 0; i < token_count; i++) {
        printf("%2d. ", i + 1);
        print_token(&tokens[i