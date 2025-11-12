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
    char input[MAX_INPUT_LEN + 1] = {0};
    char words[MAX_WORDS][MAX_WORD_LEN + 1] = {0};
    int word_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_index = 0;
    int char_index = 0;
    
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
    
    for (int i = 0; i < input_len && current_state != STATE_ERROR; i++) {
        switch (current_state) {
            case STATE_READING:
                if (IS_ALPHA(input[i])) {
                    if (word_index < MAX_WORDS && char_index < MAX_WORD_LEN) {
                        words[word_index][char_index++] = input[i];
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else if (!IS_SPACE(input[i])) {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (IS_ALPHA(input[i])) {
                    if (char_index < MAX_WORD_LEN) {
                        words[word_index][char_index++] = input[i];
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else if (IS_SPACE(input[i])) {
                    if (char_index > 0) {
                        words[word_index][char_index] = '\0';
                        word_index++;
                        word_count++;
                        char_index = 0;
                    }
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_PROCESSING && char_index > 0) {
        words[word_index][char_index] = '\0';
        word_count++;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    TRANSITION(STATE_DONE);
    
    printf("Found %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    return EXIT_SUCCESS;
}