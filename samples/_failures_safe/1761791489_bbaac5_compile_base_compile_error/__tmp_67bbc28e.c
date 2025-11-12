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
    int token_pos = 0;
    
    skip_whitespace(input, pos);
    *column = *pos + 1;
    token.column = *column;
    
    if (!input[*pos]) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = input[*pos];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        while (input[*pos] && isdigit(input[*pos])) {
            if (token_pos < MAX_TOKEN_LEN - 1) {
                token.value[token_pos++] = input[*pos];
            }
            (*pos)++;
        }
        token.value[token_pos] = '\0';
        return token;
    }
    
    if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (token_pos < MAX_TOKEN_LEN - 1) {
                token.value[token_pos++] = input[*pos];
            }
            (*pos)++;
        }
        token.value[token_pos] = '\0';
        return token;
    }
    
    if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        if (token_pos < MAX_TOKEN_LEN - 1) {
            token.value[token_pos++] = input[*pos];
        }
        (*pos)++;
        
        if (input[*pos] && is_operator_char(input[*pos])) {
            if (token_pos < MAX_TOKEN_LEN - 1) {
                token.value[token_pos++] = input[*pos];
            }
            (*pos)++;
        }
        token.value[token_pos] = '\0';
        return token;
    }
    
    if (current == '(' || current == ')') {
        token.type = TOKEN_PAREN;
        if (token_pos < MAX_TOKEN_LEN - 1) {
            token.value[token_pos++] = input[*pos];
        }
        (*pos)++;
        token.value[token_pos] = '\0';
        return token;
    }
    
    token.type = TOKEN_INVALID;
    if (token_pos < MAX_TOKEN_LEN - 1) {
        token.value[token_pos++] = input[*pos];
    }
    (*pos)++;
    token.value[token_pos] = '\0';
    return token;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
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
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        strcpy(tokens[token_count].value, "");
        tokens[token_count].line = line;
        tokens[token_count].column = column;
    }
    
    return token_count;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "PAREN", "EOF", "INVALID"
    };
    
    printf("Token: %-12s Value: '%-10s' Line: %2d Column: %2d\n",
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