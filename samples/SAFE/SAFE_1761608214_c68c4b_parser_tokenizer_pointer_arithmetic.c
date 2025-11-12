//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64

enum token_type {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct token {
    enum token_type type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char **input, struct token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    while (*p && isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
        *dest++ = *p++;
        len++;
    }
    *dest = '\0';
    *input = p;
    return len > 0;
}

int parse_identifier(const char **input, struct token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (*p && (isalpha(*p) || *p == '_')) {
        *dest++ = *p++;
        len++;
        
        while (*p && (isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
            *dest++ = *p++;
            len++;
        }
        *dest = '\0';
        *input = p;
        return 1;
    }
    return 0;
}

int parse_operator(const char **input, struct token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (*p && is_operator_char(*p)) {
        *dest++ = *p++;
        len++;
        
        if (*p && is_operator_char(*p) && len < MAX_TOKEN_LEN - 1) {
            *dest++ = *p++;
            len++;
        }
        *dest = '\0';
        *input = p;
        return 1;
    }
    return 0;
}

int parse_string(const char **input, struct token *token) {
    const char *p = *input;
    char *dest = token->value;
    int len = 0;
    
    if (*p == '"') {
        p++;
        while (*p && *p != '"' && len < MAX_TOKEN_LEN - 1) {
            *dest++ = *p++;
            len++;
        }
        if (*p == '"') {
            p++;
            *dest = '\0';
            *input = p;
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input, struct token *tokens, int max_tokens) {
    const char *p = input;
    int token_count = 0;
    
    while (*p && token_count < max_tokens) {
        while (*p && isspace(*p)) {
            p++;
        }
        
        if (!*p) {
            break;
        }
        
        struct token *current = tokens + token_count;
        
        if (parse_number(&p, current)) {
            current->type = TOKEN_NUMBER;
            token_count++;
        } else if (parse_identifier(&p, current)) {
            current->type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (parse_operator(&p, current)) {
            current->type = TOKEN_OPERATOR;
            token_count++;
        } else if (parse_string(&p, current)) {
            current->type = TOKEN_STRING;
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

void print_token_type(enum token_type type) {
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
    
    if (len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=", i);
        print_token_type(tokens[i].type);
        printf(", Value='%s'\n", tokens[i].value);
    }
    
    return 0;
}