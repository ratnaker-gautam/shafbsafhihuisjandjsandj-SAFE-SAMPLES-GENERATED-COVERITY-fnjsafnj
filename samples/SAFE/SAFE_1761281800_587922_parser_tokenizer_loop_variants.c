//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_IDENTIFIER, TOKEN_END };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    while (input[*pos] && isdigit(input[*pos])) {
        (*pos)++;
    }
    if (*pos - start > 31) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return 1;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
    }
    if (*pos - start > 31) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    while (input[*pos] && isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (!input[*pos]) {
        token->type = TOKEN_END;
        return 1;
    }
    
    if (isdigit(input[*pos])) {
        return parse_number(input, pos, token);
    }
    
    if (is_operator(input[*pos])) {
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        (*pos)++;
        return 1;
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        return parse_identifier(input, pos, token);
    }
    
    return 0;
}

void process_tokens(const char *input) {
    struct Token token;
    int pos = 0;
    int token_count = 0;
    
    while (1) {
        if (!get_next_token(input, &pos, &token)) {
            printf("Error: Invalid token at position %d\n", pos);
            return;
        }
        
        if (token.type == TOKEN_END) break;
        
        const char *type_str = "";
        switch (token.type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            default: break;
        }
        
        printf("Token %d: %s ('%s')\n", token_count, type_str, token.value);
        token_count++;
        
        if (token_count > 100) {
            printf("Error: Too many tokens\n");
            return;
        }
    }
    
    printf("Total tokens: %d\n", token_count);
}

int main() {
    char input[1024];
    
    printf("Enter expression: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1) {
        printf("Error: Empty input\n");
        return 1;
    }
    
    process_tokens(input);
    return 0;
}