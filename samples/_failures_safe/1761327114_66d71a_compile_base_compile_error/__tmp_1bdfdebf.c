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
    TOKEN_EOF
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

int parse_number(char **input, Token *token) {
    char *start = *input;
    char *end = start;
    
    while (*end && isdigit(*end)) {
        end++;
    }
    
    if (end - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_identifier(char **input, Token *token) {
    char *start = *input;
    char *end = start;
    
    if (!isalpha(*end) && *end != '_') {
        return 0;
    }
    
    end++;
    while (*end && (isalnum(*end) || *end == '_')) {
        end++;
    }
    
    if (end - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = end;
    return 1;
}

int parse_operator(char **input, Token *token) {
    char *start = *input;
    char *end = start;
    
    if (!is_operator_char(*end)) {
        return 0;
    }
    
    end++;
    if (is_operator_char(*end)) {
        end++;
    }
    
    if (end - start >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    strncpy(token->value, start, end - start);
    token->value[end - start] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = end;
    return 1;
}

int parse_string(char **input, Token *token) {
    char *start = *input;
    char *end = start;
    
    if (*end != '"') {
        return 0;
    }
    
    end++;
    while (*end && *end != '"') {
        end++;
        if (end - start >= MAX_TOKEN_LEN) {
            return 0;
        }
    }
    
    if (*end != '"') {
        return 0;
    }
    
    end++;
    strncpy(token->value, start + 1, end - start - 2);
    token->value[end - start - 2] = '\0';
    token->type = TOKEN_STRING;
    *input = end;
    return 1;
}

int tokenize(char *input, Token *tokens, int max_tokens) {
    int token_count = 0;
    
    while (*input && token_count < max_tokens) {
        skip_whitespace(&input);
        
        if (!*input) {
            break;
        }
        
        Token token;
        int parsed = 0;
        
        if (parse_number(&input, &token)) {
            parsed = 1;
        } else if (parse_string(&input, &token)) {
            parsed = 1;
        } else if (parse_identifier(&input, &token)) {
            parsed = 1;
        } else if (parse_operator(&input, &token)) {
            parsed = 1;
        }
        
        if (parsed) {
            tokens[token_count] = token;
            token_count++;
        } else {
            input++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        token_count++;
    }
    
    return token_count;
}

void print_tokens(Token *tokens, int count) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    
    for (int i = 0; i < count; i++) {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
    }
}

int main() {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) {
        *newline = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens