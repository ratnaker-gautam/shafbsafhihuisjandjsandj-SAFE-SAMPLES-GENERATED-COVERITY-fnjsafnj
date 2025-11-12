//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_STRING, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int get_operator_length(const char* str) {
    if (str[0] == '\0') return 0;
    if (str[1] == '\0') return 1;
    if ((str[0] == '&' && str[1] == '&') || (str[0] == '|' && str[1] == '|') || 
        (str[0] == '=' && str[1] == '=') || (str[0] == '!' && str[1] == '=') ||
        (str[0] == '<' && str[1] == '=') || (str[0] == '>' && str[1] == '=')) {
        return 2;
    }
    return 1;
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    struct Token tokens[256];
    int token_count = 0;
    int line = 1;
    int column = 1;
    const char* pos = input;
    
    while (*pos != '\0' && token_count < 256) {
        while (isspace((unsigned char)*pos)) {
            if (*pos == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (*pos == '\0') break;
        
        struct Token* token = &tokens[token_count];
        token->line = line;
        token->column = column;
        token->value[0] = '\0';
        
        if (isalpha((unsigned char)*pos) || *pos == '_') {
            token->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum((unsigned char)*pos) || *pos == '_') && i < 63) {
                token->value[i++] = *pos++;
                column++;
            }
            token->value[i] = '\0';
        } else if (isdigit((unsigned char)*pos)) {
            token->type = TOKEN_NUMBER;
            int i = 0;
            int has_dot = 0;
            while ((isdigit((unsigned char)*pos) || (*pos == '.' && !has_dot)) && i < 63) {
                if (*pos == '.') has_dot = 1;
                token->value[i++] = *pos++;
                column++;
            }
            token->value[i] = '\0';
        } else if (*pos == '"') {
            token->type = TOKEN_STRING;
            int i = 0;
            pos++;
            column++;
            while (*pos != '"' && *pos != '\0' && i < 63) {
                if (*pos == '\\' && *(pos + 1) != '\0') {
                    token->value[i++] = *pos++;
                    column++;
                    if (i < 63) {
                        token->value[i++] = *pos++;
                        column++;
                    }
                } else {
                    token->value[i++] = *pos++;
                    column++;
                }
            }
            token->value[i] = '\0';
            if (*pos == '"') {
                pos++;
                column++;
            } else {
                token->type = TOKEN_ERROR;
            }
        } else if (is_operator_char(*pos)) {
            token->type = TOKEN_OPERATOR;
            int op_len = get_operator_length(pos);
            if (op_len > 0 && op_len < 64) {
                strncpy(token->value, pos, op_len);
                token->value[op_len] = '\0';
                pos += op_len;
                column += op_len;
            } else {
                token->type = TOKEN_ERROR;
                token->value[0] = *pos++;
                token->value[1] = '\0';
                column++;
            }
        } else {
            token->type = TOKEN_ERROR;
            token->value[0] = *pos++;
            token->value[1] = '\0';
            column++;
        }
        
        token_count++;
    }
    
    if (token_count < 256) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }
    
    printf("Tokens