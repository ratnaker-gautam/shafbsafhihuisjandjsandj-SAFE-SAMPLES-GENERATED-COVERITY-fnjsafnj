//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define STATE_START 0
#define STATE_IDENTIFIER 1
#define STATE_NUMBER 2
#define STATE_OPERATOR 3
#define STATE_ERROR 4

typedef struct {
    int state;
    char *input;
    char *current;
    int token_count;
} lexer_t;

void lexer_init(lexer_t *lex, char *input) {
    if (lex == NULL || input == NULL) return;
    lex->state = STATE_START;
    lex->input = input;
    lex->current = input;
    lex->token_count = 0;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void process_char(lexer_t *lex) {
    if (lex == NULL || *(lex->current) == '\0') return;
    
    char c = *(lex->current);
    
    switch (lex->state) {
        case STATE_START:
            if (isalpha(c)) {
                lex->state = STATE_IDENTIFIER;
            } else if (isdigit(c)) {
                lex->state = STATE_NUMBER;
            } else if (is_operator(c)) {
                lex->state = STATE_OPERATOR;
            } else if (!isspace(c)) {
                lex->state = STATE_ERROR;
            }
            break;
            
        case STATE_IDENTIFIER:
            if (!isalnum(c)) {
                lex->token_count++;
                lex->state = STATE_START;
                lex->current--;
            }
            break;
            
        case STATE_NUMBER:
            if (!isdigit(c)) {
                lex->token_count++;
                lex->state = STATE_START;
                lex->current--;
            }
            break;
            
        case STATE_OPERATOR:
            lex->token_count++;
            lex->state = STATE_START;
            break;
            
        case STATE_ERROR:
            if (isspace(c)) {
                lex->state = STATE_START;
            }
            break;
    }
}

int lex_input(lexer_t *lex) {
    if (lex == NULL || lex->input == NULL) return -1;
    
    while (*(lex->current) != '\0') {
        process_char(lex);
        lex->current++;
    }
    
    if (lex->state != STATE_START && lex->state != STATE_ERROR) {
        lex->token_count++;
    }
    
    return lex->token_count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1) {
        printf("Empty input\n");
        return 0;
    }
    
    lexer_t lexer;
    lexer_init(&lexer, input);
    
    int token_count = lex_input(&lexer);
    
    if (token_count >= 0) {
        printf("Found %d tokens\n", token_count);
    } else {
        printf("Error during tokenization\n");
        return 1;
    }
    
    return 0;
}