//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[32];
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
    int line = 1;
    int column = 1;
    int token_count = 0;
    const char *p = input;
    
    while (*p && token_count < max_tokens) {
        while (isspace(*p)) {
            if (*p == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            p++;
        }
        
        if (!*p) break;
        
        int start_column = column;
        
        if (isdigit(*p)) {
            char buffer[32] = {0};
            int i = 0;
            while (isdigit(*p) && i < sizeof(buffer) - 1) {
                buffer[i++] = *p;
                p++;
                column++;
            }
            token_init(&tokens[token_count++], TOKEN_NUMBER, buffer, line, start_column);
        }
        else if (*p == '"') {
            char buffer[32] = {0};
            int i = 0;
            p++;
            column++;
            while (*p && *p != '"' && i < sizeof(buffer) - 1) {
                buffer[i++] = *p;
                p++;
                column++;
            }
            if (*p == '"') {
                p++;
                column++;
                token_init(&tokens[token_count++], TOKEN_STRING, buffer, line, start_column);
            } else {
                token_init(&tokens[token_count++], TOKEN_ERROR, "Unterminated string", line, start_column);
            }
        }
        else if (is_valid_identifier_start(*p)) {
            char buffer[32] = {0};
            int i = 0;
            while (is_valid_identifier_char(*p) && i < sizeof(buffer) - 1) {
                buffer[i++] = *p;
                p++;
                column++;
            }
            token_init(&tokens[token_count++], TOKEN_IDENTIFIER, buffer, line, start_column);
        }
        else if (is_operator_char(*p)) {
            char buffer[3] = {0};
            buffer[0] = *p;
            p++;
            column++;
            if (is_operator_char(*p)) {
                buffer[1] = *p;
                p++;
                column++;
            }
            token_init(&tokens[token_count++], TOKEN_OPERATOR, buffer, line, start_column);
        }
        else {
            char buffer[2] = {*p, 0};
            token_init(&tokens[token_count++], TOKEN_ERROR, buffer, line, start_column);
            p++;
            column++;
        }
    }
    
    if (token_count < max_tokens) {
        token_init(&tokens[token_count], TOKEN_EOF, "", line, column);
    }
    
    return token_count;
}

const char* token_type_name(enum TokenType type) {
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
    printf("Enter text to tokenize (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len <= 1) {
        printf("No input provided\n");
        return 1;
    }
    
    struct Token tokens[256];
    int token_count = tokenize(input, tokens, 255);
    
    printf("\nTokenized output:\n");
    printf("Line Col Type       Value\n");
    printf("---- --- ---------- --------------------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%3d %3d %-10s %s