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
        end++;
    }
    
    if (!*end) {
        return 0;
    }
    
    end++;
    
    if (end - start - 2 > MAX_TOKEN_LEN - 1) {
        return 0;
    }
    
    strncpy(token->value, start + 1, end - start - 2);
    token->value[end - start - 2] = '\0';
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
        
        struct Token token;
        int success = 0;
        
        if (parse_number(&input, &token)) {
            success = 1;
        } else if (parse_string(&input, &token)) {
            success = 1;
        } else if (parse_identifier(&input, &token)) {
            success = 1;
        } else if (parse_operator(&input, &token)) {
            success = 1;
        }
        
        if (success) {
            tokens[token_count] = token;
            token_count++;
        } else {
            break;
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
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) {
        *newline = '\0';
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        printf("%2d. ", i + 1);
        print_token(&tokens