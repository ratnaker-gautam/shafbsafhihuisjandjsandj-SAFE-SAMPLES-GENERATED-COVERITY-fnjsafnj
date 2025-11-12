//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
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
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        output[len++] = c;
        if (is_operator_char(input[pos + 1]) && 
            ((c == '&' && input[pos + 1] == '&') ||
             (c == '|' && input[pos + 1] == '|') ||
             (c == '=' && input[pos + 1] == '=') ||
             (c == '!' && input[pos + 1] == '=') ||
             (c == '<' && input[pos + 1] == '=') ||
             (c == '>' && input[pos + 1] == '='))) {
            output[len++] = input[pos + 1];
        }
    }
    output[len] = '\0';
    return len;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    int has_dot = 0;
    
    while (isdigit(input[pos]) || (input[pos] == '.' && !has_dot)) {
        if (input[pos] == '.') has_dot = 1;
        if (len < 31) output[len++] = input[pos++];
        else break;
    }
    output[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    if (input[pos] != '"') return 0;
    
    pos++;
    while (input[pos] != '\0' && input[pos] != '"') {
        if (len < 31) output[len++] = input[pos++];
        else break;
    }
    output[len] = '\0';
    return (input[pos] == '"') ? len + 2 : 0;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    
    if (!isalpha(input[pos]) && input[pos] != '_') return 0;
    
    while (isalnum(input[pos]) || input[pos] == '_') {
        if (len < 31) output[len++] = input[pos++];
        else break;
    }
    output[len] = '\0';
    return len;
}

int get_next_token(const char *input, int *pos, int line, int *column, struct Token *token) {
    int start_pos = *pos;
    int start_col = *column;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            line++;
            *column = 1;
        } else {
            (*column)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = line;
        token->column = *column;
        return 1;
    }
    
    char buffer[32] = {0};
    int advance = 0;
    
    if ((advance = parse_operator(input, *pos, buffer)) > 0) {
        token->type = TOKEN_OPERATOR;
        strcpy(token->value, buffer);
    } else if ((advance = parse_number(input, *pos, buffer)) > 0) {
        token->type = TOKEN_NUMBER;
        strcpy(token->value, buffer);
    } else if ((advance = parse_string(input, *pos, buffer)) > 0) {
        token->type = TOKEN_STRING;
        strcpy(token->value, buffer);
    } else if ((advance = parse_identifier(input, *pos, buffer)) > 0) {
        token->type = TOKEN_IDENTIFIER;
        strcpy(token->value, buffer);
    } else {
        token->type = TOKEN_INVALID;
        buffer[0] = input[*pos];
        buffer[1] = '\0';
        strcpy(token->value, buffer);
        advance = 1;
    }
    
    token->line = line;
    token->column = *column;
    *pos += advance;
    *column += advance;
    
    return 1;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[102