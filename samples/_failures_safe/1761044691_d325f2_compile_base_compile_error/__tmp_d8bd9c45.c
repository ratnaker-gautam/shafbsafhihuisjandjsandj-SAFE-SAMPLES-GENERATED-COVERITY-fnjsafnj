//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
           c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}

int parse_operator(const char *input, int pos, char *output) {
    int len = 0;
    char c = input[pos];
    
    if (is_operator_char(c)) {
        output[len++] = c;
        char next = input[pos + 1];
        
        if ((c == '&' && next == '&') || (c == '|' && next == '|') ||
            (c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=')) {
            output[len++] = next;
        }
    }
    
    output[len] = '\0';
    return len > 0 ? len : 0;
}

int parse_string(const char *input, int pos, char *output) {
    if (input[pos] != '"') return 0;
    
    int len = 0;
    output[len++] = '"';
    pos++;
    
    while (input[pos] != '\0' && input[pos] != '"') {
        if (len >= 31) break;
        output[len++] = input[pos++];
    }
    
    if (input[pos] == '"') {
        output[len++] = '"';
        output[len] = '\0';
        return len;
    }
    
    return 0;
}

int parse_number(const char *input, int pos, char *output) {
    if (!isdigit(input[pos])) return 0;
    
    int len = 0;
    while (isdigit(input[pos]) && len < 31) {
        output[len++] = input[pos++];
    }
    output[len] = '\0';
    return len;
}

int parse_identifier(const char *input, int pos, char *output) {
    if (!isalpha(input[pos]) && input[pos] != '_') return 0;
    
    int len = 0;
    while ((isalnum(input[pos]) || input[pos] == '_') && len < 31) {
        output[len++] = input[pos++];
    }
    output[len] = '\0';
    return len;
}

void tokenize(const char *input, struct Token *tokens, int max_tokens, int *token_count) {
    int pos = 0;
    int line = 1;
    int column = 1;
    *token_count = 0;
    
    while (input[pos] != '\0' && *token_count < max_tokens - 1) {
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
        
        struct Token *token = &tokens[*token_count];
        token->line = line;
        token->column = column;
        
        int advance = 0;
        
        if ((advance = parse_string(input, pos, token->value)) > 0) {
            token->type = TOKEN_STRING;
        } else if ((advance = parse_number(input, pos, token->value)) > 0) {
            token->type = TOKEN_NUMBER;
        } else if ((advance = parse_operator(input, pos, token->value)) > 0) {
            token->type = TOKEN_OPERATOR;
        } else if ((advance = parse_identifier(input, pos, token->value)) > 0) {
            token->type = TOKEN_IDENTIFIER;
        } else {
            token->type = TOKEN_INVALID;
            token->value[0] = input[pos];
            token->value[1] = '\0';
            advance = 1;
        }
        
        column += advance;
        pos += advance;
        (*token_count)++;
    }
    
    if (*token_count < max_tokens) {
        tokens[*token_count].type = TOKEN_EOF;
        strcpy(tokens[*token_count].value, "EOF");
        tokens[*token_count].line = line;
        tokens[*token_count].column = column;
        (*token_count)++;
    }
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "IDENTIFIER", "NUMBER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
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