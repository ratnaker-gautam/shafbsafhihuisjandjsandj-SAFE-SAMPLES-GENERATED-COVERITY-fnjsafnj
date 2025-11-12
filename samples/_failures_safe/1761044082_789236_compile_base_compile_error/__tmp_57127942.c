//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (token_pos < MAX_TOKEN_LEN - 1) {
                token.value[token_pos++] = input[*pos];
            }
            (*pos)++;
        }
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        if (token_pos < MAX_TOKEN_LEN - 1) {
            token.value[token_pos++] = input[*pos];
        }
        (*pos)++;
    } else if (current == '(' || current == ')') {
        token.type = TOKEN_PAREN;
        if (token_pos < MAX_TOKEN_LEN - 1) {
            token.value[token_pos++] = input[*pos];
        }
        (*pos)++;
    } else {
        if (token_pos < MAX_TOKEN_LEN - 1) {
            token.value[token_pos++] = input[*pos];
        }
        (*pos)++;
    }
    
    token.value[token_pos] = '\0';
    return token;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PAREN: return "PAREN";
        case TOKEN_EOF: return "EOF";
        default: return "INVALID";
    }
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
            break;
        }
        
        if (token_count >= max_tokens) {
            break;
        }
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
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count == 0) {
        fprintf(stderr, "No tokens found\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    printf("%-12s %-16s %-6s %-6s\n", "TYPE", "VALUE", "LINE", "COL");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-12s %-16s %-6d %-6d\n",
               token_type_to_string(tokens[i].type),
               tokens[i].value,
               tokens[i].line,
               tokens[i].column);
    }
    
    return