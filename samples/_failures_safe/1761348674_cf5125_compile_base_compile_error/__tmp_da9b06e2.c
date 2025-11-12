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
    
    while (input[pos] != '\0' && token_count < max_tokens - 1) {
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
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit((unsigned char)input[pos])) {
            tokens[token_count].type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit((unsigned char)input[pos]) && i < 63) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else if (isalpha((unsigned char)input[pos]) || input[pos] == '_') {
            tokens[token_count].type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum((unsigned char)input[pos]) || input[pos] == '_') && i < 63) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else if (input[pos] == '"') {
            tokens[token_count].type = TOKEN_STRING;
            int i = 0;
            pos++;
            column++;
            while (input[pos] != '"' && input[pos] != '\0' && i < 63) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            if (input[pos] == '"') {
                pos++;
                column++;
            }
            tokens[token_count].value[i] = '\0';
            token_count++;
        } else if (is_operator_char(input[pos])) {
            tokens[token_count].type = TOKEN_OPERATOR;
            int len = get_operator_length(input + pos);
            strncpy(tokens[token_count].value, input + pos, len);
            tokens[token_count].value[len] = '\0';
            pos += len;
            column += len;
            token_count++;
        } else {
            tokens[token_count].type = TOKEN_UNKNOWN;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            pos++;
            column++;
            token_count++;
        }
    }
    
    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].value[0] = '\0';
    tokens[token_count].line = line;
    tokens[token_count].column = column;
    
    return token_count + 1;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    printf("(%s, '%s') at line %d, column %d\n", 
           type_names[token.type], token.value, token.line, token.column);
}

int main(void) {
    char input[1024];
    struct Token tokens[256];
    
    printf("Enter input to tokenize (max 1023 characters):\n");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, 256);
    
    printf("\nTokens found:\n");
    for (