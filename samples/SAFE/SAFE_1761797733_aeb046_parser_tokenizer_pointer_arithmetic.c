//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
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
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char *input, char *output) {
    const char *p = input;
    char *out = output;
    
    if (*p == '\0') return 0;
    
    *out++ = *p++;
    
    if ((*p == '=' && (*(p-1) == '=' || *(p-1) == '!' || *(p-1) == '<' || *(p-1) == '>')) ||
        (*p == '&' && *(p-1) == '&') ||
        (*p == '|' && *(p-1) == '|')) {
        *out++ = *p++;
    }
    
    *out = '\0';
    return out - output;
}

int tokenize(const char *input, Token *tokens, int max_tokens) {
    const char *p = input;
    Token *current = tokens;
    int token_count = 0;
    
    while (*p != '\0' && token_count < max_tokens) {
        while (isspace((unsigned char)*p)) p++;
        
        if (*p == '\0') break;
        
        if (isalpha((unsigned char)*p) || *p == '_') {
            char *value_ptr = current->value;
            while ((isalnum((unsigned char)*p) || *p == '_') && 
                   (value_ptr - current->value) < MAX_TOKEN_LEN - 1) {
                *value_ptr++ = *p++;
            }
            *value_ptr = '\0';
            current->type = TOKEN_IDENTIFIER;
        }
        else if (isdigit((unsigned char)*p)) {
            char *value_ptr = current->value;
            while (isdigit((unsigned char)*p) && 
                   (value_ptr - current->value) < MAX_TOKEN_LEN - 1) {
                *value_ptr++ = *p++;
            }
            *value_ptr = '\0';
            current->type = TOKEN_NUMBER;
        }
        else if (*p == '"') {
            char *value_ptr = current->value;
            *value_ptr++ = *p++;
            while (*p != '"' && *p != '\0' && 
                   (value_ptr - current->value) < MAX_TOKEN_LEN - 1) {
                *value_ptr++ = *p++;
            }
            if (*p == '"') {
                *value_ptr++ = *p++;
            }
            *value_ptr = '\0';
            current->type = TOKEN_STRING;
        }
        else if (is_operator_char(*p)) {
            int len = parse_operator(p, current->value);
            if (len > 0) {
                p += len;
                current->type = TOKEN_OPERATOR;
            } else {
                current->value[0] = *p++;
                current->value[1] = '\0';
                current->type = TOKEN_UNKNOWN;
            }
        }
        else {
            current->value[0] = *p++;
            current->value[1] = '\0';
            current->type = TOKEN_UNKNOWN;
        }
        
        current++;
        token_count++;
    }
    
    if (token_count < max_tokens) {
        current->type = TOKEN_EOF;
        current->value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(TokenType type) {
    const char *names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    if (type >= TOKEN_IDENTIFIER && type <= TOKEN_UNKNOWN) {
        printf("%s", names[type]);
    }
}

int main(void) {
    char input[1024];
    Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline != NULL) *newline = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        print_token_type(tokens[i].type);
        printf("\t\t%s\n", tokens[i].value);
    }
    
    return 0;
}