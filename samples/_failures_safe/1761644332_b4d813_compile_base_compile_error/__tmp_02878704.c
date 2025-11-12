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

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    int token_count = 0;
    int line = 1;
    int column = 1;
    const char* ptr = input;
    
    while (*ptr != '\0' && token_count < max_tokens) {
        while (isspace(*ptr)) {
            if (*ptr == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            ptr++;
        }
        
        if (*ptr == '\0') break;
        
        struct Token* token = &tokens[token_count];
        token->line = line;
        token->column = column;
        token->value[0] = '\0';
        
        if (isalpha(*ptr) || *ptr == '_') {
            token->type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(*ptr) || *ptr == '_') && i < 63) {
                token->value[i++] = *ptr++;
                column++;
            }
            token->value[i] = '\0';
        } else if (isdigit(*ptr)) {
            token->type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(*ptr) && i < 63) {
                token->value[i++] = *ptr++;
                column++;
            }
            token->value[i] = '\0';
        } else if (*ptr == '"') {
            token->type = TOKEN_STRING;
            int i = 0;
            ptr++;
            column++;
            while (*ptr != '"' && *ptr != '\0' && i < 63) {
                if (*ptr == '\\' && ptr[1] != '\0') {
                    token->value[i++] = *ptr++;
                    column++;
                    if (i < 63) {
                        token->value[i++] = *ptr++;
                        column++;
                    }
                } else {
                    token->value[i++] = *ptr++;
                    column++;
                }
            }
            token->value[i] = '\0';
            if (*ptr == '"') {
                ptr++;
                column++;
            } else {
                token->type = TOKEN_ERROR;
            }
        } else if (is_operator_char(*ptr)) {
            token->type = TOKEN_OPERATOR;
            int op_len = get_operator_length(ptr);
            if (op_len > 0 && op_len < 64) {
                strncpy(token->value, ptr, op_len);
                token->value[op_len] = '\0';
                ptr += op_len;
                column += op_len;
            } else {
                token->type = TOKEN_ERROR;
                token->value[0] = *ptr++;
                token->value[1] = '\0';
                column++;
            }
        } else {
            token->type = TOKEN_ERROR;
            token->value[0] = *ptr++;
            token->value[1] = '\0';
            column++;
        }
        
        token_count++;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        token_count++;
    }
    
    return token_count;
}

void print_token(struct Token token) {
    const char* type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" '%s'", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading