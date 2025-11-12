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
    int current_state;
    char *input;
    size_t position;
    char token[MAX_INPUT_LEN];
    size_t token_len;
} Lexer;

void lexer_init(Lexer *lexer, char *input) {
    lexer->current_state = STATE_START;
    lexer->input = input;
    lexer->position = 0;
    lexer->token_len = 0;
    memset(lexer->token, 0, MAX_INPUT_LEN);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void process_char(Lexer *lexer) {
    char current = *(lexer->input + lexer->position);
    
    switch (lexer->current_state) {
        case STATE_START:
            if (isalpha(current)) {
                lexer->current_state = STATE_IDENTIFIER;
                *(lexer->token + lexer->token_len) = current;
                lexer->token_len++;
            } else if (isdigit(current)) {
                lexer->current_state = STATE_NUMBER;
                *(lexer->token + lexer->token_len) = current;
                lexer->token_len++;
            } else if (is_operator(current)) {
                lexer->current_state = STATE_OPERATOR;
                *(lexer->token + lexer->token_len) = current;
                lexer->token_len++;
            } else if (!isspace(current)) {
                lexer->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_IDENTIFIER:
            if (isalnum(current)) {
                *(lexer->token + lexer->token_len) = current;
                lexer->token_len++;
            } else {
                lexer->current_state = STATE_START;
            }
            break;
            
        case STATE_NUMBER:
            if (isdigit(current)) {
                *(lexer->token + lexer->token_len) = current;
                lexer->token_len++;
            } else {
                lexer->current_state = STATE_START;
            }
            break;
            
        case STATE_OPERATOR:
            lexer->current_state = STATE_START;
            break;
            
        case STATE_ERROR:
            break;
    }
}

void print_token(Lexer *lexer) {
    if (lexer->token_len > 0) {
        printf("Token: ");
        for (size_t i = 0; i < lexer->token_len; i++) {
            putchar(*(lexer->token + i));
        }
        printf(" (");
        switch (lexer->current_state) {
            case STATE_IDENTIFIER: printf("IDENTIFIER"); break;
            case STATE_NUMBER: printf("NUMBER"); break;
            case STATE_OPERATOR: printf("OPERATOR"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN");
        }
        printf(")\n");
    }
}

void reset_token(Lexer *lexer) {
    lexer->token_len = 0;
    memset(lexer->token, 0, MAX_INPUT_LEN);
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    Lexer lexer;
    
    printf("Enter expression: ");
    if (fgets(input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    lexer_init(&lexer, input_buffer);
    
    printf("Lexical analysis:\n");
    while (*(lexer.input + lexer.position) != '\0') {
        int prev_state = lexer.current_state;
        process_char(&lexer);
        
        if (lexer.current_state == STATE_START && prev_state != STATE_START) {
            print_token(&lexer);
            reset_token(&lexer);
        } else if (lexer.current_state == STATE_ERROR) {
            printf("Error: Invalid character '%c' at position %zu\n", 
                   *(lexer.input + lexer.position), lexer.position);
            return 1;
        }
        
        lexer.position++;
    }
    
    if (lexer.current_state != STATE_START) {
        print_token(&lexer);
    }
    
    return 0;
}