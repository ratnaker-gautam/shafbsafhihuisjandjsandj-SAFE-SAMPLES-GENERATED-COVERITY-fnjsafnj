//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_symbol_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) pos++;
        
        if (input[pos] == '\0') break;
        
        if (isdigit(input[pos])) {
            int start = pos;
            while (isdigit(input[pos])) pos++;
            int len = pos - start;
            if (len >= 32) len = 31;
            tokens[token_count].type = TOKEN_NUMBER;
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            token_count++;
        }
        else if (input[pos] == '"') {
            pos++;
            int start = pos;
            while (input[pos] != '\0' && input[pos] != '"') pos++;
            if (input[pos] == '"') {
                int len = pos - start;
                if (len >= 32) len = 31;
                tokens[token_count].type = TOKEN_STRING;
                strncpy(tokens[token_count].value, &input[start], len);
                tokens[token_count].value[len] = '\0';
                token_count++;
                pos++;
            } else {
                break;
            }
        }
        else if (is_symbol_char(input[pos])) {
            int start = pos;
            while (is_symbol_char(input[pos])) pos++;
            int len = pos - start;
            if (len >= 32) len = 31;
            tokens[token_count].type = TOKEN_SYMBOL;
            strncpy(tokens[token_count].value, &input[start], len);
            tokens[token_count].value[len] = '\0';
            token_count++;
        }
        else {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_STRING: printf("STRING"); break;
        case TOKEN_SYMBOL: printf("SYMBOL"); break;
        case TOKEN_EOF: printf("EOF"); break;
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[100];
    
    printf("Enter text to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, 100);
    
    if (token_count <= 0) {
        printf("No tokens found\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\tValue\n");
    printf("----\t-----\n");
    
    for (int i = 0; i < token_count; i++) {
        print_token_type(tokens[i].type);
        printf("\t%s\n", tokens[i].value);
    }
    
    return 0;
}