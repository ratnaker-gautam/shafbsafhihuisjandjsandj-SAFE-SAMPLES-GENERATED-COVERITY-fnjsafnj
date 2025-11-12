//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int is_punctuation(char c) {
    return c == '(' || c == ')' || c == ',' || c == ';';
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *p = input;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) p++;
        if (!*p) break;
        
        if (isdigit(*p)) {
            int i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = NUMBER;
            token_count++;
        } else if (isalpha(*p)) {
            int i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = IDENTIFIER;
            token_count++;
        } else if (is_operator(*p)) {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = OPERATOR;
            token_count++;
        } else if (is_punctuation(*p)) {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = PUNCTUATION;
            token_count++;
        } else {
            printf("Error: Invalid character '%c'\n", *p);
            return -1;
        }
    }
    
    tokens[token_count].type = END;
    strcpy(tokens[token_count].value, "");
    return token_count;
}

void print_tokens(const struct Token *tokens, int count) {
    const char *type_names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "PUNCTUATION", "END"};
    
    int i = 0;
    while (i <= count) {
        printf("Token %d: %s -> %s\n", i, type_names[tokens[i].type], tokens[i].value);
        i++;
        if (tokens[i-1].type == END) break;
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS + 1];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    if (token_count < 0) {
        return 1;
    }
    
    printf("\nTokens found:\n");
    print_tokens(tokens, token_count);
    
    return 0;
}