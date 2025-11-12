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
    char input_buffer[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int i = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        input_len = strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[input_len - 1] = '\0';
            input_len--;
        }
        TRANSITION(STATE_READING);
    }
    
    while (current_state != STATE_ERROR && current_state != STATE_IDLE) {
        switch (current_state) {
            case STATE_READING: {
                int pos = 0;
                int word_pos = 0;
                
                while (pos < input_len && word_count < MAX_WORDS) {
                    while (pos < input_len && !IS_VALID_CHAR(input_buffer[pos])) {
                        pos++;
                    }
                    
                    if (pos >= input_len) break;
                    
                    word_pos = 0;
                    while (pos < input_len && IS_VALID_CHAR(input_buffer[pos]) && word_pos < MAX_WORD_LEN) {
                        words[word_count][word_pos] = input_buffer[pos];
                        word_pos++;
                        pos++;
                    }
                    
                    if (word_pos < MAX_WORD_LEN) {
                        words[word_count][word_pos] = '\0';
                        word_count++;
                    } else {
                        words[word_count][MAX_WORD_LEN] = '\0';
                        word_count++;
                    }
                }
                
                if (word_count > 0) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
            }
            
            case STATE_PROCESSING: {
                for (i = 0; i < word_count; i++) {
                    int j = 0;
                    while (words[i][j] != '\0' && j < MAX_WORD_LEN) {
                        words[i][j] = TO_UPPER(words[i][j]);
                        j++;
                    }
                }
                TRANSITION(STATE_WRITING);
                break;
            }
            
            case STATE_WRITING: {
                printf("Processed words (%d):\n", word_count);
                for (i = 0; i < word_count; i++) {
                    if (i < MAX_WORDS) {
                        printf("%d: %s\n", i + 1, words[i]);
                    }
                }
                TRANSITION(STATE_IDLE);
                break;
            }
            
            case STATE_ERROR: {
                printf("Error: Invalid input or processing error\n");
                return EXIT_FAILURE;
            }
        }
    }
    
    return EXIT_SUCCESS;
}