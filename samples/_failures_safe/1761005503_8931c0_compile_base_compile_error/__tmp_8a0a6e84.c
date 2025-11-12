//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

void token_init(struct Token *token, enum TokenType type, const char *value, int line, int column) {
    token->type = type;
    strncpy(token->value, value, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
    token->line = line;
    token->column = column;
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        if (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        if (isdigit(input[pos])) {
            char number[64] = {0};
            int num_len = 0;
            int start_column = column;
            
            while (isdigit(input[pos]) && num_len < sizeof(number) - 1) {
                number[num_len++] = input[pos++];
                column++;
            }
            
            if (token_count < max_tokens) {
                token_init(&tokens[token_count++], TOKEN_NUMBER, number, line, start_column);
            }
            continue;
        }
        
        if (input[pos] == '"') {
            char string[64] = {0};
            int str_len = 0;
            int start_column = column;
            
            pos++;
            column++;
            
            while (input[pos] != '"' && input[pos] != '\0' && str_len < sizeof(string) - 1) {
                string[str_len++] = input[pos++];
                column++;
            }
            
            if (input[pos] == '"') {
                pos++;
                column++;
            } else {
                if (token_count < max_tokens) {
                    token_init(&tokens[token_count++], TOKEN_ERROR, "Unterminated string", line, start_column);
                }
                break;
            }
            
            if (token_count < max_tokens) {
                token_init(&tokens[token_count++], TOKEN_STRING, string, line, start_column);
            }
            continue;
        }
        
        if (is_valid_identifier_start(input[pos])) {
            char identifier[64] = {0};
            int id_len = 0;
            int start_column = column;
            
            while (is_valid_identifier_char(input[pos]) && id_len < sizeof(identifier) - 1) {
                identifier[id_len++] = input[pos++];
                column++;
            }
            
            if (token_count < max_tokens) {
                token_init(&tokens[token_count++], TOKEN_IDENTIFIER, identifier, line, start_column);
            }
            continue;
        }
        
        if (is_operator_char(input[pos])) {
            char operator[3] = {0};
            int op_len = 0;
            int start_column = column;
            
            while (is_operator_char(input[pos]) && op_len < sizeof(operator) - 1) {
                operator[op_len++] = input[pos++];
                column++;
            }
            
            if (token_count < max_tokens) {
                token_init(&tokens[token_count++], TOKEN_OPERATOR, operator, line, start_column);
            }
            continue;
        }
        
        if (token_count < max_tokens) {
            char error[32];
            snprintf(error, sizeof(error), "Unexpected character '%c'", input[pos]);
            token_init(&tokens[token_count++], TOKEN_ERROR, error, line, column);
        }
        break;
    }
    
    if (token_count < max_tokens) {
        token_init(&tokens[token_count++], TOKEN_EOF, "", line, column);
    }
    
    return token_count;
}

const char *token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
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
    if (