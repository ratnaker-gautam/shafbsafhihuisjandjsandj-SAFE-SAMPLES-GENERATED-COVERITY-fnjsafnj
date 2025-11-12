//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct Parser {
    const char *input;
    size_t pos;
    size_t len;
    int line;
    int column;
    struct Token current;
};

void parser_init(struct Parser *p, const char *input) {
    p->input = input;
    p->pos = 0;
    p->len = strlen(input);
    p->line = 1;
    p->column = 1;
    p->current.type = TOKEN_ERROR;
    memset(p->current.value, 0, sizeof(p->current.value));
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct Parser *p) {
    while (p->pos < p->len && isspace(p->input[p->pos])) {
        if (p->input[p->pos] == '\n') {
            p->line++;
            p->column = 1;
        } else {
            p->column++;
        }
        p->pos++;
    }
}

struct Token next_token(struct Parser *p) {
    struct Token token;
    memset(token.value, 0, sizeof(token.value));
    token.line = p->line;
    token.column = p->column;
    
    skip_whitespace(p);
    
    if (p->pos >= p->len) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char c = p->input[p->pos];
    
    if (isdigit(c)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (p->pos < p->len && i < 31 && isdigit(p->input[p->pos])) {
            token.value[i++] = p->input[p->pos++];
            p->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(c) || c == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (p->pos < p->len && i < 31 && (isalnum(p->input[p->pos]) || p->input[p->pos] == '_')) {
            token.value[i++] = p->input[p->pos++];
            p->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(c)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (p->pos < p->len && i < 31 && is_operator_char(p->input[p->pos])) {
            token.value[i++] = p->input[p->pos++];
            p->column++;
        }
        token.value[i] = '\0';
    } else if (c == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        p->pos++;
        p->column++;
        while (p->pos < p->len && i < 31 && p->input[p->pos] != '"') {
            token.value[i++] = p->input[p->pos++];
            p->column++;
        }
        if (p->pos < p->len && p->input[p->pos] == '"') {
            p->pos++;
            p->column++;
        } else {
            token.type = TOKEN_ERROR;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = c;
        token.value[1] = '\0';
        p->pos++;
        p->column++;
    }
    
    return token;
}

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void parse_expression(struct Parser *p) {
    struct Token token = p->current;
    
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("Found number: %s\n", token.value);
            break;
        case TOKEN_IDENTIFIER:
            printf("Found identifier: %s\n", token.value);
            break;
        case TOKEN_STRING:
            printf("Found string: \"%s\"\n", token.value);
            break;
        case TOKEN_OPERATOR:
            printf("Found operator: %s\n", token.value);
            break;
        case TOKEN_ERROR:
            printf("Syntax error at line %d, column %d: unexpected '%s'\n", 
                   token.line, token.column, token.value);
            break;
        case TOKEN_EOF:
            printf("End of input\n");
            break;
        default:
            printf("Unknown token type\n");
            break;
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to