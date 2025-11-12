//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_PAREN, TOKEN_END };

struct Token {
    enum TokenType type;
    char value[32];
};

int parse_expression(struct Token *tokens, int *pos, int depth);
int parse_term(struct Token *tokens, int *pos, int depth);
int parse_factor(struct Token *tokens, int *pos, int depth);

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int count = 0;
    const char *p = input;
    
    while (*p && count < max_tokens - 1) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        if (isdigit(*p)) {
            int i = 0;
            while (isdigit(*p) && i < 31) {
                tokens[count].value[i++] = *p++;
            }
            tokens[count].value[i] = '\0';
            tokens[count].type = TOKEN_NUMBER;
            count++;
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
            tokens[count].value[0] = *p++;
            tokens[count].value[1] = '\0';
            tokens[count].type = TOKEN_OPERATOR;
            count++;
        } else if (*p == '(' || *p == ')') {
            tokens[count].value[0] = *p++;
            tokens[count].value[1] = '\0';
            tokens[count].type = TOKEN_PAREN;
            count++;
        } else {
            return -1;
        }
    }
    
    tokens[count].type = TOKEN_END;
    return count;
}

int parse_factor(struct Token *tokens, int *pos, int depth) {
    if (depth > 100) return -1;
    
    if (tokens[*pos].type == TOKEN_NUMBER) {
        int val = atoi(tokens[*pos].value);
        (*pos)++;
        return val;
    } else if (tokens[*pos].type == TOKEN_PAREN && tokens[*pos].value[0] == '(') {
        (*pos)++;
        int result = parse_expression(tokens, pos, depth + 1);
        if (tokens[*pos].type != TOKEN_PAREN || tokens[*pos].value[0] != ')') {
            return -1;
        }
        (*pos)++;
        return result;
    }
    return -1;
}

int parse_term(struct Token *tokens, int *pos, int depth) {
    if (depth > 100) return -1;
    
    int left = parse_factor(tokens, pos, depth + 1);
    if (left == -1) return -1;
    
    while (tokens[*pos].type == TOKEN_OPERATOR && 
           (tokens[*pos].value[0] == '*' || tokens[*pos].value[0] == '/')) {
        char op = tokens[*pos].value[0];
        (*pos)++;
        
        int right = parse_factor(tokens, pos, depth + 1);
        if (right == -1) return -1;
        
        if (op == '*') {
            left *= right;
        } else {
            if (right == 0) return -1;
            left /= right;
        }
    }
    
    return left;
}

int parse_expression(struct Token *tokens, int *pos, int depth) {
    if (depth > 100) return -1;
    
    int left = parse_term(tokens, pos, depth + 1);
    if (left == -1) return -1;
    
    while (tokens[*pos].type == TOKEN_OPERATOR && 
           (tokens[*pos].value[0] == '+' || tokens[*pos].value[0] == '-')) {
        char op = tokens[*pos].value[0];
        (*pos)++;
        
        int right = parse_term(tokens, pos, depth + 1);
        if (right == -1) return -1;
        
        if (op == '+') {
            left += right;
        } else {
            left -= right;
        }
    }
    
    return left;
}

int main() {
    char input[256];
    printf("Enter arithmetic expression: ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    struct Token tokens[100];
    int token_count = tokenize(input, tokens, 100);
    
    if (token_count == -1) {
        printf("Invalid tokens in expression\n");
        return 1;
    }
    
    int pos = 0;
    int result = parse_expression(tokens, &pos, 0);
    
    if (result == -1 || tokens[pos].type != TOKEN_END) {
        printf("Invalid expression syntax\n");
        return 1;
    }
    
    printf("Result: %d\n", result);
    return 0;
}