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
    char output[MAX_INPUT_LEN + 1] = {0};
    int word_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int pos = 0;
    int word_pos = 0;
    int output_pos = 0;
    
    printf("Enter text (letters only, max %d chars): ", MAX_INPUT_LEN);
    
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
                
                for (pos = 0; pos < input_len && word_count < MAX_WORDS; pos++) {
                    if (IS_VALID_CHAR(input[pos])) {
                        if (word_pos < MAX_WORD_LEN) {
                            words[word_count][word_pos++] = input[pos];
                        } else {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                    } else if (word_pos > 0) {
                        words[word_count][word_pos] = '\0';
                        word_count++;
                        word_pos = 0;
                    }
                }
                
                if (word_pos > 0 && word_count < MAX_WORDS) {
                    words[word_count][word_pos] = '\0';
                    word_count++;
                }
                
                if (current_state != STATE_ERROR) {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (word_count <= 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (int i = 0; i < word_count; i++) {
                    int word_len = (int)strlen(words[i]);
                    if (word_len <= 0) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    for (int j = 0; j < word_len; j++) {
                        words[i][j] = TO_UPPER(words[i][j]);
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                output_pos = 0;
                for (int i = 0; i < word_count && output_pos < MAX_INPUT_LEN; i++) {
                    int word_len = (int)strlen(words[i]);
                    if (output_pos + word_len + 1 > MAX_INPUT_LEN) {
                        break;
                    }
                    
                    if (i > 0) {
                        output[output_pos++] = ' ';
                    }
                    
                    for (int j = 0; j < word_len && output_pos < MAX_INPUT_LEN; j++) {
                        output[output_pos++] = words[i][j];
                    }
                }
                output[output_pos] = '\0';
                
                printf("Processed text: %s\n", output);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or processing error\n");
                return EXIT_FAILURE;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return EXIT_SUCCESS;
}