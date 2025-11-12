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

void tokenizer_init(struct Tokenizer* tokenizer) {
    if (tokenizer == NULL) return;
    tokenizer->token_count = 0;
    tokenizer->current_line = 1;
    tokenizer->current_column = 1;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_delimiter(char c) {
    return strchr("(),;{}", c) != NULL;
}

int is_keyword(const char* str) {
    if (str == NULL) return 0;
    const char* keywords[] = {"if", "else", "while", "for", "return", "int", "float"};
    int num_keywords = 7;
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int tokenize(struct Tokenizer* tokenizer, const char* input) {
    if (tokenizer == NULL || input == NULL) return 0;
    
    int len = strlen(input);
    int i = 0;
    
    while (i < len && tokenizer->token_count < MAX_TOKENS) {
        while (i < len && isspace(input[i])) {
            if (input[i] == '\n') {
                tokenizer->current_line++;
                tokenizer->current_column = 1;
            } else {
                tokenizer->current_column++;
            }
            i++;
        }
        
        if (i >= len) break;
        
        struct Token* token = &tokenizer->tokens[tokenizer->token_count];
        token->line = tokenizer->current_line;
        token->column = tokenizer->current_column;
        
        if (isalpha(input[i]) || input[i] == '_') {
            int j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[i]) || input[i] == '_')) {
                token->value[j++] = input[i++];
                tokenizer->current_column++;
            }
            token->value[j] = '\0';
            
            if (is_keyword(token->value)) {
                token->type = TOKEN_KEYWORD;
            } else {
                token->type = TOKEN_IDENTIFIER;
            }
            
            tokenizer->token_count++;
        } else if (isdigit(input[i])) {
            int j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && 
                   (isdigit(input[i]) || input[i] == '.')) {
                token->value[j++] = input[i++];
                tokenizer->current_column++;
            }
            token->value[j] = '\0';
            token->type = TOKEN_NUMBER;
            tokenizer->token_count++;
        } else if (is_operator_char(input[i])) {
            int j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && 
                   is_operator_char(input[i])) {
                token->value[j++] = input[i++];
                tokenizer->current_column++;
            }
            token->value[j] = '\0';
            token->type = TOKEN_OPERATOR;
            tokenizer->token_count++;
        } else if (is_delimiter(input[i])) {
            token->value[0] = input[i++];
            token->value[1] = '\0';
            token->type = TOKEN_DELIMITER;
            tokenizer->current_column++;
            tokenizer->token_count++;
        } else {
            token->value[0] = input[i++];
            token->value[1] = '\0';
            token->type = TOKEN_UNKNOWN;
            tokenizer->current_column++;
            tokenizer->token_count++;
        }
    }
    
    return 1;
}

void parser_init(struct Parser* parser, struct Tokenizer* tokenizer) {
    if (parser == NULL || tokenizer == NULL) return;
    parser->tokenizer = tokenizer;
    parser->current_token = 0;
}

int parser_has_more_tokens(struct Parser* parser) {
    if (parser == NULL || parser->tokenizer == NULL) return 0;
    return parser->current_token < parser->tokenizer->token_count;
}

struct Token* parser_get_current_token(struct Parser* parser) {
    if (parser == NULL || parser->tokenizer == NULL) return NULL;
    if (parser->current_token < parser->tokenizer->token_count) {
        return &parser->tokenizer->tokens[parser->current_token];
    }
    return NULL;
}

void parser_advance(struct Parser* parser) {
    if (parser == NULL) return;
    if (parser->current_token < parser->tokenizer