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
            char buffer[64] = {0};
            int buf_pos = 0;
            int start_column = column;
            
            while (isdigit(input[pos]) && buf_pos < sizeof(buffer) - 1) {
                buffer[buf_pos++] = input[pos++];
                column++;
            }
            
            if (token_count < max_tokens) {
                token_init(&tokens[token_count++], TOKEN_NUMBER, buffer, line, start_column);
            }
            continue;
        }
        
        if (input[pos] == '"') {
            char buffer[64] = {0};
            int buf_pos = 0;
            int start_column = column;
            
            pos++;
            column++;
            
            while (input[pos] != '"' && input[pos] != '\0' && buf_pos < sizeof(buffer) - 1) {
                buffer[buf_pos++] = input[pos++];
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
                token_init(&tokens[token_count++], TOKEN_STRING, buffer, line, start_column);
            }
            continue;
        }
        
        if (is_valid_identifier_start(input[pos])) {
            char buffer[64] = {0};
            int buf_pos = 0;
            int start_column = column;
            
            while (is_valid_identifier_char(input[pos]) && buf_pos < sizeof(buffer) - 1) {
                buffer[buf_pos++] = input[pos++];
                column++;
            }
            
            if (token_count < max_tokens) {
                token_init(&tokens[token_count++], TOKEN_IDENTIFIER, buffer, line, start_column);
            }
            continue;
        }
        
        if (is_operator_char(input[pos])) {
            char buffer[3] = {0};
            int buf_pos = 0;
            int start_column = column;
            
            buffer[buf_pos++] = input[pos++];
            column++;
            
            if (is_operator_char(input[pos]) && buf_pos < sizeof(buffer) - 1) {
                buffer[buf_pos++] = input[pos++];
                column++;
            }
            
            if (token_count < max_tokens) {
                token_init(&tokens[token_count++], TOKEN_OPERATOR, buffer, line, start_column);
            }
            continue;
        }
        
        char error_msg[32];
        snprintf(error_msg, sizeof(error_msg), "Unexpected character '%c'", input[pos]);
        if (token_count < max_tokens) {
            token_init(&tokens[token_count++], TOKEN_ERROR, error_msg, line, column);
        }
        pos++;
        column++;
    }
    
    if (token_count < max_tokens) {
        token_init(&tokens[token_count++], TOKEN_EOF, "", line, column);
    }
    
    return token_count;
}

const char *token_type_to_string(enum TokenType type) {
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
        fprintf(stderr, "