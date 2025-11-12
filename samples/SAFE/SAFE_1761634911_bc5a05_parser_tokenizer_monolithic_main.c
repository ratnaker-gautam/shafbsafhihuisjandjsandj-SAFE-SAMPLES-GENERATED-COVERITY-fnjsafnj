//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType { NUMBER, IDENTIFIER, OPERATOR, PUNCTUATION, END };

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_punctuation_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    int pos = 0;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace(input[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        if (isdigit(input[pos])) {
            int start = pos;
            while (pos < len && isdigit(input[pos])) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = NUMBER;
            token_count++;
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            int start = pos;
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_')) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &input[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = IDENTIFIER;
            token_count++;
        } else if (is_operator_char(input[pos])) {
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = OPERATOR;
            token_count++;
            pos++;
        } else if (is_punctuation_char(input[pos])) {
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = PUNCTUATION;
            token_count++;
            pos++;
        } else {
            pos++;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = END;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    switch (type) {
        case NUMBER: printf("NUMBER"); break;
        case IDENTIFIER: printf("IDENTIFIER"); break;
        case OPERATOR: printf("OPERATOR"); break;
        case PUNCTUATION: printf("PUNCTUATION"); break;
        case END: printf("END"); break;
    }
}

int main() {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Token list:\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%d: ", i + 1);
        print_token_type(tokens[i].type);
        printf(" '%s'\n", tokens[i].value);
    }
    
    return 0;
}