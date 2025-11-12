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

Token parse_number(const char **input) {
    Token token = {TOKEN_NUMBER, ""};
    char *ptr = token.value;
    int dot_count = 0;
    
    while (**input && (isdigit(**input) || **input == '.')) {
        if (**input == '.') {
            dot_count++;
            if (dot_count > 1) {
                token.type = TOKEN_ERROR;
                return token;
            }
        }
        if (ptr - token.value < MAX_TOKEN_LEN - 1) {
            *ptr++ = **input;
        }
        (*input)++;
    }
    *ptr = '\0';
    return token;
}

Token parse_string(const char **input) {
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
    } else {
        token.type = TOKEN_ERROR;
    }
    *ptr = '\0';
    return token;
}

Token parse_identifier(const char **input) {
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

Token parse_operator(const char **input) {
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

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens) {
        skip_whitespace(&input);
        if (!*input) break;
        
        Token token;
        
        if (isdigit(*input)) {
            token = parse_number(&input);
        } else if (*input == '"') {
            token = parse_string(&input);
        } else if (is_operator_char(*input)) {
            token = parse_operator(&input);
        } else if (isalpha(*input) || *input == '_') {
            token = parse_identifier(&input);
        } else {
            token.type = TOKEN_ERROR;
            strncpy(token.value, input, 1);
            token.value[1] = '\0';
            input++;
        }
        
        if (token.type == TOKEN_ERROR) {
            tokens[count++] = token;
            break;
        }
        
        tokens[count++] = token;
    }
    
    if (count < max_tokens) {
        tokens[count].type = TOKEN_EOF;
        tokens[count].value[0] = '\0';
        count++;
    }
    
    return count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    printf("%s", names[type]);
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
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\tValue\n");
    printf("----\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        print_token_type(tokens[i].type);
        printf("\t%s\n", tokens[i].value);
    }
    
    return 0;
}