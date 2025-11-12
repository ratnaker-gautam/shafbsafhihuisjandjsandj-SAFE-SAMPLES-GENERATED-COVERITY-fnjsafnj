//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
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
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN + 1];
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

void tokenize(const char* input, struct Token* tokens, int* token_count) {
    int i = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[i] != '\0' && *token_count < MAX_TOKENS) {
        while (isspace((unsigned char)input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
        }
        
        if (input[i] == '\0') break;
        
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        
        if (isdigit((unsigned char)input[i])) {
            int j = 0;
            while (isdigit((unsigned char)input[i]) && j < MAX_TOKEN_LEN) {
                tokens[*token_count].value[j++] = input[i++];
                column++;
            }
            tokens[*token_count].value[j] = '\0';
            tokens[*token_count].type = TOKEN_NUMBER;
            (*token_count)++;
        }
        else if (is_valid_identifier_start(input[i])) {
            int j = 0;
            while (is_valid_identifier_char(input[i]) && j < MAX_TOKEN_LEN) {
                tokens[*token_count].value[j++] = input[i++];
                column++;
            }
            tokens[*token_count].value[j] = '\0';
            tokens[*token_count].type = TOKEN_IDENTIFIER;
            (*token_count)++;
        }
        else if (is_operator_char(input[i])) {
            int j = 0;
            while (is_operator_char(input[i]) && j < MAX_TOKEN_LEN) {
                tokens[*token_count].value[j++] = input[i++];
                column++;
            }
            tokens[*token_count].value[j] = '\0';
            tokens[*token_count].type = TOKEN_OPERATOR;
            (*token_count)++;
        }
        else if (input[i] == '"') {
            int j = 0;
            tokens[*token_count].value[j++] = input[i++];
            column++;
            
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN) {
                tokens[*token_count].value[j++] = input[i++];
                column++;
            }
            
            if (input[i] == '"') {
                tokens[*token_count].value[j++] = input[i++];
                column++;
                tokens[*token_count].value[j] = '\0';
                tokens[*token_count].type = TOKEN_STRING;
                (*token_count)++;
            } else {
                tokens[*token_count].value[j] = '\0';
                tokens[*token_count].type = TOKEN_ERROR;
                (*token_count)++;
                break;
            }
        }
        else {
            tokens[*token_count].value[0] = input[i++];
            tokens[*token_count].value[1] = '\0';
            tokens[*token_count].type = TOKEN_ERROR;
            (*token_count)++;
            column++;
            break;
        }
    }
    
    if (*token_count < MAX_TOKENS) {
        tokens[*token_count].type = TOKEN_EOF;
        tokens[*token_count].value[0] = '\0';
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        (*token_count)++;
    }
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    int token_count = 0;
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    tokenize(input,