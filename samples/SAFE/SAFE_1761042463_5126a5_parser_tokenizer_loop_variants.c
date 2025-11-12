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

int parse_number(const char *input, int *pos, char *buffer) {
    int i = 0;
    int start = *pos;
    while (isdigit(input[*pos])) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        buffer[i++] = input[(*pos)++];
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int i = 0;
    int start = *pos;
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

int tokenize(const char *input, struct Token *tokens, int *token_count) {
    int pos = 0;
    int count = 0;
    int len = strlen(input);
    
    while (pos < len && count < MAX_TOKENS) {
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
        
        break;
    }
    
    tokens[count].type = END;
    tokens[count].value[0] = '\0';
    *token_count = count;
    return 1;
}

void print_tokens(const struct Token *tokens, int count) {
    const char *type_names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "PUNCTUATION", "END"};
    int i = 0;
    do {
        printf("Token %d: %s -> %s\n", i, type_names[tokens[i].type], tokens[i].value);
        i++;
    } while (i <= count && tokens[i-1].type != END);
}

int main() {
    char input[1024];
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Token tokens[MAX_TOKENS + 1];
    int token_count = 0;
    
    if (!tokenize(input, tokens, &token_count)) {
        fprintf(stderr, "Tokenization failed\n");
        return 1;
    }
    
    print_tokens(tokens, token_count);
    return 0;
}