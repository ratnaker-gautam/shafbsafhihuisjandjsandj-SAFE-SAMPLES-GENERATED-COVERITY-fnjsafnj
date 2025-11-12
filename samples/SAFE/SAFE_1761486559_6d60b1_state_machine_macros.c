//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_WORDS 20
#define MAX_WORD_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_ALPHA(c) (isalpha((unsigned char)(c)))
#define IS_SPACE(c) (isspace((unsigned char)(c)))

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_pos = 0;
    int char_count = 0;
    
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
    
    for (int i = 0; i <= input_len && current_state != STATE_ERROR; i++) {
        switch (current_state) {
            case STATE_READING:
                if (i == input_len) {
                    if (word_pos > 0 && word_count < MAX_WORDS) {
                        words[word_count][word_pos] = '\0';
                        word_count++;
                    }
                    TRANSITION(STATE_PROCESSING);
                } else if (IS_ALPHA(input[i])) {
                    if (word_count >= MAX_WORDS) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    if (word_pos >= MAX_WORD_LEN) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    words[word_count][word_pos++] = (char)tolower((unsigned char)input[i]);
                    char_count++;
                } else if (IS_SPACE(input[i])) {
                    if (word_pos > 0 && word_count < MAX_WORDS) {
                        words[word_count][word_pos] = '\0';
                        word_count++;
                        word_pos = 0;
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (word_count == 0) {
                    printf("No words found in input.\n");
                } else {
                    printf("Found %d words (%d characters):\n", word_count, char_count);
                    for (int j = 0; j < word_count; j++) {
                        if (j < MAX_WORDS) {
                            printf("%d: %s\n", j + 1, words[j]);
                        }
                    }
                }
                TRANSITION(STATE_DONE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or buffer overflow.\n");
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_DONE) {
        printf("Processing completed successfully.\n");
    } else if (current_state != STATE_ERROR) {
        printf("Unexpected termination.\n");
    }
    
    return current_state == STATE_DONE ? EXIT_SUCCESS : EXIT_FAILURE;
}