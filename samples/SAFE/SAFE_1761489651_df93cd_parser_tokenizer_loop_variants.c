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
    while (isdigit(input[*pos])) {
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
    while (isalpha(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    if (*pos - start > 31) return 0;
    strncpy(token->value, &input[start], *pos - start);
    token->value[*pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return 1;
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    while (isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_END;
        return 1;
    }
    
    if (isdigit(input[*pos])) {
        return parse_number(input, pos, token);
    }
    
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        return parse_identifier(input, pos, token);
    }
    
    if (is_operator(input[*pos])) {
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        token->type = TOKEN_OPERATOR;
        (*pos)++;
        return 1;
    }
    
    return 0;
}

int main(void) {
    char input[256];
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    int pos = 0;
    struct Token tokens[64];
    int token_count = 0;
    
    while (token_count < 64) {
        struct Token token;
        if (!get_next_token(input, &pos, &token)) {
            fprintf(stderr, "Invalid token at position %d\n", pos);
            return 1;
        }
        
        if (token.type == TOKEN_END) break;
        
        tokens[token_count] = token;
        token_count++;
    }
    
    printf("Tokens:\n");
    for (int i = 0; i < token_count; i++) {
        const char *type_str;
        switch (tokens[i].type) {
            case TOKEN_NUMBER: type_str = "NUMBER"; break;
            case TOKEN_OPERATOR: type_str = "OPERATOR"; break;
            case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("%s: '%s'\n", type_str, tokens[i].value);
    }
    
    int i = 0;
    while (i < token_count) {
        if (tokens[i].type == TOKEN_IDENTIFIER) {
            int j = i + 1;
            while (j < token_count && tokens[j].type != TOKEN_OPERATOR) {
                j++;
            }
            if (j < token_count && tokens[j].value[0] == '=') {
                printf("Assignment found for variable '%s'\n", tokens[i].value);
                i = j;
            }
        }
        i++;
    }
    
    int has_numbers = 0;
    for (int k = 0; k < token_count; k++) {
        if (tokens[k].type == TOKEN_NUMBER) {
            has_numbers = 1;
            break;
        }
    }
    
    if (has_numbers) {
        printf("Expression contains numeric constants\n");
    }
    
    return 0;
}