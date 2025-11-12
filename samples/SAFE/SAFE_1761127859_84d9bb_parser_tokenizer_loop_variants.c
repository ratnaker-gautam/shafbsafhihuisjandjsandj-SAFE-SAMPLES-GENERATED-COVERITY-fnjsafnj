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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int is_delimiter(char c) {
    return c == '(' || c == ')' || c == ',' || c == ';' || c == '{' || c == '}';
}

int parse_expression(const char* input, struct Token* tokens, int* token_count) {
    int i = 0;
    int len = strlen(input);
    
    while (i < len) {
        if (isspace(input[i])) {
            i++;
            continue;
        }
        
        if (isdigit(input[i])) {
            int j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                tokens[*token_count].value[j++] = input[i++];
            }
            tokens[*token_count].value[j] = '\0';
            tokens[*token_count].type = NUMBER;
            (*token_count)++;
            if (*token_count >= MAX_TOKENS) return -1;
            continue;
        }
        
        if (isalpha(input[i])) {
            int j = 0;
            while (i < len && j < MAX_TOKEN_LEN - 1 && (isalnum(input[i]) || input[i] == '_')) {
                tokens[*token_count].value[j++] = input[i++];
            }
            tokens[*token_count].value[j] = '\0';
            tokens[*token_count].type = IDENTIFIER;
            (*token_count)++;
            if (*token_count >= MAX_TOKENS) return -1;
            continue;
        }
        
        if (is_operator(input[i])) {
            tokens[*token_count].value[0] = input[i++];
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = OPERATOR;
            (*token_count)++;
            if (*token_count >= MAX_TOKENS) return -1;
            continue;
        }
        
        if (is_delimiter(input[i])) {
            tokens[*token_count].value[0] = input[i++];
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = DELIMITER;
            (*token_count)++;
            if (*token_count >= MAX_TOKENS) return -1;
            continue;
        }
        
        tokens[*token_count].value[0] = input[i++];
        tokens[*token_count].value[1] = '\0';
        tokens[*token_count].type = UNKNOWN;
        (*token_count)++;
        if (*token_count >= MAX_TOKENS) return -1;
    }
    
    return 0;
}

void print_tokens(const struct Token* tokens, int token_count) {
    const char* type_names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "DELIMITER", "UNKNOWN"};
    
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=%s, Value='%s'\n", i + 1, type_names[tokens[i].type], tokens[i].value);
    }
}

int main() {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter an expression to tokenize: ");
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
    
    if (parse_expression(input, tokens, &token_count) != 0) {
        fprintf(stderr, "Error: Too many tokens or parsing error\n");
        return 1;
    }
    
    printf("\nParsed %d tokens:\n", token_count);
    print_tokens(tokens, token_count);
    
    return 0;
}