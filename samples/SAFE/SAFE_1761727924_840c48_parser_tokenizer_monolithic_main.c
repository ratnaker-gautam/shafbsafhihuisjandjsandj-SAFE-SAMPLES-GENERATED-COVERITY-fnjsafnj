//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int get_token(struct Token *token, const char *input, int *pos, int *line, int *col) {
    if (input == NULL || token == NULL || pos == NULL || line == NULL || col == NULL) return -1;
    
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        if (input[*pos] == '\n') {
            (*line)++;
            *col = 1;
        } else {
            (*col)++;
        }
        (*pos)++;
    }
    
    if (input[*pos] == '\0') {
        token->type = TOKEN_EOF;
        strcpy(token->value, "");
        token->line = *line;
        token->column = *col;
        return 0;
    }
    
    token->line = *line;
    token->column = *col;
    
    if (isdigit(input[*pos])) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (input[*pos] != '\0' && (isdigit(input[*pos]) || input[*pos] == '.')) {
            if (i < 63) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
    } else if (isalpha(input[*pos]) || input[*pos] == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while (input[*pos] != '\0' && (isalnum(input[*pos]) || input[*pos] == '_')) {
            if (i < 63) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
    } else if (input[*pos] == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        (*pos)++;
        (*col)++;
        while (input[*pos] != '\0' && input[*pos] != '"') {
            if (i < 63) {
                token->value[i++] = input[*pos];
            }
            (*pos)++;
            (*col)++;
        }
        if (input[*pos] == '"') {
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
    } else if (is_operator_char(input[*pos])) {
        token->type = TOKEN_OPERATOR;
        int i = 0;
        while (input[*pos] != '\0' && is_operator_char(input[*pos]) && i < 63) {
            token->value[i++] = input[*pos];
            (*pos)++;
            (*col)++;
        }
        token->value[i] = '\0';
    } else {
        token->type = TOKEN_ERROR;
        token->value[0] = input[*pos];
        token->value[1] = '\0';
        (*pos)++;
        (*col)++;
    }
    
    return 0;
}

void print_token(const struct Token *token) {
    if (token == NULL) return;
    
    const char *type_names[] = {
        "NUMBER", "STRING", "IDENTIFIER", "OPERATOR", "EOF", "ERROR"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter text to tokenize (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    int pos = 0;
    int line = 1;
    int col = 1;
    struct Token token;
    
    printf("Tokens:\n");
    while (1) {
        if (get_token(&token, input, &pos, &line, &col) != 0) {
            fprintf(stderr, "Error getting token\n");
            return 1;
        }
        
        print_token(&token);
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            break;
        }
    }
    
    return 0;
}