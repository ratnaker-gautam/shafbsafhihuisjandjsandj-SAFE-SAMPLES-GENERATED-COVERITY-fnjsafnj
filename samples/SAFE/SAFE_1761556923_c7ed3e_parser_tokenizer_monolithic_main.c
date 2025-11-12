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

void skip_whitespace(const char *input, int *pos) {
    while (input[*pos] && isspace(input[*pos])) {
        (*pos)++;
    }
}

struct Token get_next_token(const char *input, int *pos, int line, int *column) {
    struct Token token = {TOKEN_INVALID, "", line, *column};
    int token_len = 0;
    
    skip_whitespace(input, pos);
    *column = *pos + 1;
    token.column = *column;
    
    if (!input[*pos]) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    char current = input[*pos];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        while (input[*pos] && isdigit(input[*pos]) && token_len < MAX_TOKEN_LEN - 1) {
            token.value[token_len++] = input[(*pos)++];
        }
        token.value[token_len] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_') && token_len < MAX_TOKEN_LEN - 1) {
            token.value[token_len++] = input[(*pos)++];
        }
        token.value[token_len] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[token_len++] = input[(*pos)++];
        if (input[*pos] && is_operator_char(input[*pos]) && token_len < MAX_TOKEN_LEN - 1) {
            token.value[token_len++] = input[(*pos)++];
        }
        token.value[token_len] = '\0';
    } else if (current == '(' || current == ')') {
        token.type = TOKEN_PAREN;
        token.value[token_len++] = input[(*pos)++];
        token.value[token_len] = '\0';
    } else {
        token.type = TOKEN_INVALID;
        token.value[token_len++] = input[(*pos)++];
        token.value[token_len] = '\0';
    }
    
    return token;
}

int tokenize(const char *input, struct Token tokens[], int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    while (input[pos] && token_count < max_tokens) {
        struct Token token = get_next_token(input, &pos, line, &column);
        
        if (token.type == TOKEN_EOF) {
            tokens[token_count++] = token;
            break;
        }
        
        if (token.type != TOKEN_INVALID) {
            tokens[token_count++] = token;
        } else {
            printf("Invalid token '%s' at line %d, column %d\n", token.value, token.line, token.column);
            return -1;
        }
        
        if (token_count >= max_tokens) {
            printf("Too many tokens, maximum is %d\n", max_tokens);
            return -1;
        }
    }
    
    return token_count;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "PAREN", "EOF", "INVALID"
    };
    
    printf("Token: %-12s Value: %-10s Line: %2d Column: %2d\n",
           type_names[token.type], token.value, token.line, token.column);
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    if (token_count <= 0) {
        return 1;
    }
    
    printf("\nTokenized output:\n");
    for (int i = 0; i < token_count; i++) {
        print_token(tokens[i]);
    }
    
    return 0;
}