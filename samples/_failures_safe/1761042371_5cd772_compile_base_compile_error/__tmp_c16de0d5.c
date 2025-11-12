//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
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
    int line;
    int column;
};

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
};

void init_lexer(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')';
}

struct Token get_next_token(struct Lexer *lexer) {
    struct Token token = {TOKEN_INVALID, "", lexer->line, lexer->column};
    
    if (lexer->input == NULL) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    while (lexer->input[lexer->position] != '\0') {
        char current = lexer->input[lexer->position];
        
        if (isspace(current)) {
            if (current == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }
        
        if (isdigit(current)) {
            token.type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(lexer->input[lexer->position]) && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (isalpha(current)) {
            token.type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (is_operator(current)) {
            token.type = TOKEN_OPERATOR;
            token.value[0] = current;
            token.value[1] = '\0';
            lexer->position++;
            lexer->column++;
            return token;
        }
        
        lexer->position++;
        lexer->column++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Token: %-12s Value: %-10s Line: %2d Column: %2d\n",
           type_names[token.type], token.value, token.line, token.column);
}

void parse_expression(struct Lexer *lexer, struct Token *current_token);
void parse_term(struct Lexer *lexer, struct Token *current_token);
void parse_factor(struct Lexer *lexer, struct Token *current_token);

void parse_factor(struct Lexer *lexer, struct Token *current_token) {
    if (current_token->type == TOKEN_NUMBER || current_token->type == TOKEN_IDENTIFIER) {
        printf("Factor: %s\n", current_token->value);
        *current_token = get_next_token(lexer);
    } else if (current_token->type == TOKEN_OPERATOR && current_token->value[0] == '(') {
        printf("Begin group\n");
        *current_token = get_next_token(lexer);
        parse_expression(lexer, current_token);
        if (current_token->type == TOKEN_OPERATOR && current_token->value[0] == ')') {
            printf("End group\n");
            *current_token = get_next_token(lexer);
        } else {
            printf("Error: Expected ')'\n");
        }
    } else {
        printf("Error: Expected factor\n");
    }
}

void parse_term(struct Lexer *lexer, struct Token *current_token) {
    parse_factor(lexer, current_token);
    
    while (current_token->type == TOKEN_OPERATOR && 
          (current_token->value[0] == '*' || current_token->value[0] == '/')) {
        char op = current_token->value[0];
        *current_token = get_next_token(lexer);
        parse_factor(lexer, current_token);
        printf("Operation: %c\n", op);
    }
}

void parse_expression(struct Lexer *lexer, struct Token *current_token) {
    parse_term(lexer, current_token);
    
    while (current_token->type == TOKEN_OPERATOR && 
          (current_token->value[0] == '+' || current_token->value[0] == '-')) {
        char op = current_token->value[0];
        *current_token = get_next_token(lexer);
        parse_term(lexer, current