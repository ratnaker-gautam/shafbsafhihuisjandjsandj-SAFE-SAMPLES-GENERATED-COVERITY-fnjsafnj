//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[32];
};

int is_symbol_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int start = *pos;
    int has_dot = 0;
    int len = 0;
    
    while (input[*pos] != '\0') {
        if (isdigit(input[*pos])) {
            if (len < 31) buffer[len++] = input[*pos];
            (*pos)++;
        } else if (input[*pos] == '.' && !has_dot) {
            if (len < 31) buffer[len++] = input[*pos];
            (*pos)++;
            has_dot = 1;
        } else {
            break;
        }
    }
    
    if (len > 0) {
        buffer[len] = '\0';
        return 1;
    }
    return 0;
}

int parse_string(const char *input, int *pos, char *buffer) {
    if (input[*pos] != '"') return 0;
    
    (*pos)++;
    int len = 0;
    
    while (input[*pos] != '\0' && input[*pos] != '"') {
        if (input[*pos] == '\\' && input[*pos + 1] != '\0') {
            (*pos)++;
            if (len < 31) buffer[len++] = input[*pos];
            (*pos)++;
        } else {
            if (len < 31) buffer[len++] = input[*pos];
            (*pos)++;
        }
    }
    
    if (input[*pos] == '"') {
        (*pos)++;
        buffer[len] = '\0';
        return 1;
    }
    return 0;
}

int parse_symbol(const char *input, int *pos, char *buffer) {
    int len = 0;
    
    while (input[*pos] != '\0' && is_symbol_char(input[*pos])) {
        if (len < 31) buffer[len++] = input[*pos];
        (*pos)++;
    }
    
    if (len > 0) {
        buffer[len] = '\0';
        return 1;
    }
    return 0;
}

void skip_whitespace(const char *input, int *pos) {
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        (*pos)++;
    }
}

int get_next_token(const char *input, int *pos, struct Token *token) {
    skip_whitespace(input, pos);
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    if (parse_number(input, pos, token->value)) {
        token->type = TOKEN_NUMBER;
        return 1;
    }
    
    if (parse_string(input, pos, token->value)) {
        token->type = TOKEN_STRING;
        return 1;
    }
    
    if (parse_symbol(input, pos, token->value)) {
        token->type = TOKEN_SYMBOL;
        return 1;
    }
    
    return 0;
}

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_SYMBOL: return "SYMBOL";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    struct Token token;
    int token_count = 0;
    
    printf("Tokens:\n");
    printf("Type\t\tValue\n");
    printf("----\t\t-----\n");
    
    while (get_next_token(input, &pos, &token)) {
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        printf("%-12s\t%s\n", token_type_name(token.type), token.value);
        token_count++;
        
        if (token_count >= 100) {
            printf("Too many tokens, stopping\n");
            break;
        }
    }
    
    if (token_count == 0) {
        printf("No valid tokens found\n");
    } else {
        printf("Total tokens: %d\n", token_count);
    }
    
    return 0;
}