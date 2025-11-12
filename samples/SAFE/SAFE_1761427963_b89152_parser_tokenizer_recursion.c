//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
};

struct Tokenizer {
    const char *input;
    size_t pos;
    size_t len;
};

void init_tokenizer(struct Tokenizer *t, const char *str) {
    t->input = str;
    t->pos = 0;
    t->len = strlen(str);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

struct Token get_next_token(struct Tokenizer *t) {
    struct Token token = {TOKEN_INVALID, ""};
    
    if (t->pos >= t->len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (t->pos < t->len && isspace(t->input[t->pos])) {
        t->pos++;
    }
    
    if (t->pos >= t->len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = t->input[t->pos];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (t->pos < t->len && i < 31 && isdigit(t->input[t->pos])) {
            token.value[i++] = t->input[t->pos++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (t->pos < t->len && i < 31 && (isalnum(t->input[t->pos]) || t->input[t->pos] == '_')) {
            token.value[i++] = t->input[t->pos++];
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = current;
        token.value[1] = '\0';
        t->pos++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = current;
        token.value[1] = '\0';
        t->pos++;
    }
    
    return token;
}

void parse_expression(struct Tokenizer *t);

void parse_factor(struct Tokenizer *t) {
    struct Token token = get_next_token(t);
    
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        printf("Factor: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '-') {
        printf("Unary minus\n");
        parse_factor(t);
    } else {
        printf("Unexpected token in factor: %s\n", token.value);
    }
}

void parse_term(struct Tokenizer *t) {
    parse_factor(t);
    
    struct Token token = get_next_token(t);
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        printf("Operator: %s\n", token.value);
        parse_term(t);
    } else {
        t->pos -= strlen(token.value);
    }
}

void parse_expression(struct Tokenizer *t) {
    parse_term(t);
    
    struct Token token = get_next_token(t);
    
    if (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        printf("Operator: %s\n", token.value);
        parse_expression(t);
    } else {
        t->pos -= strlen(token.value);
    }
}

int main() {
    char input[256];
    
    printf("Enter an arithmetic expression: ");
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
    
    struct Tokenizer tokenizer;
    init_tokenizer(&tokenizer, input);
    
    printf("Parsing expression: %s\n", input);
    parse_expression(&tokenizer);
    
    struct Token final_token = get_next_token(&tokenizer);
    if (final_token.type != TOKEN_EOF) {
        printf("Unexpected token at end: %s\n", final_token.value);
    } else {
        printf("Parse completed successfully\n");
    }
    
    return 0;
}