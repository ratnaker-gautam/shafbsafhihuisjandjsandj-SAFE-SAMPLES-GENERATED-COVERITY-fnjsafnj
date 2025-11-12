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
    char *current_pos;
    char token[MAX_INPUT_LEN];
} lexer_t;

void lexer_init(lexer_t *lex, char *input) {
    if (lex == NULL || input == NULL) return;
    lex->current_state = STATE_START;
    lex->input = input;
    lex->current_pos = input;
    memset(lex->token, 0, MAX_INPUT_LEN);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

int handle_start_state(lexer_t *lex) {
    if (lex == NULL || *(lex->current_pos) == '\0') return 0;
    
    char c = *(lex->current_pos);
    if (isalpha(c)) {
        lex->current_state = STATE_IDENTIFIER;
        return 1;
    } else if (isdigit(c)) {
        lex->current_state = STATE_NUMBER;
        return 1;
    } else if (is_operator(c)) {
        lex->current_state = STATE_OPERATOR;
        return 1;
    } else if (isspace(c)) {
        lex->current_pos++;
        return 1;
    } else {
        lex->current_state = STATE_ERROR;
        return 0;
    }
}

int handle_identifier_state(lexer_t *lex) {
    if (lex == NULL) return 0;
    
    char *token_ptr = lex->token;
    while (isalnum(*(lex->current_pos)) && (token_ptr - lex->token) < MAX_INPUT_LEN - 1) {
        *token_ptr = *(lex->current_pos);
        token_ptr++;
        lex->current_pos++;
    }
    *token_ptr = '\0';
    lex->current_state = STATE_START;
    return 1;
}

int handle_number_state(lexer_t *lex) {
    if (lex == NULL) return 0;
    
    char *token_ptr = lex->token;
    while (isdigit(*(lex->current_pos)) && (token_ptr - lex->token) < MAX_INPUT_LEN - 1) {
        *token_ptr = *(lex->current_pos);
        token_ptr++;
        lex->current_pos++;
    }
    *token_ptr = '\0';
    lex->current_state = STATE_START;
    return 1;
}

int handle_operator_state(lexer_t *lex) {
    if (lex == NULL) return 0;
    
    lex->token[0] = *(lex->current_pos);
    lex->token[1] = '\0';
    lex->current_pos++;
    lex->current_state = STATE_START;
    return 1;
}

int get_next_token(lexer_t *lex) {
    if (lex == NULL || lex->input == NULL || lex->current_pos == NULL) return 0;
    
    memset(lex->token, 0, MAX_INPUT_LEN);
    
    while (*(lex->current_pos) != '\0') {
        switch (lex->current_state) {
            case STATE_START:
                if (!handle_start_state(lex)) return 0;
                break;
            case STATE_IDENTIFIER:
                if (handle_identifier_state(lex) && lex->token[0] != '\0') return 1;
                break;
            case STATE_NUMBER:
                if (handle_number_state(lex) && lex->token[0] != '\0') return 1;
                break;
            case STATE_OPERATOR:
                if (handle_operator_state(lex) && lex->token[0] != '\0') return 1;
                break;
            case STATE_ERROR:
                return 0;
            default:
                return 0;
        }
    }
    return 0;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    printf("Enter expression: ");
    
    if (fgets(input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    lexer_t lexer;
    lexer_init(&lexer, input_buffer);
    
    printf("Tokens:\n");
    while (get_next_token(&lexer)) {
        if (lexer.token[0] != '\0') {
            printf("Token: %s\n", lexer.token);
        }
    }
    
    if (lexer.current_state == STATE_ERROR) {
        printf("Lexical error at position: %ld\n", lexer.current_pos - lexer.input);
        return 1;
    }
    
    return 0;
}