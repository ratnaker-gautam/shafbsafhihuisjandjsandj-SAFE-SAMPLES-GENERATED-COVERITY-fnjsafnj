//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *op) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        op[len++] = c;
        char next = input[pos + 1];
        
        if ((c == '=' && next == '=') ||
            (c == '!' && next == '=') ||
            (c == '<' && next == '=') ||
            (c == '>' && next == '=') ||
            (c == '&' && next == '&') ||
            (c == '|' && next == '|')) {
            op[len++] = next;
        }
    }
    
    op[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_string(const char *input, int pos, char *str) {
    if (input[pos] != '"') return 0;
    
    int len = 0;
    str[len++] = '"';
    pos++;
    
    while (input[pos] != '\0' && input[pos] != '"') {
        if (len >= 62) break;
        str[len++] = input[pos++];
    }
    
    if (input[pos] == '"') {
        str[len++] = '"';
        str[len] = '\0';
        return len + 1;
    }
    
    return 0;
}

int parse_number(const char *input, int pos, char *num) {
    int len = 0;
    
    if (isdigit(input[pos])) {
        while (isdigit(input[pos]) && len < 63) {
            num[len++] = input[pos++];
        }
        num[len] = '\0';
    }
    
    return len;
}

int parse_identifier(const char *input, int pos, char *ident) {
    int len = 0;
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        while ((isalnum(input[pos]) || input[pos] == '_') && len < 63) {
            ident[len++] = input[pos++];
        }
        ident[len] = '\0';
    }
    
    return len;
}

int get_next_token(const char *input, int *pos, int line, struct Token *token) {
    int start_pos = *pos;
    int column = start_pos + 1;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            column = 0;
        }
        (*pos)++;
        column++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = line;
        token->column = column;
        return 1;
    }
    
    char buffer[64];
    int len;
    
    if ((len = parse_string(input, *pos, buffer)) > 0) {
        token->type = TOKEN_STRING;
        strncpy(token->value, buffer, 63);
        token->value[63] = '\0';
        *pos += len;
    } else if ((len = parse_number(input, *pos, buffer)) > 0) {
        token->type = TOKEN_NUMBER;
        strncpy(token->value, buffer, 63);
        token->value[63] = '\0';
        *pos += len;
    } else if ((len = parse_operator(input, *pos, buffer)) > 0) {
        token->type = TOKEN_OPERATOR;
        strncpy(token->value, buffer, 63);
        token->value[63] = '\0';
        *pos += len;
    } else if ((len = parse_identifier(input, *pos, buffer)) > 0) {
        token->type = TOKEN_IDENTIFIER;
        strncpy(token->value, buffer, 63);
        token->value[63] = '\0';
        *pos += len;
    } else {
        token->type = TOKEN_UNKNOWN;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
    }
    
    token->line = line;
    token->column = column;
    return 1;
}

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1