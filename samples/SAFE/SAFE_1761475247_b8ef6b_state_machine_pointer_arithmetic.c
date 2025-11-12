//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len >= MAX_INPUT) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)str[i]) && str[i] != ' ') return 0;
    }
    return 1;
}

void process_token(char *start, char *end) {
    if (start >= end) return;
    size_t len = end - start;
    if (len == 0) return;
    
    char buffer[MAX_INPUT];
    if (len >= MAX_INPUT) return;
    
    char *dest = buffer;
    char *src = start;
    while (src < end) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    
    printf("Token: %s\n", buffer);
}

int main(void) {
    char input[MAX_INPUT];
    int state = STATE_IDLE;
    
    printf("Enter text (alphanumeric and spaces only): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *current = input;
    char *token_start = NULL;
    
    while (state != STATE_DONE) {
        switch (state) {
            case STATE_IDLE:
                if (*current == '\0') {
                    state = STATE_DONE;
                } else if (!isspace((unsigned char)*current)) {
                    token_start = current;
                    state = STATE_READING;
                } else {
                    current++;
                }
                break;
                
            case STATE_READING:
                if (*current == '\0' || isspace((unsigned char)*current)) {
                    state = STATE_PROCESSING;
                } else {
                    current++;
                }
                break;
                
            case STATE_PROCESSING:
                process_token(token_start, current);
                state = STATE_IDLE;
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    return 0;
}