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

int parse_expression(struct Token *tokens, int *pos, int depth);
int parse_term(struct Token *tokens, int *pos, int depth);
int parse_factor(struct Token *tokens, int *pos, int depth);

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int i = 0, token_count = 0;
    while (input[i] != '\0' && token_count < max_tokens) {
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
    return token_count;
}

int parse_factor(struct Token *tokens, int *pos, int depth) {
    if (depth > 100) return 0;
    if (tokens[*pos].type == TOKEN_NUMBER) {
        int val = atoi(tokens[*pos].value);
        (*pos)++;
        return val;
    } else if (tokens[*pos].type == TOKEN_PAREN && tokens[*pos].value[0] == '(') {
        (*pos)++;
        int result = parse_expression(tokens, pos, depth + 1);
        if (tokens[*pos].type == TOKEN_PAREN && tokens[*pos].value[0] == ')') {
            (*pos)++;
            return result;
        }
        return 0;
    }
    return 0;
}

int parse_term(struct Token *tokens, int *pos, int depth) {
    if (depth > 100) return 0;
    int left = parse_factor(tokens, pos, depth + 1);
    while (tokens[*pos].type == TOKEN_OPERATOR && 
           (tokens[*pos].value[0] == '*' || tokens[*pos].value[0] == '/')) {
        char op = tokens[*pos].value[0];
        (*pos)++;
        int right = parse_factor(tokens, pos, depth + 1);
        if (op == '*') {
            left *= right;
        } else if (right != 0) {
            left /= right;
        } else {
            return 0;
        }
    }
    return left;
}

int parse_expression(struct Token *tokens, int *pos, int depth) {
    if (depth > 100) return 0;
    int left = parse_term(tokens, pos, depth + 1);
    while (tokens[*pos].type == TOKEN_OPERATOR && 
           (tokens[*pos].value[0] == '+' || tokens[*pos].value[0] == '-')) {
        char op = tokens[*pos].value[0];
        (*pos)++;
        int right = parse_term(tokens, pos, depth + 1);
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
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';

    struct Token tokens[100];
    int token_count = tokenize(input, tokens, 100);
    if (token_count < 0) {
        printf("Invalid expression\n");
        return 1;
    }

    int pos = 0;
    int result = parse_expression(tokens, &pos, 0);
    if (tokens[pos].type != TOKEN_END) {
        printf("Invalid expression\n");
        return 1;
    }
    printf("Result: %d\n", result);
    return 0;
}