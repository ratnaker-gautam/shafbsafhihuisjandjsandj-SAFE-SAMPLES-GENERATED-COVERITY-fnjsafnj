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

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token(%s, '%s')\n", type_names[token.type], token.value);
}

void parse_expression(struct Tokenizer *t);

void parse_primary(struct Tokenizer *t) {
    struct Token token = get_next_token(t);
    
    if (token.type == TOKEN_NUMBER) {
        printf("Found number: %s\n", token.value);
    } else if (token.type == TOKEN_IDENTIFIER) {
        printf("Found identifier: %s\n", token.value);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("Entering subexpression\n");
        parse_expression(t);
        struct Token close_paren = get_next_token(t);
        if (close_paren.type != TOKEN_OPERATOR || close_paren.value[0] != ')') {
            printf("Error: Expected closing parenthesis\n");
        } else {
            printf("Exiting subexpression\n");
        }
    } else {
        printf("Error: Expected primary expression\n");
    }
}

void parse_expression(struct Tokenizer *t) {
    parse_primary(t);
    
    struct Token op = get_next_token(t);
    
    if (op.type == TOKEN_OPERATOR && (op.value[0] == '+' || op.value[0] == '-' || 
                                      op.value[0] == '*' || op.value[0] == '/')) {
        printf("Found operator: %s\n", op.value);
        parse_expression(t);
    } else {
        t->pos -= strlen(op.value);
    }
}

int main(void) {
    char input[256];
    
    printf("Enter expression to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    struct Tokenizer tokenizer;
    init_tokenizer(&tokenizer, input);
    
    printf("Parsing expression: %s\n", input);
    parse_expression(&tokenizer);
    
    struct Token final_token = get_next_token(&tokenizer);
    if (final_token.type != TOKEN_EOF) {
        printf("Unexpected token at end: ");
        print_token(final_token);
    } else {
        printf("Parsing completed successfully\n");
    }
    
    return 0;
}