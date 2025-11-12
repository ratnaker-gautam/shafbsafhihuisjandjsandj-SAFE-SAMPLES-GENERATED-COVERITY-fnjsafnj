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
    return isalnum(c) || c == '_' || c == '-';
}

int parse_number(const char *str, int *pos, char *result) {
    int start = *pos;
    int len = 0;
    
    while (str[*pos] && (isdigit(str[*pos]) || str[*pos] == '.')) {
        if (len < 63) {
            result[len++] = str[*pos];
        }
        (*pos)++;
    }
    result[len] = '\0';
    return len > 0;
}

int parse_string(const char *str, int *pos, char *result) {
    if (str[*pos] != '"') return 0;
    
    (*pos)++;
    int len = 0;
    
    while (str[*pos] && str[*pos] != '"') {
        if (str[*pos] == '\\' && str[*pos + 1]) {
            (*pos)++;
            if (str[*pos] == 'n') result[len++] = '\n';
            else if (str[*pos] == 't') result[len++] = '\t';
            else result[len++] = str[*pos];
        } else {
            if (len < 63) {
                result[len++] = str[*pos];
            }
        }
        (*pos)++;
    }
    
    if (str[*pos] == '"') {
        (*pos)++;
        result[len] = '\0';
        return 1;
    }
    return 0;
}

int parse_symbol(const char *str, int *pos, char *result) {
    if (!is_symbol_char(str[*pos])) return 0;
    
    int len = 0;
    while (str[*pos] && is_symbol_char(str[*pos])) {
        if (len < 63) {
            result[len++] = str[*pos];
        }
        (*pos)++;
    }
    result[len] = '\0';
    return len > 0;
}

int get_next_token(const char *str, int *pos, int line, struct Token *token) {
    if (!str || *pos < 0 || !token) return 0;
    
    while (str[*pos] && isspace(str[*pos])) {
        if (str[*pos] == '\n') line++;
        (*pos)++;
    }
    
    if (!str[*pos]) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = line;
        return 1;
    }
    
    token->line = line;
    
    if (parse_number(str, pos, token->value)) {
        token->type = TOKEN_NUMBER;
        return 1;
    }
    
    if (parse_string(str, pos, token->value)) {
        token->type = TOKEN_STRING;
        return 1;
    }
    
    if (parse_symbol(str, pos, token->value)) {
        token->type = TOKEN_SYMBOL;
        return 1;
    }
    
    return 0;
}

void print_token(struct Token *token) {
    const char *type_names[] = {"NUMBER", "STRING", "SYMBOL", "EOF"};
    printf("Line %d: %s", token->line, type_names[token->type]);
    
    if (token->type != TOKEN_EOF) {
        printf(" '%s'", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max 1023 chars): ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    int pos = 0;
    int line = 1;
    struct Token token;
    int token_count = 0;
    
    printf("\nTokens:\n");
    printf("-------\n");
    
    while (get_next_token(input, &pos, line, &token)) {
        print_token(&token);
        token_count++;
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        if (token_count >= 100) {
            printf("Token limit reached\n");
            break;
        }
    }
    
    printf("\nTotal tokens: %d\n", token_count);
    return 0;
}