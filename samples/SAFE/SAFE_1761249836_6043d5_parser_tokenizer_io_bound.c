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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_punctuation_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';' || c == '.';
}

int classify_token(const char *str) {
    if (str[0] == '\0') return TOKEN_UNKNOWN;
    
    if (isdigit(str[0])) {
        for (size_t i = 0; str[i] != '\0'; i++) {
            if (!isdigit(str[i]) && str[i] != '.') return TOKEN_UNKNOWN;
        }
        return TOKEN_NUMBER;
    }
    
    if (is_operator_char(str[0])) {
        return TOKEN_OPERATOR;
    }
    
    if (is_punctuation_char(str[0])) {
        return TOKEN_PUNCTUATION;
    }
    
    if (isalpha(str[0]) || str[0] == '_') {
        for (size_t i = 0; str[i] != '\0'; i++) {
            if (!isalnum(str[i]) && str[i] != '_') return TOKEN_UNKNOWN;
        }
        return TOKEN_WORD;
    }
    
    return TOKEN_UNKNOWN;
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
    size_t pos = 0;
    size_t len = strlen(line);
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace(line[pos])) {
            pos++;
        }
        
        if (pos >= len) break;
        
        size_t start = pos;
        size_t token_len = 0;
        
        if (is_operator_char(line[pos]) || is_punctuation_char(line[pos])) {
            token_len = 1;
        } else {
            while (pos < len && !isspace(line[pos]) && 
                   !is_operator_char(line[pos]) && !is_punctuation_char(line[pos])) {
                pos++;
                token_len++;
            }
        }
        
        if (token_len > 0) {
            if (token_len >= MAX_TOKEN_LEN) {
                token_len = MAX_TOKEN_LEN - 1;
            }
            
            strncpy(tokens[token_count].value, &line[start], token_len);
            tokens[token_count].value[token_len] = '\0';
            tokens[token_count].type = classify_token(tokens[token_count].value);
            token_count++;
            
            if (is_operator_char(line[start]) || is_punctuation_char(line[start])) {
                pos = start + 1;
            }
        } else {
            pos++;
        }
    }
    
    return token_count;
}

int main(void) {
    char input_line[MAX_INPUT_LINE];
    struct token tokens[MAX_TOKENS];
    
    printf("Enter lines of text to tokenize (empty line to exit):\n");
    
    while (1) {
        printf("> ");
        
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        if (input_line[0] == '\0') {
            break;
        }
        
        int token_count = tokenize_line(input_line, tokens, MAX_TOKENS);
        
        printf("Tokens found: %d\n", token_count);
        for (int i = 0; i < token_count; i++) {
            printf("  [%d] %s: '%s'\n", i + 1, token_type_name(tokens[i].type), tokens[i].value);
        }
        printf("\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}