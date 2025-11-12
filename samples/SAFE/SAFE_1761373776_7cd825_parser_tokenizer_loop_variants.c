//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF };

struct Token {
    enum TokenType type;
    char value[32];
    int line;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int parse_number(const char *input, int *pos, char *buffer) {
    int i = 0;
    while (isdigit(input[*pos])) {
        if (i >= 31) return 0;
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return 1;
}

int parse_identifier(const char *input, int *pos, char *buffer) {
    int i = 0;
    if (!isalpha(input[*pos]) && input[*pos] != '_') return 0;
    
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        if (i >= 31) return 0;
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return 1;
}

int parse_string(const char *input, int *pos, char *buffer) {
    int i = 0;
    if (input[*pos] != '"') return 0;
    (*pos)++;
    
    while (input[*pos] != '"' && input[*pos] != '\0') {
        if (i >= 31) return 0;
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    
    if (input[*pos] != '"') return 0;
    (*pos)++;
    buffer[i] = '\0';
    return 1;
}

int parse_operator(const char *input, int *pos, char *buffer) {
    int i = 0;
    if (!is_operator_char(input[*pos])) return 0;
    
    while (is_operator_char(input[*pos])) {
        if (i >= 31) return 0;
        buffer[i++] = input[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    return 1;
}

int get_next_token(const char *input, int *pos, int *line, struct Token *token) {
    while (input[*pos] != '\0') {
        if (isspace(input[*pos])) {
            if (input[*pos] == '\n') (*line)++;
            (*pos)++;
            continue;
        }
        
        if (parse_number(input, pos, token->value)) {
            token->type = TOKEN_NUMBER;
            token->line = *line;
            return 1;
        }
        
        if (parse_identifier(input, pos, token->value)) {
            token->type = TOKEN_IDENTIFIER;
            token->line = *line;
            return 1;
        }
        
        if (parse_string(input, pos, token->value)) {
            token->type = TOKEN_STRING;
            token->line = *line;
            return 1;
        }
        
        if (parse_operator(input, pos, token->value)) {
            token->type = TOKEN_OPERATOR;
            token->line = *line;
            return 1;
        }
        
        return 0;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    token->line = *line;
    return 1;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF"
    };
    printf("Line %d: %s", token->line, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" '%s'", token->value);
    }
    printf("\n");
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
    
    if (len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int pos = 0;
    int line = 1;
    struct Token token;
    
    printf("Tokens:\n");
    
    while (1) {
        if (!get_next_token(input, &pos, &line, &token)) {
            fprintf(stderr, "Invalid token at position %d\n", pos);
            return 1;
        }
        
        print_token(&token);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
    
    return 0;
}