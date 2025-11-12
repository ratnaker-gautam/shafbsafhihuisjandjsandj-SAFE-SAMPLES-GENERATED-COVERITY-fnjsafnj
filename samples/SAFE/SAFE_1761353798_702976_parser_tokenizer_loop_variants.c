//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int is_punctuation(char c) {
    return c == '(' || c == ')' || c == ',' || c == ';';
}

int parse_tokens(const char* input, struct Token* tokens, int max_tokens) {
    int token_count = 0;
    const char* p = input;
    
    while (*p != '\0' && token_count < max_tokens) {
        while (isspace(*p)) p++;
        if (*p == '\0') break;
        
        if (isdigit(*p)) {
            int i = 0;
            while (isdigit(*p) && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = NUMBER;
            token_count++;
        } else if (isalpha(*p)) {
            int i = 0;
            while ((isalnum(*p) || *p == '_') && i < MAX_TOKEN_LEN - 1) {
                tokens[token_count].value[i++] = *p++;
            }
            tokens[token_count].value[i] = '\0';
            tokens[token_count].type = IDENTIFIER;
            token_count++;
        } else if (is_operator(*p)) {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = OPERATOR;
            token_count++;
        } else if (is_punctuation(*p)) {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = PUNCTUATION;
            token_count++;
        } else {
            break;
        }
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = END;
        tokens[token_count].value[0] = '\0';
    }
    
    return token_count;
}

void print_token_type(enum TokenType type) {
    const char* names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "PUNCTUATION", "END"};
    if (type >= NUMBER && type <= END) {
        printf("%s", names[type]);
    }
}

int main() {
    char input[512];
    struct Token tokens[MAX_TOKENS];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int token_count = parse_tokens(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Token list:\n");
    
    int i = 0;
    while (i < token_count && tokens[i].type != END) {
        printf("  ");
        print_token_type(tokens[i].type);
        printf(": '%s'\n", tokens[i].value);
        i++;
    }
    
    for (int j = 0; j < token_count; j++) {
        if (tokens[j].type == END) {
            printf("  END\n");
            break;
        }
    }
    
    return 0;
}