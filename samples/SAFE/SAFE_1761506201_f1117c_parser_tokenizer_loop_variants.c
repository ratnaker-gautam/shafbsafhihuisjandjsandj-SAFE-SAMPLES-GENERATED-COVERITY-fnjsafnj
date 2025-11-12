//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType { NUMBER, IDENTIFIER, OPERATOR, DELIMITER, UNKNOWN };

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';';
}

int parse_tokens(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int i = 0;
    int input_len = strlen(input);
    
    while (i < input_len && token_count < max_tokens) {
        while (i < input_len && isspace(input[i])) i++;
        if (i >= input_len) break;
        
        char current = input[i];
        
        if (isdigit(current)) {
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                tokens[token_count].value[j++] = input[i++];
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = NUMBER;
            token_count++;
        }
        else if (isalpha(current) || current == '_') {
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && (isalnum(input[i]) || input[i] == '_')) {
                tokens[token_count].value[j++] = input[i++];
            }
            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = IDENTIFIER;
            token_count++;
        }
        else if (is_operator(current)) {
            tokens[token_count].value[0] = current;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = OPERATOR;
            token_count++;
            i++;
        }
        else if (is_delimiter(current)) {
            tokens[token_count].value[0] = current;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = DELIMITER;
            token_count++;
            i++;
        }
        else {
            tokens[token_count].value[0] = current;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = UNKNOWN;
            token_count++;
            i++;
        }
    }
    
    return token_count;
}

void print_tokens(const struct Token* tokens, int count) {
    const char* type_names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "DELIMITER", "UNKNOWN"};
    
    for (int i = 0; i < count; i++) {
        printf("Token %d: %s -> %s\n", i + 1, tokens[i].value, type_names[tokens[i].type]);
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
    
    int token_count = parse_tokens(input, tokens, MAX_TOKENS);
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 0;
    }
    
    printf("Found %d tokens:\n", token_count);
    print_tokens(tokens, token_count);
    
    return 0;
}