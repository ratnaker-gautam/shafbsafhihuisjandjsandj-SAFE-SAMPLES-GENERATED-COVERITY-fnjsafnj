//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_TOKENS 100
#define MAX_TOKEN_LENGTH 64

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LENGTH];
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_number(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    while (isdigit(input[*pos])) {
        if (len >= MAX_TOKEN_LENGTH - 1) return 0;
        buffer[len++] = input[(*pos)++];
    }
    
    buffer[len] = '\0';
    return len > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        buffer[len++] = input[(*pos)++];
        
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (len >= MAX_TOKEN_LENGTH - 1) break;
            buffer[len++] = input[(*pos)++];
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    return 0;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    if (is_operator_char(input[*pos])) {
        buffer[len++] = input[(*pos)++];
        
        if (is_operator_char(input[*pos]) && len < MAX_TOKEN_LENGTH - 1) {
            buffer[len++] = input[(*pos)++];
        }
        
        buffer[len] = '\0';
        return 1;
    }
    
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    if (input[*pos] == '"') {
        buffer[len++] = input[(*pos)++];
        
        while (input[*pos] != '"' && input[*pos] != '\0') {
            if (len >= MAX_TOKEN_LENGTH - 1) break;
            buffer[len++] = input[(*pos)++];
        }
        
        if (input[*pos] == '"') {
            buffer[len++] = input[(*pos)++];
            buffer[len] = '\0';
            return 1;
        }
    }
    
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int *token_count) {
    int pos = 0;
    *token_count = 0;
    
    while (input[pos] != '\0' && *token_count < MAX_TOKENS) {
        while (isspace(input[pos])) pos++;
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[*token_count];
        
        if (parse_number(input, &pos, token->value)) {
            token->type = TOKEN_NUMBER;
            (*token_count)++;
        } else if (parse_identifier(input, &pos, token->value)) {
            token->type = TOKEN_IDENTIFIER;
            (*token_count)++;
        } else if (parse_operator(input, &pos, token->value)) {
            token->type = TOKEN_OPERATOR;
            (*token_count)++;
        } else if (parse_string(input, &pos, token->value)) {
            token->type = TOKEN_STRING;
            (*token_count)++;
        } else {
            return 0;
        }
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        strcpy(tokens[*token_count].value, "EOF");
        (*token_count)++;
    }
    
    return 1;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[MAX_INPUT_LENGTH];
    struct Token tokens[MAX_TOKENS];
    int token_count;
    
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
    
    if (!tokenize(input, tokens, &token_count)) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        printf("%d: %s [