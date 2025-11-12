//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define DELIMITERS " \t\n\r"
#define IS_OPERATOR(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')')

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
size_t token_count = 0;
size_t current_token = 0;

void tokenize(const char* input) {
    if (input == NULL) return;
    
    const char* ptr = input;
    token_count = 0;
    
    while (*ptr != '\0' && token_count < MAX_TOKENS) {
        while (isspace((unsigned char)*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        if (IS_OPERATOR(*ptr)) {
            if (token_count < MAX_TOKENS) {
                tokens[token_count].type = TOKEN_OPERATOR;
                tokens[token_count].value[0] = *ptr;
                tokens[token_count].value[1] = '\0';
                token_count++;
            }
            ptr++;
            continue;
        }
        
        if (isdigit((unsigned char)*ptr)) {
            size_t len = 0;
            while (isdigit((unsigned char)*ptr) && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
            continue;
        }
        
        if (isalpha((unsigned char)*ptr) || *ptr == '_') {
            size_t len = 0;
            while ((isalnum((unsigned char)*ptr) || *ptr == '_') && len < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[len++] = *ptr++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_IDENTIFIER;
            token_count++;
            continue;
        }
        
        ptr++;
    }
    
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = TOKEN_END;
        tokens[token_count].value[0] = '\0';
    }
}

Token get_next_token(void) {
    if (current_token < token_count) {
        return tokens[current_token++];
    }
    Token end_token = {TOKEN_END, ""};
    return end_token;
}

void reset_parser(void) {
    current_token = 0;
}

int parse_expression(void);

int parse_factor(void) {
    Token token = get_next_token();
    
    if (token.type == TOKEN_NUMBER) {
        return atoi(token.value);
    }
    
    if (token.type == TOKEN_IDENTIFIER) {
        return 0;
    }
    
    if (strcmp(token.value, "(") == 0) {
        int result = parse_expression();
        token = get_next_token();
        if (strcmp(token.value, ")") != 0) {
            return 0;
        }
        return result;
    }
    
    return 0;
}

int parse_term(void) {
    int result = parse_factor();
    
    while (1) {
        Token token = get_next_token();
        
        if (token.type != TOKEN_OPERATOR) {
            current_token--;
            break;
        }
        
        if (strcmp(token.value, "*") == 0) {
            result *= parse_factor();
        } else if (strcmp(token.value, "/") == 0) {
            int divisor = parse_factor();
            if (divisor != 0) {
                result /= divisor;
            }
        } else {
            current_token--;
            break;
        }
    }
    
    return result;
}

int parse_expression(void) {
    int result = parse_term();
    
    while (1) {
        Token token = get_next_token();
        
        if (token.type != TOKEN_OPERATOR) {
            current_token--;
            break;
        }
        
        if (strcmp(token.value, "+") == 0) {
            result += parse_term();
        } else if (strcmp(token.value, "-") == 0) {
            result -= parse_term();
        } else {
            current_token--;
            break;
        }
    }
    
    return result;
}

int main(void) {
    char input[1024];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    tokenize(input);
    
    if (token_count == 0) {
        printf("No tokens found\n");
        return 1;
    }
    
    printf("Tokens:\n");
    for (size_t i = 0; i < token_count && tokens[i].type != TOKEN_END; i++) {