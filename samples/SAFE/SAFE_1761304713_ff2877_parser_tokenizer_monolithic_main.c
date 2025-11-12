//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType { TOKEN_NUMBER, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_EOF, TOKEN_ERROR };

struct Token {
    enum TokenType type;
    char value[64];
    int line;
    int column;
};

int is_symbol_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

int tokenize(const char *input, struct Token *tokens, int max_tokens) {
    int pos = 0;
    int line = 1;
    int column = 1;
    int token_count = 0;
    
    while (input[pos] != '\0' && token_count < max_tokens) {
        while (isspace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        
        if (input[pos] == '\0') break;
        
        tokens[token_count].line = line;
        tokens[token_count].column = column;
        
        if (isdigit(input[pos])) {
            tokens[token_count].type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(input[pos]) && i < 63) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            tokens[token_count].value[i] = '\0';
        } else if (input[pos] == '"') {
            tokens[token_count].type = TOKEN_STRING;
            int i = 0;
            pos++;
            column++;
            while (input[pos] != '"' && input[pos] != '\0' && i < 63) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            if (input[pos] == '"') {
                pos++;
                column++;
            } else {
                tokens[token_count].type = TOKEN_ERROR;
            }
            tokens[token_count].value[i] = '\0';
        } else if (is_symbol_char(input[pos])) {
            tokens[token_count].type = TOKEN_SYMBOL;
            int i = 0;
            while (is_symbol_char(input[pos]) && i < 63) {
                tokens[token_count].value[i++] = input[pos++];
                column++;
            }
            tokens[token_count].value[i] = '\0';
        } else {
            tokens[token_count].type = TOKEN_ERROR;
            tokens[token_count].value[0] = input[pos++];
            tokens[token_count].value[1] = '\0';
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

const char* token_type_name(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_SYMBOL: return "SYMBOL";
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
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    struct Token tokens[100];
    int token_count = tokenize(input, tokens, 100);
    
    if (token_count <= 0) {
        fprintf(stderr, "No tokens generated\n");
        return 1;
    }
    
    printf("\nTokens:\n");
    printf("%-8s %-10s %-8s %s\n", "Line", "Column", "Type", "Value");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < token_count; i++) {
        printf("%-8d %-10d %-8s \"%s\"\n", 
               tokens[i].line, 
               tokens[i].column, 
               token_type_name(tokens[i].type), 
               tokens[i].value);
    }
    
    return 0;
}