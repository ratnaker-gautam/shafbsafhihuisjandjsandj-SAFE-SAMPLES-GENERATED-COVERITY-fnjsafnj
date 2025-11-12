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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int skip_whitespace(const char *input, int pos) {
    while (input[pos] != '\0' && isspace(input[pos])) {
        pos++;
    }
    return pos;
}

int read_number(const char *input, int pos, struct Token *token) {
    int start = pos;
    while (input[pos] != '\0' && isdigit(input[pos])) {
        pos++;
    }
    if (pos - start >= MAX_TOKEN_LEN) {
        return -1;
    }
    strncpy(token->value, &input[start], pos - start);
    token->value[pos - start] = '\0';
    token->type = TOKEN_NUMBER;
    return pos;
}

int read_identifier(const char *input, int pos, struct Token *token) {
    int start = pos;
    while (input[pos] != '\0' && (isalnum(input[pos]) || input[pos] == '_')) {
        pos++;
    }
    if (pos - start >= MAX_TOKEN_LEN) {
        return -1;
    }
    strncpy(token->value, &input[start], pos - start);
    token->value[pos - start] = '\0';
    token->type = TOKEN_IDENTIFIER;
    return pos;
}

int read_operator(const char *input, int pos, struct Token *token) {
    int start = pos;
    while (input[pos] != '\0' && is_operator_char(input[pos])) {
        pos++;
    }
    if (pos - start >= MAX_TOKEN_LEN) {
        return -1;
    }
    strncpy(token->value, &input[start], pos - start);
    token->value[pos - start] = '\0';
    token->type = TOKEN_OPERATOR;
    return pos;
}

int read_string(const char *input, int pos, struct Token *token) {
    if (input[pos] != '"') {
        return -1;
    }
    int start = pos;
    pos++;
    while (input[pos] != '\0' && input[pos] != '"') {
        pos++;
    }
    if (input[pos] != '"') {
        return -1;
    }
    pos++;
    if (pos - start - 2 >= MAX_TOKEN_LEN) {
        return -1;
    }
    strncpy(token->value, &input[start + 1], pos - start - 2);
    token->value[pos - start - 2] = '\0';
    token->type = TOKEN_STRING;
    return pos;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    int line = 1;
    int column = 1;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        pos = skip_whitespace(input, pos);
        if (input[pos] == '\0') {
            break;
        }
        
        struct Token token;
        token.line = line;
        token.column = column;
        
        int new_pos = -1;
        
        if (isdigit(input[pos])) {
            new_pos = read_number(input, pos, &token);
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            new_pos = read_identifier(input, pos, &token);
        } else if (is_operator_char(input[pos])) {
            new_pos = read_operator(input, pos, &token);
        } else if (input[pos] == '"') {
            new_pos = read_string(input, pos, &token);
        } else {
            return -1;
        }
        
        if (new_pos == -1) {
            return -1;
        }
        
        column += (new_pos - pos);
        tokens[token_count] = token;
        token_count++;
        pos = new_pos;
        
        if (input[pos] == '\n') {
            line++;
            column = 1;
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
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
}

int main() {
    char input[1024];