//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_PAREN, TOKEN_END, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[32];
};

int parse_expression(const char* input, int* pos, struct Token* tokens, int token_count);
int parse_term(const char* input, int* pos, struct Token* tokens, int token_count);
int parse_factor(const char* input, int* pos, struct Token* tokens, int token_count);

int get_next_token(const char* input, int* pos, struct Token* token) {
    while (isspace(input[*pos])) (*pos)++;
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_END;
        return 1;
    }
    
    if (isdigit(input[*pos])) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(input[*pos]) && i < 31) {
            token->value[i++] = input[*pos];
            (*pos)++;
        }
        token->value[i] = '\0';
        return 1;
    }
    
    if (strchr("+-*/", input[*pos]) != NULL) {
        token->type = TOKEN_OPERATOR;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
        return 1;
    }
    
    if (input[*pos] == '(' || input[*pos] == ')') {
        token->type = TOKEN_PAREN;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
        return 1;
    }
    
    token->type = TOKEN_ERROR;
    return 0;
}

int parse_factor(const char* input, int* pos, struct Token* tokens, int token_count) {
    struct Token token;
    if (!get_next_token(input, pos, &token)) return 0;
    
    if (token.type == TOKEN_NUMBER) {
        printf("Number: %s\n", token.value);
        return 1;
    }
    
    if (token.type == TOKEN_PAREN && token.value[0] == '(') {
        if (!parse_expression(input, pos, tokens, token_count)) return 0;
        if (!get_next_token(input, pos, &token)) return 0;
        if (token.type != TOKEN_PAREN || token.value[0] != ')') return 0;
        return 1;
    }
    
    return 0;
}

int parse_term(const char* input, int* pos, struct Token* tokens, int token_count) {
    if (!parse_factor(input, pos, tokens, token_count)) return 0;
    
    struct Token token;
    if (!get_next_token(input, pos, &token)) return 1;
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        printf("Operator: %s\n", token.value);
        if (!parse_factor(input, pos, tokens, token_count)) return 0;
        return 1;
    }
    
    (*pos) -= strlen(token.value);
    return 1;
}

int parse_expression(const char* input, int* pos, struct Token* tokens, int token_count) {
    if (!parse_term(input, pos, tokens, token_count)) return 0;
    
    struct Token token;
    if (!get_next_token(input, pos, &token)) return 1;
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        printf("Operator: %s\n", token.value);
        if (!parse_term(input, pos, tokens, token_count)) return 0;
        return 1;
    }
    
    (*pos) -= strlen(token.value);
    return 1;
}

int main() {
    char input[256];
    printf("Enter arithmetic expression: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    struct Token tokens[100];
    
    printf("Parsing expression: %s\n", input);
    
    if (!parse_expression(input, &pos, tokens, 100)) {
        fprintf(stderr, "Parse error at position %d\n", pos);
        return 1;
    }
    
    struct Token final_token;
    if (get_next_token(input, &pos, &final_token) && final_token.type != TOKEN_END) {
        fprintf(stderr, "Unexpected token at end\n");
        return 1;
    }
    
    printf("Expression parsed successfully\n");
    return 0;
}