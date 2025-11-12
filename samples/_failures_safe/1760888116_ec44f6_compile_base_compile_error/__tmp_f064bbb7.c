//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: parser_tokenizer
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

struct Token* tokenize(const char* input, int* token_count) {
    static struct Token tokens[MAX_TOKENS];
    *token_count = 0;
    int line = 1;
    int column = 1;
    int i = 0;
    int input_len = strlen(input);
    
    while (i < input_len && *token_count < MAX_TOKENS - 1) {
        while (i < input_len && isspace(input[i])) {
            if (input[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
        }
        
        if (i >= input_len) break;
        
        struct Token* token = &tokens[*token_count];
        token->line = line;
        token->column = column;
        
        if (isdigit(input[i])) {
            token->type = TOKEN_NUMBER;
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && isdigit(input[i])) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
        } else if (isalpha(input[i]) || input[i] == '_') {
            token->type = TOKEN_IDENTIFIER;
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && 
                   (isalnum(input[i]) || input[i] == '_')) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
        } else if (input[i] == '"') {
            token->type = TOKEN_STRING;
            int j = 0;
            i++;
            column++;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && input[i] != '"') {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
            if (i < input_len && input[i] == '"') {
                i++;
                column++;
            }
        } else if (is_operator_char(input[i])) {
            token->type = TOKEN_OPERATOR;
            int j = 0;
            while (i < input_len && j < MAX_TOKEN_LEN - 1 && is_operator_char(input[i])) {
                token->value[j++] = input[i++];
                column++;
            }
            token->value[j] = '\0';
        } else {
            i++;
            column++;
            continue;
        }
        
        (*token_count)++;
    }
    
    tokens[*token_count].type = TOKEN_EOF;
    strcpy(tokens[*token_count].value, "EOF");
    tokens[*token_count].line = line;
    tokens[*token_count].column = column;
    (*token_count)++;
    
    return tokens;
}

void print_token(const struct Token* token) {
    const char* type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF"
    };
    printf("Type: %-12s Value: %-15s Line: %2d Column: %2d\n",
           type_names[token->type], token->value, token->line, token->column);
}

void parse_expression(struct Token* tokens, int token_count, int* pos) {
    if (*pos >= token_count) return;
    
    struct Token* token = &tokens[*pos];
    
    if (token->type == TOKEN_NUMBER || token->type == TOKEN_IDENTIFIER) {
        printf("Parsing primary: %s\n", token->value);
        (*pos)++;
        
        if (*pos < token_count && tokens[*pos].type == TOKEN_OPERATOR) {
            printf("Parsing operator: %s\n", tokens[*pos].value);
            (*pos)++;
            parse_expression(tokens, token_count, pos);
        }
    } else if (token->type == TOKEN_STRING) {
        printf("Parsing string: \"%s\"\n", token->value);
        (*pos)++;
    }
}

void parse_tokens(struct Token* tokens, int token_count) {
    int pos = 0;
    printf("Starting parse...\n");
    
    while (pos < token_count && tokens[pos].type != TOKEN_EOF) {
        parse_expression(tokens, token_count, &pos);
    }
    
    printf("Parse complete.\n");
}

int main() {
    char input[1024];
    printf("Enter expression to tokenize and parse: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0