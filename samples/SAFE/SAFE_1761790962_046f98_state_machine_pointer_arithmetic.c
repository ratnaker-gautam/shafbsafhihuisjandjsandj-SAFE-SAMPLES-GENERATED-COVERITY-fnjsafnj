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

int main(void) {
    char input[MAX_INPUT];
    char *ptr = input;
    int state = STATE_IDLE;
    int word_count = 0;
    int char_count = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *end_ptr = input + strlen(input);
    if (end_ptr > input + MAX_INPUT - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    while (ptr < end_ptr && state != STATE_DONE) {
        switch (state) {
            case STATE_IDLE:
                if (isalpha((unsigned char)*ptr)) {
                    state = STATE_READING;
                    word_count++;
                    char_count++;
                } else {
                    ptr++;
                }
                break;
                
            case STATE_READING:
                if (isalpha((unsigned char)*ptr)) {
                    char_count++;
                    ptr++;
                } else if (isspace((unsigned char)*ptr) || *ptr == '\0') {
                    state = STATE_PROCESSING;
                } else {
                    ptr++;
                }
                break;
                
            case STATE_PROCESSING:
                if (ptr >= end_ptr - 1 || *ptr == '\0') {
                    state = STATE_DONE;
                } else if (isalpha((unsigned char)*ptr)) {
                    state = STATE_READING;
                    word_count++;
                    char_count++;
                    ptr++;
                } else {
                    ptr++;
                }
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (state == STATE_DONE || ptr >= end_ptr) {
        printf("Words: %d\n", word_count);
        printf("Characters in words: %d\n", char_count);
    } else {
        fprintf(stderr, "Unexpected state: %d\n", state);
        return 1;
    }
    
    return 0;
}