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
    } else if (strlen(t->value) == 1 && is_operator(t->value[0])) {
        t->type = TOKEN_OPERATOR;
    } else if (strlen(t->value) == 1 && is_punctuation(t->value[0])) {
        t->type = TOKEN_PUNCTUATION;
    } else {
        t->type = TOKEN_UNKNOWN;
    }
}

const char* token_type_name(enum token_type type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_WORD: return "WORD";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        default: return "UNKNOWN";
    }
}

int tokenize_line(const char *line, struct token *tokens, int max_tokens) {
    int token_count = 0;
    size_t i = 0;
    size_t len = strlen(line);
    
    while (i < len && token_count < max_tokens) {
        while (i < len && isspace(line[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        if (is_operator(line[i]) || is_punctuation(line[i])) {
            if (token_count >= max_tokens) break;
            tokens[token_count].value[0] = line[i];
            tokens[token_count].value[1] = '\0';
            classify_token(&tokens[token_count]);
            token_count++;
            i++;
            continue;
        }
        
        size_t start = i;
        while (i < len && !isspace(line[i]) && !is_operator(line[i]) && !is_punctuation(line[i])) {
            i++;
        }
        
        if (i - start > 0) {
            if (token_count >= max_tokens) break;
            size_t token_len = i - start;
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            strncpy(tokens[token_count].value, &line[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            classify_token(&tokens[token_count]);
            token_count++;
        }
    }
    
    return token_count;
}

int main(void) {
    char input_line[MAX_INPUT_LINE];
    struct token tokens[MAX_TOKENS];
    
    printf("Enter text to tokenize (empty line to exit):\n");
    
    while (1) {
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
        
        int token_count = tokenize_line(input_line, tokens, MAX_TOKENS);
        
        printf("Tokens found: %d\n", token_count);
        for (int i = 0; i < token_count; i++) {
            printf("  %s: '%s'\n", token_type_name(tokens[i].type), tokens[i].value);
        }
        printf("\n");
    }
    
    return 0;
}