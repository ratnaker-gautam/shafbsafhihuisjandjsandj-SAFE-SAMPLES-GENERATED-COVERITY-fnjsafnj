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
    int pos = 0;
    int word_pos = 0;
    int output_pos = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    }
    
    if (current_state != STATE_ERROR) {
        input_len = strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
            input_len--;
        }
        
        if (input_len > MAX_INPUT_LEN) {
            TRANSITION(STATE_ERROR);
        }
    }
    
    while (current_state != STATE_ERROR && pos <= input_len) {
        switch (current_state) {
            case STATE_IDLE:
                if (pos < input_len && IS_VALID_CHAR(input[pos])) {
                    word_count = 0;
                    word_pos = 0;
                    if (word_count < MAX_WORDS) {
                        words[word_count][word_pos++] = TO_UPPER(input[pos]);
                        words[word_count][word_pos] = '\0';
                    }
                    pos++;
                    TRANSITION(STATE_READING);
                } else {
                    pos++;
                }
                break;
                
            case STATE_READING:
                if (pos < input_len && IS_VALID_CHAR(input[pos])) {
                    if (word_pos < MAX_WORD_LEN - 1) {
                        words[word_count][word_pos++] = TO_UPPER(input[pos]);
                        words[word_count][word_pos] = '\0';
                    }
                    pos++;
                } else {
                    word_count++;
                    word_pos = 0;
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (word_count >= MAX_WORDS || pos >= input_len) {
                    output_pos = 0;
                    for (int i = 0; i < word_count && output_pos < MAX_INPUT_LEN; i++) {
                        int word_len = strlen(words[i]);
                        if (output_pos + word_len + 1 < MAX_INPUT_LEN) {
                            if (i > 0) {
                                output[output_pos++] = ' ';
                            }
                            strncpy(&output[output_pos], words[i], word_len);
                            output_pos += word_len;
                        }
                    }
                    output[output_pos] = '\0';
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_IDLE);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed: %s\n", output);
                TRANSITION(STATE_IDLE);
                pos = input_len + 1;
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing error\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}