//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[32];
    int line;
    int column;
} Token;

typedef struct {
    const char* input;
    int position;
    int line;
    int column;
} Lexer;

typedef void (*TokenHandler)(const Token*);

void handle_number(const Token* token) {
    printf("Number: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_identifier(const Token* token) {
    printf("Identifier: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_operator(const Token* token) {
    printf("Operator: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void handle_string(const Token* token) {
    printf("String: %s at line %d, column %d\n", token->value, token->line, token->column);
}

void skip_whitespace(Lexer* lexer) {
    while (lexer->input[lexer->position] != '\0') {
        char c = lexer->input[lexer->position];
        if (c == ' ' || c == '\t') {
            lexer->position++;
            lexer->column++;
        } else if (c == '\n') {
            lexer->position++;
            lexer->line++;
            lexer->column = 1;
        } else {
            break;
        }
    }
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

Token get_next_token(Lexer* lexer) {
    Token token = {TOKEN_EOF, "", lexer->line, lexer->column};
    
    skip_whitespace(lexer);
    
    if (lexer->input[lexer->position] == '\0') {
        return token;
    }
    
    char current = lexer->input[lexer->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(lexer->input[lexer->position]) && i < 31) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(lexer->input[lexer->position]) || lexer->input[lexer->position] == '_') && i < 31) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(lexer->input[lexer->position]) && i < 31) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        lexer->position++;
        lexer->column++;
        while (lexer->input[lexer->position] != '"' && lexer->input[lexer->position] != '\0' && i < 31) {
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
        }
        token.value[i] = '\0';
        if (lexer->input[lexer->position] == '"') {
            lexer->position++;
            lexer->column++;
        }
    } else {
        lexer->position++;
        lexer->column++;
        return get_next_token(lexer);
    }
    
    return token;
}

void process_tokens(const char* input, TokenHandler handlers[4]) {
    if (!input || !handlers) {
        return;
    }
    
    Lexer lexer = {input, 0, 1, 1};
    
    while (1) {
        Token token = get_next_token(&lexer);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        if (token.type >= TOKEN_NUMBER && token.type <= TOKEN_STRING) {
            handlers[token.type](&token);
        }
    }
}

int main(void) {
    const char* test_input = "x = 42 + y * 3 \"hello world\" abc_123 <=";
    
    TokenHandler handlers[4] = {
        handle_number,
        handle_identifier,
        handle_operator,
        handle_string
    };
    
    printf("Input: %s\n", test_input);
    printf("Tokens:\n");
    process_tokens(test_input, handlers);
    
    return 0;
}