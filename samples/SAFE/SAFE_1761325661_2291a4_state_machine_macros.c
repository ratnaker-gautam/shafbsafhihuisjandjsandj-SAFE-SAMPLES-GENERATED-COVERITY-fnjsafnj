//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_WORDS 50
#define MAX_WORD_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? (c - 32) : c)

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
    
    while (current_state != STATE_ERROR && current_state != STATE_IDLE) {
        switch (current_state) {
            case STATE_READING:
                if (input_len <= 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (int i = 0; i < input_len && word_count < MAX_WORDS; i++) {
                    if (IS_VALID_CHAR(input[i])) {
                        if (char_index < MAX_WORD_LEN) {
                            words[word_count][char_index] = TO_UPPER(input[i]);
                            char_index++;
                        }
                    } else {
                        if (char_index > 0) {
                            words[word_count][char_index] = '\0';
                            word_count++;
                            char_index = 0;
                        }
                    }
                }
                
                if (char_index > 0 && word_count < MAX_WORDS) {
                    words[word_count][char_index] = '\0';
                    word_count++;
                    char_index = 0;
                }
                
                if (word_count > 0) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                for (int i = 0; i < word_count - 1; i++) {
                    for (int j = 0; j < word_count - i - 1; j++) {
                        if (strcmp(words[j], words[j + 1]) > 0) {
                            char temp[MAX_WORD_LEN + 1];
                            strncpy(temp, words[j], sizeof(temp) - 1);
                            temp[sizeof(temp) - 1] = '\0';
                            strncpy(words[j], words[j + 1], MAX_WORD_LEN);
                            words[j][MAX_WORD_LEN] = '\0';
                            strncpy(words[j + 1], temp, MAX_WORD_LEN);
                            words[j + 1][MAX_WORD_LEN] = '\0';
                        }
                    }
                }
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Sorted words:\n");
                for (int i = 0; i < word_count; i++) {
                    if (printf("%s\n", words[i]) < 0) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                }
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error processing input\n");
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return 0;
}