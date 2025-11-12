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
    char input[MAX_INPUT_LEN + 1];
    int number_count = 0;
    int word_count = 0;
    int i = 0;
    int len = 0;

    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    len = (int)strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }

    TRANSITION(STATE_READING);

    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (i >= len) {
                    TRANSITION(STATE_DONE);
                } else if (IS_DIGIT(input[i])) {
                    TRANSITION(STATE_PROCESSING);
                } else if (IS_ALPHA(input[i])) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    i++;
                }
                break;

            case STATE_PROCESSING:
                if (IS_DIGIT(input[i])) {
                    while (i < len && IS_DIGIT(input[i])) {
                        i++;
                    }
                    number_count++;
                    TRANSITION(STATE_READING);
                } else if (IS_ALPHA(input[i])) {
                    while (i < len && IS_ALPHA(input[i])) {
                        i++;
                    }
                    word_count++;
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;

            case STATE_ERROR:
                printf("Invalid input format\n");
                return 1;

            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }

    if (current_state == STATE_DONE) {
        printf("Numbers found: %d\n", number_count);
        printf("Words found: %d\n", word_count);
    }

    return 0;
}