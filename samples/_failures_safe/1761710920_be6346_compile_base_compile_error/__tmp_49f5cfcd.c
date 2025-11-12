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
    size_t position;
    size_t length;
};

void init_tokenizer(struct Tokenizer *tokenizer, const char *input) {
    tokenizer->input = input;
    tokenizer->position = 0;
    tokenizer->length = strlen(input);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

struct Token get_next_token(struct Tokenizer *tokenizer) {
    struct Token token = {TOKEN_INVALID, ""};
    
    if (tokenizer->position >= tokenizer->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    while (tokenizer->position < tokenizer->length && 
           isspace(tokenizer->input[tokenizer->position])) {
        tokenizer->position++;
    }
    
    if (tokenizer->position >= tokenizer->length) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char current = tokenizer->input[tokenizer->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        size_t i = 0;
        while (tokenizer->position < tokenizer->length && 
               i < sizeof(token.value) - 1 &&
               isdigit(tokenizer->input[tokenizer->position])) {
            token.value[i++] = tokenizer->input[tokenizer->position++];
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        size_t i = 0;
        while (tokenizer->position < tokenizer->length && 
               i < sizeof(token.value) - 1 &&
               (isalnum(tokenizer->input[tokenizer->position]) || 
                tokenizer->input[tokenizer->position] == '_')) {
            token.value[i++] = tokenizer->input[tokenizer->position++];
        }
        token.value[i] = '\0';
    } else if (is_operator(current)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = current;
        token.value[1] = '\0';
        tokenizer->position++;
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = current;
        token.value[1] = '\0';
        tokenizer->position++;
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token(%s, '%s')\n", type_names[token.type], token.value);
}

void parse_expression(struct Tokenizer *tokenizer, struct Token *current_token);

void parse_factor(struct Tokenizer *tokenizer, struct Token *current_token) {
    if (current_token->type == TOKEN_NUMBER || current_token->type == TOKEN_IDENTIFIER) {
        print_token(*current_token);
        *current_token = get_next_token(tokenizer);
    } else if (current_token->type == TOKEN_OPERATOR && current_token->value[0] == '(') {
        *current_token = get_next_token(tokenizer);
        parse_expression(tokenizer, current_token);
        if (current_token->type == TOKEN_OPERATOR && current_token->value[0] == ')') {
            *current_token = get_next_token(tokenizer);
        } else {
            printf("Error: Expected ')'\n");
        }
    } else {
        printf("Error: Expected number, identifier, or '('\n");
    }
}

void parse_term(struct Tokenizer *tokenizer, struct Token *current_token) {
    parse_factor(tokenizer, current_token);
    
    while (current_token->type == TOKEN_OPERATOR && 
           (current_token->value[0] == '*' || current_token->value[0] == '/')) {
        print_token(*current_token);
        *current_token = get_next_token(tokenizer);
        parse_factor(tokenizer, current_token);
    }
}

void parse_expression(struct Tokenizer *tokenizer, struct Token *current_token) {
    parse_term(tokenizer, current_token);
    
    while (current_token->type == TOKEN_OPERATOR && 
           (current_token->value[0] == '+' || current_token->value[0] == '-')) {
        print_token(*current_token);
        *current_token = get_next_token(tokenizer);
        parse_term(tokenizer, current_token);
    }
}

void parse_assignment(struct Tokenizer *tokenizer, struct Token *current_token) {
    if (current_token->type == TOKEN_IDENTIFIER) {
        print_token(*current_token);
        *current_token = get_next_token(tokenizer);
        
        if (current_token->type == TOKEN_OPERATOR && current_token->value[0] == '=') {
            print_token(*current_token);
            *current_token = get_next_token(tokenizer);
            parse_expression(tokenizer, current_token);
        } else {
            printf("Error: Expected '=' after identifier\n");
        }
    } else {