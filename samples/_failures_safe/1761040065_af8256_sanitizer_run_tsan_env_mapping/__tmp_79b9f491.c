//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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

int parse_expression(struct Token tokens[], int token_count) {
    int i = 0;
    int balance = 0;
    
    while (i < token_count) {
        switch (tokens[i].type) {
            case PUNCTUATION:
                if (strcmp(tokens[i].value, "(") == 0) balance++;
                else if (strcmp(tokens[i].value, ")") == 0) balance--;
                break;
            case OPERATOR:
                if (i == 0 || i == token_count - 1) return 0;
                if (tokens[i-1].type == OPERATOR || tokens[i+1].type == OPERATOR) return 0;
                break;
            default:
                break;
        }
        if (balance < 0) return 0;
        i++;
    }
    
    return balance == 0;
}

int main(void) {
    char input[512];
    struct Token tokens[MAX_TOKENS];
    int token_count = 0;
    int pos = 0;
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
    
    while (input[pos] != '\0' && token_count < MAX_TOKENS) {
        while (isspace((unsigned char)input[pos])) pos++;
        
        if (input[pos] == '\0') break;
        
        if (isdigit((unsigned char)input[pos])) {
            int start = pos;
            while (isdigit((unsigned char)input[pos])) pos++;
            int length = pos - start;
            if (length >= MAX_TOKEN_LEN) length = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, &input[start], length);
            tokens[token_count].value[length] = '\0';
            tokens[token_count].type = NUMBER;
            token_count++;
        } else if (isalpha((unsigned char)input[pos])) {
            int start = pos;
            while (isalnum((unsigned char)input[pos])) pos++;
            int length = pos - start;
            if (length >= MAX_TOKEN_LEN) length = MAX_TOKEN_LEN - 1;
            strncpy(tokens[token_count].value, &input[start], length);
            tokens[token_count].value[length] = '\0';
            tokens[token_count].type = IDENTIFIER;
            token_count++;
        } else if (is_operator(input[pos])) {
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = OPERATOR;
            token_count++;
            pos++;
        } else if (is_punctuation(input[pos])) {
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = PUNCTUATION;
            token_count++;
            pos++;
        } else {
            fprintf(stderr, "Invalid character: %c\n", input[pos]);
            return 1;
        }
    }
    
    if (token_count >= MAX_TOKENS && input[pos] != '\0') {
        fprintf(stderr, "Too many tokens\n");
        return 1;
    }
    
    tokens[token_count].type = END;
    strcpy(tokens[token_count].value, "");
    
    printf("Tokens:\n");
    for (int i = 0; i < token_count; i++) {
        const char* type_str;
        switch (tokens[i].type) {
            case NUMBER: type_str = "NUMBER"; break;
            case IDENTIFIER: type_str = "IDENTIFIER"; break;
            case OPERATOR: type_str = "OPERATOR"; break;
            case PUNCTUATION: type_str = "PUNCTUATION"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("%s: %s\n", type_str, tokens[i].value);
    }
    
    int valid = parse_expression(tokens, token_count);
    printf("Expression is %svalid\n", valid ? "" : "in");
    
    return 0;
}