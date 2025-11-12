//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
};

int is_symbol_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_number(const char *input, int *pos, int len, char *buffer) {
    int i = 0;
    int has_dot = 0;
    while (*pos < len && (isdigit(input[*pos]) || input[*pos] == '.')) {
        if (i >= 63) return 0;
        if (input[*pos] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        }
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

int parse_string(const char *input, int *pos, int len, char *buffer) {
    if (*pos >= len || input[*pos] != '"') return 0;
    (*pos)++;
    int i = 0;
    while (*pos < len && input[*pos] != '"') {
        if (i >= 63) return 0;
        if (input[*pos] == '\\') {
            (*pos)++;
            if (*pos >= len) return 0;
        }
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    if (*pos >= len || input[*pos] != '"') return 0;
    buffer[i] = '\0';
    (*pos)++;
    return 1;
}

int parse_symbol(const char *input, int *pos, int len, char *buffer) {
    int i = 0;
    while (*pos < len && is_symbol_char(input[*pos])) {
        if (i >= 63) return 0;
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return i > 0;
}

int skip_whitespace(const char *input, int *pos, int len) {
    while (*pos < len && isspace(input[*pos])) {
        if (input[*pos] == '\n') return 1;
        (*pos)++;
    }
    return 0;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int len = strlen(input);
    int pos = 0;
    int token_count = 0;
    int line = 1;
    
    while (pos < len && token_count < max_tokens) {
        if (skip_whitespace(input, &pos, len)) {
            line++;
            continue;
        }
        
        if (pos >= len) break;
        
        struct Token *token = &tokens[token_count];
        token->line = line;
        
        if (parse_number(input, &pos, len, token->value)) {
            token->type = TOKEN_NUMBER;
            token_count++;
            continue;
        }
        
        if (parse_string(input, &pos, len, token->value)) {
            token->type = TOKEN_STRING;
            token_count++;
            continue;
        }
        
        if (parse_symbol(input, &pos, len, token->value)) {
            token->type = TOKEN_SYMBOL;
            token_count++;
            continue;
        }
        
        if (isalpha(input[pos]) || input[pos] == '_') {
            int i = 0;
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_')) {
                if (i >= 63) break;
                token->value[i++] = input[pos];
                pos++;
            }
            token->value[i] = '\0';
            token->type = TOKEN_SYMBOL;
            token_count++;
            continue;
        }
        
        pos++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        token_count++;
    }
    
    return token_count;
}

void print_token(struct Token token) {
    const char *type_names[] = {"NUMBER", "STRING", "SYMBOL", "EOF"};
    printf("Line %d: %s '%s'\n", token.line, type_names[token.type], token.value);
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Token tokens[100];
    int token_count = tokenize(input, tokens, 100);
    
    if (token_count <= 0) {
        fprintf(stderr, "No tokens found\n");
        return