//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LINE 1024
#define MAX_TOKENS 256
#define MAX_TOKEN_LEN 128

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
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';' || c == '.';
}

int tokenize_line(const char *line, struct token *tokens, int *token_count) {
    const char *p = line;
    int count = 0;
    
    while (*p && count < MAX_TOKENS - 1) {
        while (*p && isspace(*p)) p++;
        
        if (!*p) break;
        
        if (isdigit(*p)) {
            int len = 0;
            while (*p && isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[count].value[len++] = *p++;
            }
            tokens[count].value[len] = '\0';
            tokens[count].type = TOKEN_NUMBER;
            count++;
        }
        else if (isalpha(*p) || *p == '_') {
            int len = 0;
            while (*p && (isalnum(*p) || *p == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[count].value[len++] = *p++;
            }
            tokens[count].value[len] = '\0';
            tokens[count].type = TOKEN_WORD;
            count++;
        }
        else if (is_operator(*p)) {
            tokens[count].value[0] = *p++;
            tokens[count].value[1] = '\0';
            tokens[count].type = TOKEN_OPERATOR;
            count++;
        }
        else if (is_punctuation(*p)) {
            tokens[count].value[0] = *p++;
            tokens[count].value[1] = '\0';
            tokens[count].type = TOKEN_PUNCTUATION;
            count++;
        }
        else {
            tokens[count].value[0] = *p++;
            tokens[count].value[1] = '\0';
            tokens[count].type = TOKEN_UNKNOWN;
            count++;
        }
    }
    
    *token_count = count;
    return 0;
}

void print_token_type(enum token_type type) {
    switch (type) {
        case TOKEN_NUMBER: printf("NUMBER"); break;
        case TOKEN_WORD: printf("WORD"); break;
        case TOKEN_OPERATOR: printf("OPERATOR"); break;
        case TOKEN_PUNCTUATION: printf("PUNCTUATION"); break;
        case TOKEN_UNKNOWN: printf("UNKNOWN"); break;
    }
}

int main(void) {
    char input_line[MAX_INPUT_LINE];
    struct token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter text to tokenize (empty line to exit):\n");
    
    while (1) {
        printf("> ");
        fflush(stdout);
        
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        if (strlen(input_line) == 0) {
            break;
        }
        
        if (tokenize_line(input_line, tokens, &token_count) != 0) {
            fprintf(stderr, "Error tokenizing input\n");
            continue;
        }
        
        printf("Tokens found: %d\n", token_count);
        for (int i = 0; i < token_count; i++) {
            printf("  %d: '%-16s' [", i + 1, tokens[i].value);
            print_token_type(tokens[i].type);
            printf("]\n");
        }
        printf("\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}