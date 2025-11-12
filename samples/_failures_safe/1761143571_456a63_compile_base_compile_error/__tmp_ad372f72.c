//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_PAREN, TOKEN_END };

struct Token {
    enum TokenType type;
    char value[32];
};

int parse_expression(struct Token *tokens, int *pos, int *error);
int parse_term(struct Token *tokens, int *pos, int *error);
int parse_factor(struct Token *tokens, int *pos, int *error);

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int i = 0;
    int token_count = 0;
    
    while (input[i] != '\0' && token_count < max_tokens - 1) {
        while (isspace(input[i])) i++;
        
        if (input[i] == '\0') break;
        
        if (isdigit(input[i])) {
            int j = 0;
            while (isdigit(input[i]) && j < 31) {
                tokens[token_count].value[j++] = input[i++];
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
        } else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') {
            tokens[token_count].value[0] = input[i++];
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            token_count++;
        } else if (input[i] == '(' || input[i] == ')') {
            tokens[token_count].value[0] = input[i++];
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_PAREN;
            token_count++;
        } else {
            return -1;
        }
    }
    
    tokens[token_count].type = TOKEN_END;
    tokens[token_count].value[0] = '\0';
    return token_count;
}

int parse_factor(struct Token *tokens, int *pos, int *error) {
    if (*error) return 0;
    
    if (tokens[*pos].type == TOKEN_NUMBER) {
        int val = atoi(tokens[*pos].value);
        (*pos)++;
        return val;
    } else if (tokens[*pos].type == TOKEN_PAREN && tokens[*pos].value[0] == '(') {
        (*pos)++;
        int result = parse_expression(tokens, pos, error);
        if (*error) return 0;
        
        if (tokens[*pos].type != TOKEN_PAREN || tokens[*pos].value[0] != ')') {
            *error = 1;
            return 0;
        }
        (*pos)++;
        return result;
    } else {
        *error = 1;
        return 0;
    }
}

int parse_term(struct Token *tokens, int *pos, int *error) {
    if (*error) return 0;
    
    int left = parse_factor(tokens, pos, error);
    if (*error) return 0;
    
    while (tokens[*pos].type == TOKEN_OPERATOR && 
           (tokens[*pos].value[0] == '*' || tokens[*pos].value[0] == '/')) {
        char op = tokens[*pos].value[0];
        (*pos)++;
        
        int right = parse_factor(tokens, pos, error);
        if (*error) return 0;
        
        if (op == '*') {
            left = left * right;
        } else {
            if (right == 0) {
                *error = 1;
                return 0;
            }
            left = left / right;
        }
    }
    
    return left;
}

int parse_expression(struct Token *tokens, int *pos, int *error) {
    if (*error) return 0;
    
    int left = parse_term(tokens, pos, error);
    if (*error) return 0;
    
    while (tokens[*pos].type == TOKEN_OPERATOR && 
           (tokens[*pos].value[0] == '+' || tokens[*pos].value[0] == '-')) {
        char op = tokens[*pos].value[0];
        (*pos)++;
        
        int right = parse_term(tokens, pos, error);
        if (*error) return 0;
        
        if (op == '+') {
            left = left + right;
        } else {
            left = left - right;
        }
    }
    
    return left;
}

int main() {
    char input[256];
    struct Token tokens[100];
    
    printf("Enter arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, 100);
    if (token_count < 0) {
        printf("Invalid characters in input\n");
        return 1;
    }
    
    int pos = 0;
    int error = 0;
    int result = parse_expression(tokens, &pos, &error);
    
    if (error || tokens[pos].