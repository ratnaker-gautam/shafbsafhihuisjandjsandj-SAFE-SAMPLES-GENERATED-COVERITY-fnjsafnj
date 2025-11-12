//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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

int parse_operator(const char *input, int pos, char *buffer) {
    int len = 0;
    char c = input[pos];
    
    if (c == '&' && input[pos + 1] == '&') {
        buffer[len++] = '&';
        buffer[len++] = '&';
    } else if (c == '|' && input[pos + 1] == '|') {
        buffer[len++] = '|';
        buffer[len++] = '|';
    } else if (c == '=' && input[pos + 1] == '=') {
        buffer[len++] = '=';
        buffer[len++] = '=';
    } else if (c == '!' && input[pos + 1] == '=') {
        buffer[len++] = '!';
        buffer[len++] = '=';
    } else if (c == '<' && input[pos + 1] == '=') {
        buffer[len++] = '<';
        buffer[len++] = '=';
    } else if (c == '>' && input[pos + 1] == '=') {
        buffer[len++] = '>';
        buffer[len++] = '=';
    } else {
        buffer[len++] = c;
    }
    
    buffer[len] = '\0';
    return len;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    int input_len = (int)strlen(input);
    
    while (pos < input_len && token_count < max_tokens - 1) {
        char c = input[pos];
        
        if (isspace(c)) {
            if (c == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit(c)) {
            int len = 0;
            while (pos < input_len && isdigit(input[pos])) {
                if (len < 63) {
                    token->value[len++] = input[pos];
                }
                pos++;
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_NUMBER;
            token_count++;
        } else if (isalpha(c) || c == '_') {
            int len = 0;
            while (pos < input_len && (isalnum(input[pos]) || input[pos] == '_')) {
                if (len < 63) {
                    token->value[len++] = input[pos];
                }
                pos++;
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_IDENTIFIER;
            token_count++;
        } else if (c == '"') {
            int len = 0;
            pos++;
            column++;
            while (pos < input_len && input[pos] != '"') {
                if (len < 63) {
                    token->value[len++] = input[pos];
                }
                pos++;
                column++;
            }
            if (pos < input_len && input[pos] == '"') {
                pos++;
                column++;
            }
            token->value[len] = '\0';
            token->type = TOKEN_STRING;
            token_count++;
        } else if (is_operator_char(c)) {
            int op_len = parse_operator(input, pos, token->value);
            pos += op_len;
            column += op_len;
            token->type = TOKEN_OPERATOR;
            token_count++;
        } else {
            token->value[0] = c;
            token->value[1] = '\0';
            token->type = TOKEN_UNKNOWN;
            token_count++;
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
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    struct Token tokens[256];
    
    printf("Enter text to tokenize (max %zu characters):\n", sizeof(input) - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n