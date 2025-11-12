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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char **input, Token *token) {
    char *end = NULL;
    double num = strtod(*input, &end);
    if (end == *input) return 0;
    
    int len = end - *input;
    if (len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(token->value, *input, len);
    token->value[len] = '\0';
    token->type = TOKEN_NUMBER;
    *input = end;
    return 1;
}

int parse_string(const char **input, Token *token) {
    if (**input != '"') return 0;
    
    const char *start = *input + 1;
    const char *ptr = start;
    int len = 0;
    
    while (*ptr && *ptr != '"' && len < MAX_TOKEN_LEN - 1) {
        if (*ptr == '\\' && *(ptr + 1)) {
            ptr += 2;
            len += 2;
        } else {
            ptr++;
            len++;
        }
    }
    
    if (*ptr != '"') return 0;
    
    strncpy(token->value, start, len);
    token->value[len] = '\0';
    token->type = TOKEN_STRING;
    *input = ptr + 1;
    return 1;
}

int parse_identifier(const char **input, Token *token) {
    if (!isalpha(**input) && **input != '_') return 0;
    
    const char *ptr = *input;
    int len = 0;
    
    while ((isalnum(*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
        ptr++;
        len++;
    }
    
    strncpy(token->value, *input, len);
    token->value[len] = '\0';
    token->type = TOKEN_IDENTIFIER;
    *input = ptr;
    return 1;
}

int parse_operator(const char **input, Token *token) {
    if (!is_operator_char(**input)) return 0;
    
    const char *ptr = *input;
    int len = 0;
    
    while (is_operator_char(*ptr) && len < MAX_TOKEN_LEN - 1) {
        ptr++;
        len++;
    }
    
    strncpy(token->value, *input, len);
    token->value[len] = '\0';
    token->type = TOKEN_OPERATOR;
    *input = ptr;
    return 1;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    int count = 0;
    
    while (*input && count < max_tokens) {
        while (isspace(*input)) input++;
        if (!*input) break;
        
        Token *current = tokens + count;
        
        if (parse_number(&input, current) ||
            parse_string(&input, current) ||
            parse_identifier(&input, current) ||
            parse_operator(&input, current)) {
            count++;
        } else {
            current->type = TOKEN_ERROR;
            strncpy(current->value, input, 1);
            current->value[1] = '\0';
            input++;
            count++;
        }
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
    if (type >= TOKEN_NUMBER && type <= TOKEN_ERROR) {
        printf("%s", names[type]);
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (!fgets(input, sizeof(input), stdin)) {
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