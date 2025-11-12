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
    
    char *end = input + strlen(input);
    if (end > input + MAX_INPUT - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    while (ptr < end && state != STATE_DONE) {
        switch (state) {
            case STATE_IDLE:
                if (isalpha(*ptr)) {
                    state = STATE_READING;
                    word_count++;
                    char_count++;
                } else {
                    ptr++;
                }
                break;
                
            case STATE_READING:
                if (isalpha(*ptr)) {
                    char_count++;
                    ptr++;
                } else if (isspace(*ptr) || *ptr == '\0') {
                    state = STATE_PROCESSING;
                } else {
                    state = STATE_IDLE;
                    ptr++;
                }
                break;
                
            case STATE_PROCESSING:
                if (isspace(*ptr)) {
                    ptr++;
                } else if (*ptr == '\0') {
                    state = STATE_DONE;
                } else {
                    state = STATE_IDLE;
                }
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    printf("Words: %d\n", word_count);
    printf("Alphabetic characters: %d\n", char_count);
    
    return 0;
}