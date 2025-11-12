//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_PAREN, TOKEN_END, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    int value;
    char symbol;
};

struct Tokenizer {
    const char *input;
    int position;
};

void init_tokenizer(struct Tokenizer *t, const char *str) {
    t->input = str;
    t->position = 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

struct Token get_next_token(struct Tokenizer *t) {
    struct Token token;
    token.type = TOKEN_ERROR;
    
    while (t->input[t->position] && isspace(t->input[t->position])) {
        t->position++;
    }
    
    if (t->input[t->position] == '\0') {
        token.type = TOKEN_END;
        return token;
    }
    
    char current = t->input[t->position];
    
    if (isdigit(current)) {
        int value = 0;
        while (t->input[t->position] && isdigit(t->input[t->position])) {
            value = value * 10 + (t->input[t->position] - '0');
            t->position++;
        }
        token.type = TOKEN_NUMBER;
        token.value = value;
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        token.symbol = current;
        t->position++;
    } else if (current == '(' || current == ')') {
        token.type = TOKEN_PAREN;
        token.symbol = current;
        t->position++;
    } else {
        token.type = TOKEN_ERROR;
    }
    
    return token;
}

struct Token peek_token(struct Tokenizer *t) {
    int saved_pos = t->position;
    struct Token token = get_next_token(t);
    t->position = saved_pos;
    return token;
}

int parse_expression(struct Tokenizer *t);
int parse_term(struct Tokenizer *t);
int parse_factor(struct Tokenizer *t);

int parse_factor(struct Tokenizer *t) {
    struct Token token = get_next_token(t);
    
    if (token.type == TOKEN_NUMBER) {
        return token.value;
    } else if (token.type == TOKEN_PAREN && token.symbol == '(') {
        int result = parse_expression(t);
        token = get_next_token(t);
        if (token.type != TOKEN_PAREN || token.symbol != ')') {
            return 0;
        }
        return result;
    }
    return 0;
}

int parse_term(struct Tokenizer *t) {
    int result = parse_factor(t);
    
    while (1) {
        struct Token token = peek_token(t);
        if (token.type != TOKEN_OPERATOR || (token.symbol != '*' && token.symbol != '/')) {
            break;
        }
        get_next_token(t);
        int right = parse_factor(t);
        
        if (token.symbol == '*') {
            result *= right;
        } else if (token.symbol == '/') {
            if (right != 0) {
                result /= right;
            } else {
                result = 0;
            }
        }
    }
    
    return result;
}

int parse_expression(struct Tokenizer *t) {
    int result = parse_term(t);
    
    while (1) {
        struct Token token = peek_token(t);
        if (token.type != TOKEN_OPERATOR || (token.symbol != '+' && token.symbol != '-')) {
            break;
        }
        get_next_token(t);
        int right = parse_term(t);
        
        if (token.symbol == '+') {
            result += right;
        } else if (token.symbol == '-') {
            result -= right;
        }
    }
    
    return result;
}

int evaluate_expression(const char *expr) {
    struct Tokenizer tokenizer;
    init_tokenizer(&tokenizer, expr);
    return parse_expression(&tokenizer);
}

int main() {
    char input[256];
    
    printf("Enter arithmetic expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty expression\n");
        return 1;
    }
    
    int result = evaluate_expression(input);
    printf("Result: %d\n", result);
    
    return 0;
}