//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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
#define IS_ALPHA_CHAR(c) (isalpha((unsigned char)(c)))
#define IS_SPACE_CHAR(c) (isspace((unsigned char)(c)))

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    char words[MAX_WORDS][MAX_WORD_LEN + 1] = {0};
    int word_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_pos = 0;
    int char_pos = 0;
    
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
                    TRANSITION(STATE_ERROR);
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (char_pos >= input_len) {
                    if (word_pos > 0 && word_count < MAX_WORDS) {
                        words[word_count][word_pos] = '\0';
                        word_count++;
                    }
                    TRANSITION(STATE_DONE);
                } else if (word_count >= MAX_WORDS) {
                    TRANSITION(STATE_ERROR);
                } else {
                    char current_char = input[char_pos];
                    
                    if (IS_ALPHA_CHAR(current_char)) {
                        if (word_pos < MAX_WORD_LEN) {
                            words[word_count][word_pos] = current_char;
                            word_pos++;
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    } else if (IS_SPACE_CHAR(current_char)) {
                        if (word_pos > 0) {
                            words[word_count][word_pos] = '\0';
                            word_count++;
                            word_pos = 0;
                        }
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                    char_pos++;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or processing limit exceeded\n");
                TRANSITION(STATE_DONE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_DONE) {
        printf("Processed %d words:\n", word_count);
        for (int i = 0; i < word_count; i++) {
            if (i < MAX_WORDS) {
                printf("%d: %s\n", i + 1, words[i]);
            }
        }
    }
    
    return current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}