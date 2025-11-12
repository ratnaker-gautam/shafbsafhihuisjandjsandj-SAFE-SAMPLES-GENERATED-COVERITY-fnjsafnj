//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define TRANSITION(next) do { current_state = next; } while(0)
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define IS_ALPHA(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    int current_state = STATE_IDLE;
    int char_count = 0;
    int digit_count = 0;
    int i = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    if (len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }
    
    TRANSITION(STATE_READING);
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (i >= len) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    char c = input[i];
                    if (IS_ALPHA(c)) {
                        char_count++;
                    } else if (IS_DIGIT(c)) {
                        digit_count++;
                    } else if (c != ' ' && c != '\t' && c != '\0') {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    i++;
                }
                break;
                
            case STATE_PROCESSING:
                printf("Analysis complete:\n");
                printf("Alphabetic characters: %d\n", char_count);
                printf("Digit characters: %d\n", digit_count);
                printf("Total characters processed: %d\n", i);
                TRANSITION(STATE_DONE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid character at position %d\n", i);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return current_state == STATE_ERROR ? 1 : 0;
}