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
        if (!isprint(*(input + i))) {
            return 0;
        }
    }
    return 1;
}

void process_word(char *start, char *end) {
    if (start >= end) {
        return;
    }
    char *left = start;
    char *right = end - 1;
    while (left < right) {
        char temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }
}

int main(void) {
    char input[MAX_INPUT];
    printf("Enter text: ");
    if (fgets(input, MAX_INPUT, stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && *(input + len - 1) == '\n') {
        *(input + len - 1) = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int state = STATE_IDLE;
    char *current = input;
    char *word_start = NULL;
    
    while (state != STATE_DONE) {
        switch (state) {
            case STATE_IDLE:
                if (*current == '\0') {
                    state = STATE_DONE;
                } else if (!isspace(*current)) {
                    word_start = current;
                    state = STATE_READING;
                } else {
                    current++;
                }
                break;
                
            case STATE_READING:
                if (*current == '\0' || isspace(*current)) {
                    state = STATE_PROCESSING;
                } else {
                    current++;
                }
                break;
                
            case STATE_PROCESSING:
                process_word(word_start, current);
                state = STATE_IDLE;
                break;
                
            default:
                state = STATE_DONE;
                break;
        }
    }
    
    printf("Processed: %s\n", input);
    return 0;
}