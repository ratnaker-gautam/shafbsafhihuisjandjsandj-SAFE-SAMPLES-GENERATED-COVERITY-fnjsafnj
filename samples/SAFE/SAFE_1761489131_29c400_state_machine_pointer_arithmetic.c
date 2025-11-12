//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char *ptr = input;
    char *end = input + MAX_INPUT_LEN;
    int state = STATE_IDLE;
    int word_count = 0;
    int in_word = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }
    
    while (state != STATE_DONE && ptr < end && *ptr != '\0') {
        switch (state) {
            case STATE_IDLE:
                if (isalpha((unsigned char)*ptr)) {
                    state = STATE_READING;
                    in_word = 1;
                } else {
                    ptr++;
                }
                break;
                
            case STATE_READING:
                if (!isalpha((unsigned char)*ptr)) {
                    state = STATE_PROCESSING;
                    in_word = 0;
                } else {
                    ptr++;
                }
                break;
                
            case STATE_PROCESSING:
                word_count++;
                state = STATE_IDLE;
                break;
                
            default:
                state = STATE_DONE;
                break;
        }
    }
    
    if (state == STATE_READING && in_word) {
        word_count++;
    }
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}