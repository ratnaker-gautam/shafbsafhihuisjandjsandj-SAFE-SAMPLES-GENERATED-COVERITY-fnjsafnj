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
    return c == '(' || c == ')' || c == ';' || c == ',' || c == '{' || c == '}';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int i = 0;
    while (isdigit(input[*pos])) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        buffer[i++] = input[(*pos)++];
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int i = 0;
    if (isalpha(input[*pos]) || input[*pos] == '_') {
        buffer[i++] = input[(*pos)++];
        while (isalnum(input[*pos]) || input[*pos] == '_') {
            if (i >= MAX_TOKEN_LEN - 1) break;
            buffer[i++] = input[(*pos)++];
        }
        buffer[i] = '\0';
        return 1;
    }
    return 0;
}

void tokenize(const char *input, struct Token *tokens, int *token_count) {
    int pos = 0;
    int count = 0;
    int len = strlen(input);
    
    while (pos < len && count < MAX_TOKENS - 1) {
        while (isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        char buffer[MAX_TOKEN_LEN];
        
        if (parse_number(input, &pos, buffer)) {
            tokens[count].type = NUMBER;
            strncpy(tokens[count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[count].value[MAX_TOKEN_LEN - 1] = '\0';
            count++;
            continue;
        }
        
        if (parse_identifier(input, &pos, buffer)) {
            tokens[count].type = IDENTIFIER;
            strncpy(tokens[count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[count].value[MAX_TOKEN_LEN - 1] = '\0';
            count++;
            continue;
        }
        
        if (is_operator(input[pos])) {
            tokens[count].type = OPERATOR;
            tokens[count].value[0] = input[pos];
            tokens[count].value[1] = '\0';
            pos++;
            count++;
            continue;
        }
        
        if (is_punctuation(input[pos])) {
            tokens[count].type = PUNCTUATION;
            tokens[count].value[0] = input[pos];
            tokens[count].value[1] = '\0';
            pos++;
            count++;
            continue;
        }
        
        pos++;
    }
    
    tokens[count].type = END;
    tokens[count].value[0] = '\0';
    *token_count = count + 1;
}

void print_tokens(const struct Token *tokens, int token_count) {
    const char *type_names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "PUNCTUATION", "END"};
    int i = 0;
    do {
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
        i++;
    } while (i < token_count && tokens[i].type != END);
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    tokenize(input, tokens, &token_count);
    
    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        if (tokens[i].type == END) break;
        const char *type_names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "PUNCTUATION", "END"};
        printf("Token %d: %s -> '%s'\n", i, type_names[tokens[i].type], tokens[i].value);
    }
    
    return 0;
}