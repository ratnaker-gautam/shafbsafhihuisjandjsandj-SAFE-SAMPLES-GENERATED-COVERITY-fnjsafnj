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

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

void process_token(char *start, char *end) {
    if (start >= end) return;
    
    size_t len = end - start;
    if (len > 0) {
        for (char *p = start; p < end; p++) {
            if (islower((unsigned char)*p)) {
                *p = toupper((unsigned char)*p);
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT];
    char buffer[MAX_INPUT];
    
    printf("Enter text (alphanumeric and spaces only): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (input_len >= sizeof(buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    memcpy(buffer, input, input_len + 1);
    
    int state = STATE_IDLE;
    char *current = buffer;
    char *end = buffer + input_len;
    char *token_start = NULL;
    
    while (state != STATE_DONE) {
        switch (state) {
            case STATE_IDLE:
                if (current < end) {
                    if (!isspace((unsigned char)*current)) {
                        token_start = current;
                        state = STATE_READING;
                    } else {
                        current++;
                    }
                } else {
                    state = STATE_DONE;
                }
                break;
                
            case STATE_READING:
                if (current >= end || isspace((unsigned char)*current)) {
                    state = STATE_PROCESSING;
                } else {
                    current++;
                }
                break;
                
            case STATE_PROCESSING:
                process_token(token_start, current);
                state = STATE_IDLE;
                break;
                
            default:
                state = STATE_DONE;
                break;
        }
    }
    
    printf("Processed text: %s\n", buffer);
    return 0;
}