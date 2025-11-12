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
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        output[len++] = c;
        if (is_operator_char(input[pos + 1])) {
            output[len++] = input[pos + 1];
        }
        output[len] = '\0';
    }
    return len;
}

int parse_number(const char *input, int pos, char *output) {
    int len = 0;
    while (isdigit(input[pos])) {
        if (len >= 31) break;
        output[len++] = input[pos++];
    }
    output[len] = '\0';
    return len;
}

int parse_string(const char *input, int pos, char *output) {
    int len = 0;
    if (input[pos] == '"') {
        pos++;
        while (input[pos] != '"' && input[pos] != '\0') {
            if (len >= 31) break;
            output[len++] = input[pos++];
        }
        output[len] = '\0';
        return len + 2;
    }
    return 0;
}

int parse_identifier(const char *input, int pos, char *output) {
    int len = 0;
    if (isalpha(input[pos]) || input[pos] == '_') {
        while (isalnum(input[pos]) || input[pos] == '_') {
            if (len >= 31) break;
            output[len++] = input[pos++];
        }
        output[len] = '\0';
    }
    return len;
}

void tokenize(const char *input, struct Token *tokens, int max_tokens, int *token_count) {
    int pos = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[pos] != '\0' && *token_count < max_tokens - 1) {
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
        
        struct Token *token = &tokens[*token_count];
        token->line = line;
        token->column = column;
        
        int advance = 0;
        char buffer[32] = {0};
        
        if ((advance = parse_string(input, pos, buffer)) > 0) {
            token->type = TOKEN_STRING;
            strncpy(token->value, buffer, 31);
            column += advance;
            pos += advance;
        } else if ((advance = parse_number(input, pos, buffer)) > 0) {
            token->type = TOKEN_NUMBER;
            strncpy(token->value, buffer, 31);
            column += advance;
            pos += advance;
        } else if ((advance = parse_operator(input, pos, buffer)) > 0) {
            token->type = TOKEN_OPERATOR;
            strncpy(token->value, buffer, 31);
            column += advance;
            pos += advance;
        } else if ((advance = parse_identifier(input, pos, buffer)) > 0) {
            token->type = TOKEN_IDENTIFIER;
            strncpy(token->value, buffer, 31);
            column += advance;
            pos += advance;
        } else {
            token->type = TOKEN_UNKNOWN;
            token->value[0] = input[pos];
            token->value[1] = '\0';
            column++;
            pos++;
        }
        
        (*token_count)++;
    }
    
    if (*token_count < max_tokens) {
        tokens[*token_count].type = TOKEN_EOF;
        tokens[*token_count].value[0] = '\0';
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        (*token_count)++;
    }
}

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1) {
        printf("Empty input\n");
        return 1;