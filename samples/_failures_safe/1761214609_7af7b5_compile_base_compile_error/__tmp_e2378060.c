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

int parse_number(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int has_dot = 0;
    int len = strlen(input);
    
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        (*pos)++;
    }
    
    if (*pos - start > 0 && *pos - start < MAX_TOKEN_LEN) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, &input[start], *pos - start);
        token->value[*pos - start] = '\0';
        return 1;
    }
    return 0;
}

int parse_identifier(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int len = strlen(input);
    
    if (*pos < len && (isalpha(input[*pos]) || input[*pos] == '_')) {
        (*pos)++;
        while (*pos < len && (isalnum(input[*pos]) || input[*pos] == '_')) {
            (*pos)++;
        }
        
        if (*pos - start < MAX_TOKEN_LEN) {
            token->type = TOKEN_IDENTIFIER;
            strncpy(token->value, &input[start], *pos - start);
            token->value[*pos - start] = '\0';
            return 1;
        }
    }
    return 0;
}

int parse_operator(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    if (*pos < len && is_operator_char(input[*pos])) {
        token->type = TOKEN_OPERATOR;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, struct Token *token) {
    int len = strlen(input);
    if (*pos < len && input[*pos] == '"') {
        int start = *pos;
        (*pos)++;
        
        while (*pos < len && input[*pos] != '"') {
            (*pos)++;
        }
        
        if (*pos < len && input[*pos] == '"') {
            int value_len = *pos - start - 1;
            if (value_len < MAX_TOKEN_LEN && value_len >= 0) {
                token->type = TOKEN_STRING;
                strncpy(token->value, &input[start + 1], value_len);
                token->value[value_len] = '\0';
                (*pos)++;
                return 1;
            }
        }
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (parse_number(input, &pos, &tokens[token_count]) ||
            parse_identifier(input, &pos, &tokens[token_count]) ||
            parse_operator(input, &pos, &tokens[token_count]) ||
            parse_string(input, &pos, &tokens[token_count])) {
            
            int token_len = strlen(tokens[token_count].value);
            if (token_len > 0) {
                column += token_len;
                token_count++;
            } else {
                pos++;
                column++;
            }
        } else {
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

int main() {
    char input[1024];