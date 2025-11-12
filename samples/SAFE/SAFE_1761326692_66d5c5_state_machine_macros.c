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
#define IS_DIGIT(c) (isdigit((unsigned char)(c)))
#define IS_ALPHA(c) (isalpha((unsigned char)(c)))

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1] = {0};
    int input_len = 0;
    int number_count = 0;
    int word_count = 0;
    int i = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        input_len = (int)strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
            input_len--;
        }
        TRANSITION(STATE_READING);
    }
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (input_len <= 0) {
                    TRANSITION(STATE_DONE);
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (i >= input_len) {
                    TRANSITION(STATE_DONE);
                } else if (IS_DIGIT(input[i])) {
                    while (i < input_len && IS_DIGIT(input[i])) {
                        i++;
                    }
                    number_count++;
                    TRANSITION(STATE_PROCESSING);
                } else if (IS_ALPHA(input[i])) {
                    while (i < input_len && IS_ALPHA(input[i])) {
                        i++;
                    }
                    word_count++;
                    TRANSITION(STATE_PROCESSING);
                } else {
                    i++;
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_DONE:
                break;
                
            case STATE_ERROR:
                printf("Input error occurred\n");
                return EXIT_FAILURE;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_DONE) {
        printf("Analysis complete:\n");
        printf("Numbers found: %d\n", number_count);
        printf("Words found: %d\n", word_count);
    }
    
    return EXIT_SUCCESS;
}