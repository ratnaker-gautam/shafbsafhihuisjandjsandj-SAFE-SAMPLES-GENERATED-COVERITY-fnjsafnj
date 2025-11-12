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

int parse_tokens(const char *input, struct Token *tokens, int max_tokens) {
    int token_count = 0;
    const char *ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (isspace(*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        if (isdigit(*ptr)) {
            int i = 0;
            while (isdigit(*ptr) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = NUMBER;
            token_count++;
            continue;
        }
        
        if (isalpha(*ptr)) {
            int i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *ptr++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = IDENTIFIER;
            token_count++;
            continue;
        }
        
        if (is_operator(*ptr)) {
            tokens[token_count].value[0] = *ptr;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = OPERATOR;
            token_count++;
            ptr++;
            continue;
        }
        
        if (is_delimiter(*ptr)) {
            tokens[token_count].value[0] = *ptr;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = DELIMITER;
            token_count++;
            ptr++;
            continue;
        }
        
        tokens[token_count].value[0] = *ptr;
        tokens[token_count].value[1] = '\0';
        tokens[token_count].type = UNKNOWN;
        token_count++;
        ptr++;
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    const char *names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "DELIMITER", "UNKNOWN"};
    if (type >= 0 && type <= 4) {
        printf("%s", names[type]);
    }
}

int main(void) {
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
    
    printf("Tokens found: %d\n", token_count);
    printf("Token list:\n");
    
    int i = 0;
    while (i < token_count) {
        printf("Token %d: ", i + 1);
        print_token_type(tokens[i].type);
        printf(" -> '%s'\n", tokens[i].value);
        i++;
    }
    
    printf("\nToken summary:\n");
    int counts[5] = {0};
    for (int j = 0; j < token_count; j++) {
        if (tokens[j].type >= 0 && tokens[j].type <= 4) {
            counts[tokens[j].type]++;
        }
    }
    
    const char *type_names[] = {"Numbers", "Identifiers", "Operators", "Delimiters", "Unknown"};
    for (int k = 0; k < 5; k++) {
        printf("%s: %d\n", type_names[k], counts[k]);
    }
    
    return 0;
}