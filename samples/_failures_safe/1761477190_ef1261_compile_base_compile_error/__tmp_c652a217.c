//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens, int* token_count) {
    if (input == NULL || tokens == NULL || token_count == NULL) {
        return 0;
    }
    
    int line = 1;
    int column = 1;
    int pos = 0;
    *token_count = 0;
    
    while (input[pos] != '\0' && *token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') {
            break;
        }
        
        struct Token* token = &tokens[*token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit(input[pos])) {
            token->type = TOKEN_NUMBER;
            int len = 0;
            while (isdigit(input[pos]) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = input[pos++];
                column++;
            }
            token->value[len] = '\0';
            (*token_count)++;
        }
        else if (is_valid_identifier_start(input[pos])) {
            token->type = TOKEN_IDENTIFIER;
            int len = 0;
            while (is_valid_identifier_char(input[pos]) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = input[pos++];
                column++;
            }
            token->value[len] = '\0';
            (*token_count)++;
        }
        else if (is_operator_char(input[pos])) {
            token->type = TOKEN_OPERATOR;
            int len = 0;
            while (is_operator_char(input[pos]) && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = input[pos++];
                column++;
            }
            token->value[len] = '\0';
            (*token_count)++;
        }
        else if (input[pos] == '"') {
            token->type = TOKEN_STRING;
            int len = 0;
            token->value[len++] = input[pos++];
            column++;
            
            while (input[pos] != '"' && input[pos] != '\0' && len < MAX_TOKEN_LEN - 1) {
                token->value[len++] = input[pos++];
                column++;
            }
            
            if (input[pos] == '"') {
                token->value[len++] = input[pos++];
                column++;
                token->value[len] = '\0';
                (*token_count)++;
            } else {
                return 0;
            }
        }
        else {
            return 0;
        }
    }
    
    if (*token_count < max_tokens) {
        tokens[*token_count].type = TOKEN_EOF;
        tokens[*token_count].value[0] = '\0';
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        (*token_count)++;
    }
    
    return 1;
}

void print_token(const struct Token* token) {
    if (token == NULL) return;
    
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    
    printf("\n");
}

void print_tokens(const struct Token* tokens, int token_count) {
    if (tokens == NULL || token_count <= 0) return;
    
    printf("Tokenized output:\n");
    for (int i = 0; i < token_count; i++) {
        print_token(&tokens[i]);
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    if (!tokenize(input, tokens, MAX_TOKENS, &token_count