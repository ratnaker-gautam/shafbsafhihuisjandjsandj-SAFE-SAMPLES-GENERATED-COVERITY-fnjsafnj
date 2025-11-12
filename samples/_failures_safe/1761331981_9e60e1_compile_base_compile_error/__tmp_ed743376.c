//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
} Token;

typedef struct {
    const char* input;
    int position;
    int line;
    int column;
    Token tokens[MAX_TOKENS];
    int token_count;
} Lexer;

void init_lexer(Lexer* lexer, const char* input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

void add_token(Lexer* lexer, TokenType type, const char* value) {
    if (lexer->token_count >= MAX_TOKENS) return;
    
    Token* token = &lexer->tokens[lexer->token_count];
    token->type = type;
    token->line = lexer->line;
    token->column = lexer->column;
    
    size_t len = strlen(value);
    if (len >= MAX_TOKEN_LEN) len = MAX_TOKEN_LEN - 1;
    strncpy(token->value, value, len);
    token->value[len] = '\0';
    
    lexer->token_count++;
}

char peek(Lexer* lexer) {
    return lexer->input[lexer->position];
}

char advance(Lexer* lexer) {
    char c = lexer->input[lexer->position];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    lexer->position++;
    return c;
}

void skip_whitespace(Lexer* lexer) {
    while (isspace((unsigned char)peek(lexer))) {
        advance(lexer);
    }
}

void read_string(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int pos = 0;
    advance(lexer);
    
    while (peek(lexer) != '"' && peek(lexer) != '\0') {
        if (pos < MAX_TOKEN_LEN - 1) {
            buffer[pos++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    buffer[pos] = '\0';
    
    if (peek(lexer) == '"') {
        advance(lexer);
        add_token(lexer, TOKEN_STRING, buffer);
    }
}

void read_number(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int pos = 0;
    
    while (isdigit((unsigned char)peek(lexer))) {
        if (pos < MAX_TOKEN_LEN - 1) {
            buffer[pos++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    buffer[pos] = '\0';
    add_token(lexer, TOKEN_NUMBER, buffer);
}

void read_identifier(Lexer* lexer) {
    char buffer[MAX_TOKEN_LEN];
    int pos = 0;
    
    while (isalnum((unsigned char)peek(lexer))) {
        if (pos < MAX_TOKEN_LEN - 1) {
            buffer[pos++] = advance(lexer);
        } else {
            advance(lexer);
        }
    }
    buffer[pos] = '\0';
    add_token(lexer, TOKEN_IDENTIFIER, buffer);
}

void read_operator(Lexer* lexer) {
    char buffer[3] = {0};
    buffer[0] = advance(lexer);
    
    if (is_operator_char(peek(lexer))) {
        buffer[1] = advance(lexer);
    }
    
    add_token(lexer, TOKEN_OPERATOR, buffer);
}

void tokenize(Lexer* lexer) {
    while (peek(lexer) != '\0') {
        skip_whitespace(lexer);
        
        char current = peek(lexer);
        if (current == '\0') break;
        
        if (isalpha((unsigned char)current)) {
            read_identifier(lexer);
        } else if (isdigit((unsigned char)current)) {
            read_number(lexer);
        } else if (current == '"') {
            read_string(lexer);
        } else if (is_operator_char(current)) {
            read_operator(lexer);
        } else {
            advance(lexer);
        }
    }
    add_token(lexer, TOKEN_EOF, "");
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens(Lexer*