//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
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
    int line;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, char *output) {
    int len = 0;
    int start_pos = *pos;
    
    while (isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LEN - 1) {
            *pos = start_pos;
            return 0;
        }
        output[len++] = input[(*pos)++];
    }
    
    output[len] = '\0';
    return len > 0;
}

int parse_identifier(const char *input, int *pos, char *output) {
    int len = 0;
    int start_pos = *pos;
    
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        if (len >= MAX_TOKEN_LEN - 1) {
            *pos = start_pos;
            return 0;
        }
        output[len++] = input[(*pos)++];
    }
    
    output[len] = '\0';
    return len > 0;
}

int parse_operator(const char *input, int *pos, char *output) {
    int len = 0;
    int start_pos = *pos;
    
    if (!is_operator_char(input[*pos])) return 0;
    
    while (is_operator_char(input[*pos]) && len < 2) {
        if (len >= MAX_TOKEN_LEN - 1) {
            *pos = start_pos;
            return 0;
        }
        output[len++] = input[(*pos)++];
    }
    
    output[len] = '\0';
    return len > 0;
}

int parse_string(const char *input, int *pos, char *output) {
    int len = 0;
    int start_pos = *pos;
    
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    while (input[*pos] != '"' && input[*pos] != '\0') {
        if (len >= MAX_TOKEN_LEN - 1) {
            *pos = start_pos;
            return 0;
        }
        output[len++] = input[(*pos)++];
    }
    
    if (input[*pos] != '"') {
        *pos = start_pos;
        return 0;
    }
    (*pos)++;
    
    output[len] = '\0';
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    
    if (input == NULL || tokens == NULL || max_tokens <= 0) {
        return 0;
    }
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        
        if (parse_number(input, &pos, token->value)) {
            token->type = TOKEN_NUMBER;
            token_count++;
            continue;
        }
        
        if (parse_identifier(input, &pos, token->value)) {
            token->type = TOKEN_IDENTIFIER;
            token_count++;
            continue;
        }
        
        if (parse_operator(input, &pos, token->value)) {
            token->type = TOKEN_OPERATOR;
            token_count++;
            continue;
        }
        
        if (parse_string(input, &pos, token->value)) {
            token->type = TOKEN_STRING;
            token_count++;
            continue;
        }
        
        pos++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        token_count++;
    }
    
    return token_count;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    
    if (token == NULL) return;
    
    printf("Line %d: %s", token->line, type_names[token->type]);
    
    if (token->type != TOKEN_EOF) {
        printf(" '%s'", token->value);
    }
    
    printf("\n");
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0