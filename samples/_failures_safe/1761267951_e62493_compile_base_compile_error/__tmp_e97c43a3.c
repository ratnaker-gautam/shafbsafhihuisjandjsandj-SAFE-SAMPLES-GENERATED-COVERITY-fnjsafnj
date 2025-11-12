//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(const char *input, int *pos) {
    while (input[*pos] && isspace(input[*pos])) {
        (*pos)++;
    }
}

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    
    while (input[*pos] && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    
    if (input[*pos] && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
        
        token->type = TOKEN_IDENTIFIER;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    if (is_operator_char(input[*pos])) {
        token->type = TOKEN_OPERATOR;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    if (input[*pos] == '"') {
        int start = *pos;
        (*pos)++;
        
        while (input[*pos] && input[*pos] != '"') {
            if (input[*pos] == '\\' && input[*pos + 1]) {
                (*pos) += 2;
            } else {
                (*pos)++;
            }
        }
        
        if (input[*pos] == '"') {
            (*pos)++;
            token->type = TOKEN_STRING;
            strncpy(token->value, &input[start], *pos - start);
            token->value[*pos - start] = '\0';
            return 1;
        }
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    while (input[pos] && token_count < max_tokens) {
        skip_whitespace(input, &pos);
        if (!input[pos]) break;
        
        int current_column = column;
        struct Token token = {0};
        token.line = line;
        token.column = current_column;
        
        if (parse_number(input, &pos, &token) ||
            parse_string(input, &pos, &token) ||
            parse_identifier(input, &pos, &token) ||
            parse_operator(input, &pos, &token)) {
            
            tokens[token_count] = token;
            token_count++;
            
            for (int i = 0; i < strlen(token.value); i++) {
                if (token.value[i] == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
            }
        } else {
            printf("Error: Unexpected character '%c' at line %d, column %d\n", 
                   input[pos], line, column);
            pos++;
            column++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }
    
    return token_count;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("Type: %-12s Value: %-15s Line: %2d Column: %2d\n",
           type_names[token.type], token.value, token.line, token.column);
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (f