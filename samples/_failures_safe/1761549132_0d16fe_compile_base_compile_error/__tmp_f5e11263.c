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

int get_operator_length(const char *str) {
    if (str[0] == '\0') return 0;
    if (str[1] == '\0') return 1;
    if ((str[0] == '=' && str[1] == '=') ||
        (str[0] == '!' && str[1] == '=') ||
        (str[0] == '<' && str[1] == '=') ||
        (str[0] == '>' && str[1] == '=') ||
        (str[0] == '&' && str[1] == '&') ||
        (str[0] == '|' && str[1] == '|')) {
        return 2;
    }
    return 1;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace((unsigned char)input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit((unsigned char)input[pos])) {
            token->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit((unsigned char)input[pos]) && i < 63) {
                token->value[i++] = input[pos++];
                column++;
            }
            token->value[i] = '\0';
            token_count++;
        } else if (isalpha((unsigned char)input[pos]) || input[pos] == '_') {
            token->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum((unsigned char)input[pos]) || input[pos] == '_') && i < 63) {
                token->value[i++] = input[pos++];
                column++;
            }
            token->value[i] = '\0';
            token_count++;
        } else if (input[pos] == '"') {
            token->type = TOKEN_STRING;
            int i = 0;
            pos++;
            column++;
            while (input[pos] != '"' && input[pos] != '\0' && i < 63) {
                token->value[i++] = input[pos++];
                column++;
            }
            token->value[i] = '\0';
            if (input[pos] == '"') {
                pos++;
                column++;
            }
            token_count++;
        } else if (is_operator_char(input[pos])) {
            token->type = TOKEN_OPERATOR;
            int len = get_operator_length(&input[pos]);
            if (len > 0 && len < 64) {
                strncpy(token->value, &input[pos], len);
                token->value[len] = '\0';
                pos += len;
                column += len;
                token_count++;
            } else {
                token->type = TOKEN_UNKNOWN;
                token->value[0] = input[pos];
                token->value[1] = '\0';
                pos++;
                column++;
                token_count++;
            }
        } else {
            token->type = TOKEN_UNKNOWN;
            token->value[0] = input[pos];
            token->value[1] = '\0';
            pos++;
            column++;
            token_count++;
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
    printf("Enter code to tokenize (max 1023 characters):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if