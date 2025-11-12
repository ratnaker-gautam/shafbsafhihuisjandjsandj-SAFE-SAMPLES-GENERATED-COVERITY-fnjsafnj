//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LINE 1024
#define MAX_TOKENS 256
#define MAX_TOKEN_LEN 64

enum token_type {
    TOKEN_NUMBER,
    TOKEN_WORD,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_UNKNOWN
};

struct token {
    enum token_type type;
    char value[MAX_TOKEN_LEN];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_punctuation(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';' || c == ':';
}

int tokenize_line(const char *line, struct token *tokens, int *token_count) {
    int pos = 0;
    int len = strlen(line);
    
    while (pos < len && *token_count < MAX_TOKENS) {
        while (pos < len && isspace(line[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        if (isdigit(line[pos])) {
            int start = pos;
            while (pos < len && (isdigit(line[pos]) || line[pos] == '.')) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[*token_count].value, &line[start], token_len);
            tokens[*token_count].value[token_len] = '\0';
            tokens[*token_count].type = TOKEN_NUMBER;
            (*token_count)++;
        } else if (isalpha(line[pos]) || line[pos] == '_') {
            int start = pos;
            while (pos < len && (isalnum(line[pos]) || line[pos] == '_')) {
                pos++;
            }
            int token_len = pos - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[*token_count].value, &line[start], token_len);
            tokens[*token_count].value[token_len] = '\0';
            tokens[*token_count].type = TOKEN_WORD;
            (*token_count)++;
        } else if (is_operator(line[pos])) {
            tokens[*token_count].value[0] = line[pos];
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = TOKEN_OPERATOR;
            (*token_count)++;
            pos++;
        } else if (is_punctuation(line[pos])) {
            tokens[*token_count].value[0] = line[pos];
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = TOKEN_PUNCTUATION;
            (*token_count)++;
            pos++;
        } else {
            tokens[*token_count].value[0] = line[pos];
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = TOKEN_UNKNOWN;
            (*token_count)++;
            pos++;
        }
    }
    
    return 0;
}

void print_token_type(enum token_type type) {
    switch (type) {
        case TOKEN_NUMBER:
            printf("NUMBER");
            break;
        case TOKEN_WORD:
            printf("WORD");
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR");
            break;
        case TOKEN_PUNCTUATION:
            printf("PUNCTUATION");
            break;
        default:
            printf("UNKNOWN");
            break;
    }
}

int main(void) {
    char input_line[MAX_INPUT_LINE];
    struct token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter text to tokenize (empty line to exit):\n");
    
    while (fgets(input_line, sizeof(input_line), stdin) != NULL) {
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        if (strlen(input_line) == 0) {
            break;
        }
        
        token_count = 0;
        if (tokenize_line(input_line, tokens, &token_count) != 0) {
            fprintf(stderr, "Error tokenizing line\n");
            continue;
        }
        
        printf("Tokens found: %d\n", token_count);
        for (int i = 0; i < token_count; i++) {
            printf("Token %d: Type=", i + 1);
            print_token_type(tokens[i].type);
            printf(", Value='%s'\n", tokens[i].value);
        }
        printf("\nEnter next line (empty to exit):\n");
    }
    
    return 0;
}