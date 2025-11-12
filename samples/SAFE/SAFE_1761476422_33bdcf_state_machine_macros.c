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
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1] = {0};
    char words[MAX_WORDS][MAX_WORD_LEN + 1] = {0};
    int word_count = 0;
    int char_index = 0;
    int word_index = 0;
    int valid_input = 1;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (char_index >= (int)strlen(input)) {
                    if (word_index > 0 && word_count < MAX_WORDS) {
                        words[word_count][word_index] = '\0';
                        word_count++;
                    }
                    TRANSITION(STATE_PROCESSING);
                    break;
                }
                
                char c = input[char_index];
                
                if (IS_ALPHA(c)) {
                    if (word_index < MAX_WORD_LEN) {
                        words[word_count][word_index] = tolower((unsigned char)c);
                        word_index++;
                    } else {
                        valid_input = 0;
                        TRANSITION(STATE_ERROR);
                    }
                } else if (IS_SPACE(c)) {
                    if (word_index > 0) {
                        if (word_count < MAX_WORDS) {
                            words[word_count][word_index] = '\0';
                            word_count++;
                            word_index = 0;
                        } else {
                            valid_input = 0;
                            TRANSITION(STATE_ERROR);
                        }
                    }
                } else {
                    valid_input = 0;
                    TRANSITION(STATE_ERROR);
                }
                
                char_index++;
                break;
                
            case STATE_PROCESSING:
                if (!valid_input || word_count == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                printf("Found %d words:\n", word_count);
                for (int i = 0; i < word_count; i++) {
                    if (i < MAX_WORDS && words[i][0] != '\0') {
                        printf("%d: %s\n", i + 1, words[i]);
                    }
                }
                
                TRANSITION(STATE_DONE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}