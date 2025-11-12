//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    TOKEN_PAREN,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char *input, int *pos, int *line, int *col) {
    while (input[*pos] != '\0') {
        if (input[*pos] == ' ') {
            (*pos)++;
            (*col)++;
        } else if (input[*pos] == '\t') {
            (*pos)++;
            (*col) += 4;
        } else if (input[*pos] == '\n') {
            (*pos)++;
            (*line)++;
            *col = 1;
        } else {
            break;
        }
    }
}

struct Token get_next_token(const char *input, int *pos, int *line, int *col) {
    struct Token token = {TOKEN_INVALID, "", *line, *col};
    int token_len = 0;
    
    skip_whitespace(input, pos, line, col);
    token.line = *line;
    token.column = *col;
    
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = input[*pos];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        while (isdigit(input[*pos]) && token_len < MAX_TOKEN_LEN - 1) {
            token.value[token_len++] = input[(*pos)++];
            (*col)++;
        }
        token.value[token_len] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        while ((isalnum(input[*pos]) || input[*pos] == '_') && token_len < MAX_TOKEN_LEN - 1) {
            token.value[token_len++] = input[(*pos)++];
            (*col)++;
        }
        token.value[token_len] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[token_len++] = input[(*pos)++];
        (*col)++;
        if (is_operator_char(input[*pos]) && token_len < MAX_TOKEN_LEN - 1) {
            token.value[token_len++] = input[(*pos)++];
            (*col)++;
        }
        token.value[token_len] = '\0';
    } else if (current == '(' || current == ')') {
        token.type = TOKEN_PAREN;
        token.value[token_len++] = input[(*pos)++];
        (*col)++;
        token.value[token_len] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[token_len++] = input[(*pos)++];
        (*col)++;
        token.value[token_len] = '\0';
    }
    
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PAREN: return "PAREN";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int col = 1;
    int token_count = 0;
    
    while (token_count < max_tokens) {
        struct Token token = get_next_token(input, &pos, &line, &col);
        
        if (token.type == TOKEN_EOF) {
            tokens[token_count++] = token;
            break;
        }
        
        if (token.type == TOKEN_INVALID) {
            tokens[token_count++] = token;
            break;
        }
        
        tokens[token_count++] = token;
    }
    
    return token_count;
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
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("\nTokens found:\n");
    printf("%-12s %-15s %-6s %-6s\n", "