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
    int has_dot = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            (*pos)++;
        } else if (input[*pos] == '.' && !has_dot) {
            has_dot = 1;
            (*pos)++;
        } else {
            break;
        }
    }
    
    int len = *pos - start;
    if (len > 0 && len < 32) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_NUMBER;
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            (*pos)++;
        }
    }
    
    int len = *pos - start;
    if (len > 0 && len < 32) {
        strncpy(token->value, &input[start], len);
        token->value[len] = '\0';
        token->type = TOKEN_IDENTIFIER;
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (is_operator(input[*pos])) {
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        (*pos)++;
        return 1;
    }
    return 0;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    while (isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_END;
        return 1;
    }
    
    if (parse_number(input, pos, token)) return 1;
    if (parse_identifier(input, pos, token)) return 1;
    if (parse_operator(input, pos, token)) return 1;
    
    return 0;
}

void process_tokens(const char *input) {
    struct Token token;
    int pos = 0;
    int token_count = 0;
    
    while (1) {
        if (!get_next_token(input, &pos, &token)) {
            printf("Error: Invalid token at position %d\n", pos);
            break;
        }
        
        if (token.type == TOKEN_END) {
            break;
        }
        
        const char *type_str = "";
        switch (token.type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            default: type_str = "UNKNOWN"; break;
        }
        
        printf("Token %d: %s -> '%s'\n", token_count, type_str, token.value);
        token_count++;
        
        if (token_count >= 100) {
            printf("Error: Too many tokens\n");
            break;
        }
    }
    
    printf("Total tokens processed: %d\n", token_count);
}

int main(void) {
    char input[256];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Error: Empty input\n");
        return 1;
    }
    
    if (strlen(input) >= 255) {
        printf("Error: Input too long\n");
        return 1;
    }
    
    process_tokens(input);
    return 0;
}