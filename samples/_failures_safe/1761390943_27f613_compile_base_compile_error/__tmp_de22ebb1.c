//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_PUNCTUATION,
    TOKEN_KEYWORD,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
    int line;
    int column;
} Token;

const char* keywords[] = {
    "if", "else", "while", "for", "return", "int", "float", "char", "void"
};
const int num_keywords = 9;

int is_keyword(const char* str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!&|", c) != NULL;
}

int is_punctuation_char(char c) {
    return strchr("(){}[],;:", c) != NULL;
}

void tokenize(const char* input, Token* tokens, int* token_count) {
    int i = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[i] != '\0' && *token_count < MAX_TOKENS - 1) {
        if (isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
            continue;
        }
        
        Token* token = &tokens[*token_count];
        token->line = line;
        token->column = column;
        
        if (isalpha(input[i]) || input[i] == '_') {
            int j = 0;
            while ((isalnum(input[i]) || input[i] == '_') && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            token->type = is_keyword(token->value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        }
        else if (isdigit(input[i])) {
            int j = 0;
            while (isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
            }
            if (input[i] == '.' && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
                while (isdigit(input[i]) && j < MAX_TOKEN_LEN - 1) {
                    token->value[j++] = input[i++];
                    column++;
                }
            }
            token->value[j] = '\0';
            token->type = TOKEN_NUMBER;
        }
        else if (input[i] == '"') {
            int j = 0;
            token->value[j++] = input[i++];
            column++;
            while (input[i] != '"' && input[i] != '\0' && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
            }
            if (input[i] == '"') {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            token->type = TOKEN_STRING;
        }
        else if (is_operator_char(input[i])) {
            int j = 0;
            token->value[j++] = input[i++];
            column++;
            if (is_operator_char(input[i]) && j < MAX_TOKEN_LEN - 1) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            token->type = TOKEN_OPERATOR;
        }
        else if (is_punctuation_char(input[i])) {
            token->value[0] = input[i++];
            token->value[1] = '\0';
            column++;
            token->type = TOKEN_PUNCTUATION;
        }
        else {
            i++;
            column++;
            continue;
        }
        
        (*token_count)++;
    }
    
    tokens[*token_count].type = TOKEN_EOF;
    tokens[*token_count].value[0] = '\0';
    tokens[*token_count].line = line;
    tokens[*token_count].column = column;
    (*token_count)++;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[4096];
    printf("Enter code to tokenize (max 4095 chars):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error