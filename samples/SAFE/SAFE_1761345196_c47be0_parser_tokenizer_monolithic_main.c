//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 50

enum TokenType { NUMBER, IDENTIFIER, OPERATOR, PUNCTUATION, END };

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_LEN];
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_punctuation_char(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

int tokenize(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    int pos = 0;
    int len = strlen(input);
    
    while (pos < len && token_count < max_tokens) {
        while (pos < len && isspace(input[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        if (isdigit(input[pos])) {
            int start = pos;
            while (pos < len && isdigit(input[pos])) {
                pos++;
            }
            if (pos - start >= MAX_TOKEN_LEN) {
                return -1;
            }
            tokens[token_count].type = NUMBER;
            strncpy(tokens[token_count].value, &input[start], pos - start);
            tokens[token_count].value[pos - start] = '\0';
            token_count++;
        } else if (isalpha(input[pos]) || input[pos] == '_') {
            int start = pos;
            while (pos < len && (isalnum(input[pos]) || input[pos] == '_')) {
                pos++;
            }
            if (pos - start >= MAX_TOKEN_LEN) {
                return -1;
            }
            tokens[token_count].type = IDENTIFIER;
            strncpy(tokens[token_count].value, &input[start], pos - start);
            tokens[token_count].value[pos - start] = '\0';
            token_count++;
        } else if (is_operator_char(input[pos])) {
            if (pos + 1 < len && is_operator_char(input[pos + 1])) {
                if (pos + 1 - pos >= MAX_TOKEN_LEN) {
                    return -1;
                }
                tokens[token_count].type = OPERATOR;
                tokens[token_count].value[0] = input[pos];
                tokens[token_count].value[1] = input[pos + 1];
                tokens[token_count].value[2] = '\0';
                pos += 2;
            } else {
                if (1 >= MAX_TOKEN_LEN) {
                    return -1;
                }
                tokens[token_count].type = OPERATOR;
                tokens[token_count].value[0] = input[pos];
                tokens[token_count].value[1] = '\0';
                pos++;
            }
            token_count++;
        } else if (is_punctuation_char(input[pos])) {
            if (1 >= MAX_TOKEN_LEN) {
                return -1;
            }
            tokens[token_count].type = PUNCTUATION;
            tokens[token_count].value[0] = input[pos];
            tokens[token_count].value[1] = '\0';
            pos++;
            token_count++;
        } else {
            return -1;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = END;
        strcpy(tokens[token_count].value, "END");
        token_count++;
    }
    
    return token_count;
}

const char* token_type_to_string(enum TokenType type) {
    switch (type) {
        case NUMBER: return "NUMBER";
        case IDENTIFIER: return "IDENTIFIER";
        case OPERATOR: return "OPERATOR";
        case PUNCTUATION: return "PUNCTUATION";
        case END: return "END";
        default: return "UNKNOWN";
    }
}

int main() {
    char input[1024];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter an expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    if (token_count == -1) {
        fprintf(stderr, "Error: Input contains invalid tokens or token too long\n");
        return 1;
    }
    
    printf("Tokens found:\n");
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type=%s, Value='%s'\n", 
               i + 1, token_type_to_string(tokens[i].type), tokens[i].value);
    }
    
    return 0;
}