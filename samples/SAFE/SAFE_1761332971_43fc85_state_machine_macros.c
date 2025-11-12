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
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    char output[MAX_INPUT_LEN + 1];
    int word_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_index = 0;
    int char_index = 0;
    int output_index = 0;
    
    printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
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
                if (input_len >= (int)sizeof(input)) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (int i = 0; i < input_len && word_count < MAX_WORDS; i++) {
                    char c = input[i];
                    
                    if (IS_VALID_CHAR(c)) {
                        if (char_index < MAX_WORD_LEN) {
                            words[word_count][char_index++] = c;
                        } else {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                    } else if (c == ' ' || c == '\0') {
                        if (char_index > 0) {
                            words[word_count][char_index] = '\0';
                            word_count++;
                            char_index = 0;
                        }
                    } else {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    if (char_index > 0 && word_count < MAX_WORDS) {
                        words[word_count][char_index] = '\0';
                        word_count++;
                    }
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (word_count == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                output_index = 0;
                for (int i = 0; i < word_count && output_index < MAX_INPUT_LEN; i++) {
                    int word_len = (int)strlen(words[i]);
                    if (word_len == 0) continue;
                    
                    for (int j = 0; j < word_len && output_index < MAX_INPUT_LEN - 1; j++) {
                        output[output_index++] = TO_UPPER(words[i][j]);
                    }
                    
                    if (i < word_count - 1 && output_index < MAX_INPUT_LEN - 1) {
                        output[output_index++] = ' ';
                    }
                }
                
                if (output_index >= MAX_INPUT_LEN) {
                    TRANSITION(STATE_ERROR);
                } else {
                    output[output_index] = '\0';
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed text: %s\n", output);
                printf("Word count: %d\n", word_count);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or processing error\n");
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return 0;
}