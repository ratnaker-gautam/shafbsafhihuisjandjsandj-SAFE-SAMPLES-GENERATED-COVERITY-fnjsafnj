//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

struct Tokenizer {
    struct Token tokens[MAX_TOKENS];
    int token_count;
    int current_line;
    int current_column;
};

struct Parser {
    struct Tokenizer* tokenizer;
    int current_token;
};

const char* keywords[] = {"if", "else", "while", "for", "return", NULL};

int is_keyword(const char* str) {
    if (str == NULL) return 0;
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void init_tokenizer(struct Tokenizer* tokenizer) {
    if (tokenizer == NULL) return;
    tokenizer->token_count = 0;
    tokenizer->current_line = 1;
    tokenizer->current_column = 1;
}

void init_parser(struct Parser* parser, struct Tokenizer* tokenizer) {
    if (parser == NULL || tokenizer == NULL) return;
    parser->tokenizer = tokenizer;
    parser->current_token = 0;
}

int add_token(struct Tokenizer* tokenizer, enum TokenType type, const char* value) {
    if (tokenizer == NULL || value == NULL) return 0;
    if (tokenizer->token_count >= MAX_TOKENS) {
        return 0;
    }
    if (strlen(value) >= MAX_TOKEN_LEN) {
        return 0;
    }
    
    struct Token* token = &tokenizer->tokens[tokenizer->token_count];
    token->type = type;
    strncpy(token->value, value, MAX_TOKEN_LEN - 1);
    token->value[MAX_TOKEN_LEN - 1] = '\0';
    token->line = tokenizer->current_line;
    token->column = tokenizer->current_column;
    
    tokenizer->token_count++;
    return 1;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

void tokenize(struct Tokenizer* tokenizer, const char* input) {
    if (tokenizer == NULL || input == NULL) return;
    
    int i = 0;
    int len = strlen(input);
    
    while (i < len && tokenizer->token_count < MAX_TOKENS) {
        if (isspace((unsigned char)input[i])) {
            if (input[i] == '\n') {
                tokenizer->current_line++;
                tokenizer->current_column = 1;
            } else {
                tokenizer->current_column++;
            }
            i++;
            continue;
        }
        
        if (isalpha((unsigned char)input[i]) || input[i] == '_') {
            char buffer[MAX_TOKEN_LEN] = {0};
            int j = 0;
            
            while ((i < len) && (isalnum((unsigned char)input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                buffer[j++] = input[i++];
                tokenizer->current_column++;
            }
            buffer[j] = '\0';
            
            enum TokenType type = is_keyword(buffer) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            add_token(tokenizer, type, buffer);
            continue;
        }
        
        if (isdigit((unsigned char)input[i])) {
            char buffer[MAX_TOKEN_LEN] = {0};
            int j = 0;
            
            while ((i < len) && isdigit((unsigned char)input[i]) && j < MAX_TOKEN_LEN - 1) {
                buffer[j++] = input[i++];
                tokenizer->current_column++;
            }
            buffer[j] = '\0';
            
            add_token(tokenizer, TOKEN_NUMBER, buffer);
            continue;
        }
        
        if (is_operator(input[i])) {
            char op[2] = {input[i], '\0'};
            add_token(tokenizer, TOKEN_OPERATOR, op);
            i++;
            tokenizer->current_column++;
            continue;
        }
        
        if (is_delimiter(input[i])) {
            char delim[2] = {input[i], '\0'};
            add_token(tokenizer, TOKEN_DELIMITER, delim);
            i++;
            tokenizer->current_column++;
            continue;
        }
        
        char unknown[2] = {input[i], '\0'};
        add_token(tokenizer, TOKEN_UNKNOWN, unknown);
        i++;
        tokenizer->current_column++;
    }
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: printf("IDENTIFIER"); break;
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_OPERATOR: