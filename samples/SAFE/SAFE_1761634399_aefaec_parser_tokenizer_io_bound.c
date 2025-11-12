//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
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
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';' || c == '.';
}

void classify_token(struct token *t) {
    if (isdigit(t->value[0])) {
        t->type = TOKEN_NUMBER;
        for (size_t i = 1; t->value[i] != '\0'; i++) {
            if (!isdigit(t->value[i])) {
                t->type = TOKEN_UNKNOWN;
                return;
            }
        }
    } else if (isalpha(t->value[0])) {
        t->type = TOKEN_WORD;
        for (size_t i = 1; t->value[i] != '\0'; i++) {
            if (!isalnum(t->value[i])) {
                t->type = TOKEN_UNKNOWN;
                return;
            }
        }
    } else if (strlen(t->value) == 1) {
        if (is_operator(t->value[0])) {
            t->type = TOKEN_OPERATOR;
        } else if (is_punctuation(t->value[0])) {
            t->type = TOKEN_PUNCTUATION;
        } else {
            t->type = TOKEN_UNKNOWN;
        }
    } else {
        t->type = TOKEN_UNKNOWN;
    }
}

int tokenize_line(const char *line, struct token *tokens, int *token_count) {
    const char *p = line;
    int count = 0;
    
    while (*p != '\0' && count < MAX_TOKENS) {
        while (isspace(*p)) p++;
        
        if (*p == '\0') break;
        
        if (is_operator(*p) || is_punctuation(*p)) {
            if (count >= MAX_TOKENS) return -1;
            tokens[count].value[0] = *p;
            tokens[count].value[1] = '\0';
            classify_token(&tokens[count]);
            count++;
            p++;
            continue;
        }
        
        size_t len = 0;
        const char *start = p;
        
        if (isdigit(*p)) {
            while (isdigit(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[count].value[len++] = *p++;
            }
        } else if (isalpha(*p)) {
            while (isalnum(*p) && len < MAX_TOKEN_LEN - 1) {
                tokens[count].value[len++] = *p++;
            }
        } else {
            tokens[count].value[len++] = *p++;
            tokens[count].type = TOKEN_UNKNOWN;
        }
        
        if (len >= MAX_TOKEN_LEN) return -1;
        
        tokens[count].value[len] = '\0';
        if (tokens[count].type != TOKEN_UNKNOWN) {
            classify_token(&tokens[count]);
        }
        count++;
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
    char input[MAX_INPUT_LINE];
    struct token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter lines to tokenize (empty line to quit):\n");
    
    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            break;
        }
        
        if (tokenize_line(input, tokens, &token_count) != 0) {
            printf("Error: Input too long or invalid\n");
            continue;
        }
        
        printf("Tokens: ");
        for (int i = 0; i < token_count; i++) {
            printf("[%s:", tokens[i].value);
            print_token_type(tokens[i].type);
            printf("] ");
        }
        printf("\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}