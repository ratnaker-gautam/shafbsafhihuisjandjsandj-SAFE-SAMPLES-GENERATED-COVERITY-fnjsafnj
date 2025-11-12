//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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

void skip_whitespace(const char **input) {
    while (**input && isspace(**input)) {
        (*input)++;
    }
}

int parse_number(const char **input, struct Token *token) {
    int i = 0;
    while (**input && isdigit(**input)) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = NUMBER;
    return 1;
}

int parse_identifier(const char **input, struct Token *token) {
    int i = 0;
    while (**input && (isalnum(**input) || **input == '_')) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = IDENTIFIER;
    return 1;
}

int parse_operator(const char **input, struct Token *token) {
    int i = 0;
    while (**input && is_operator_char(**input)) {
        if (i >= MAX_TOKEN_LEN - 1) return 0;
        token->value[i++] = **input;
        (*input)++;
    }
    token->value[i] = '\0';
    token->type = OPERATOR;
    return 1;
}

int parse_punctuation(const char **input, struct Token *token) {
    if (is_punctuation_char(**input)) {
        token->value[0] = **input;
        token->value[1] = '\0';
        token->type = PUNCTUATION;
        (*input)++;
        return 1;
    }
    return 0;
}

int tokenize(const char *input, struct Token tokens[], int max_tokens) {
    int count = 0;
    const char *ptr = input;
    
    while (*ptr && count < max_tokens) {
        skip_whitespace(&ptr);
        if (!*ptr) break;
        
        struct Token token;
        int success = 0;
        
        if (isdigit(*ptr)) {
            success = parse_number(&ptr, &token);
        } else if (isalpha(*ptr) || *ptr == '_') {
            success = parse_identifier(&ptr, &token);
        } else if (is_operator_char(*ptr)) {
            success = parse_operator(&ptr, &token);
        } else if (is_punctuation_char(*ptr)) {
            success = parse_punctuation(&ptr, &token);
        } else {
            break;
        }
        
        if (success) {
            tokens[count++] = token;
        } else {
            break;
        }
    }
    
    if (count < max_tokens) {
        tokens[count].type = END;
        tokens[count].value[0] = '\0';
    }
    
    return count;
}

void print_token_type(enum TokenType type) {
    const char *names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "PUNCTUATION", "END"};
    if (type >= NUMBER && type <= END) {
        printf("%s", names[type]);
    }
}

int main(void) {
    char input[1024];
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
    
    struct Token tokens[MAX_TOKENS];
    int token_count = tokenize(input, tokens, MAX_TOKENS);
    
    printf("Tokens found: %d\n", token_count);
    printf("Type\tValue\n");
    printf("----\t-----\n");
    
    int i = 0;
    while (i < token_count && tokens[i].type != END) {
        print_token_type(tokens[i].type);
        printf("\t%s\n", tokens[i].value);
        i++;
    }
    
    for (int j = 0; j < token_count; j++) {
        if (tokens[j].type == END) {
            printf("END\t(end of tokens)\n");
            break;
        }
    }
    
    return 0;
}