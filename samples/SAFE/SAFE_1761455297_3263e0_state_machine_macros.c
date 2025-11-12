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
    int word_index = 0;
    int char_index = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        input_len = (int)strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[input_len - 1] = '\0';
            input_len--;
        }
        TRANSITION(STATE_READING);
    }
    
    while (current_state != STATE_ERROR && current_state != STATE_IDLE) {
        switch (current_state) {
            case STATE_READING:
                if (word_count >= MAX_WORDS) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (word_index >= input_len) {
                    if (char_index > 0) {
                        words[word_count][char_index] = '\0';
                        word_count++;
                    }
                    TRANSITION(STATE_PROCESSING);
                    break;
                }
                
                char current_char = input_buffer[word_index];
                
                if (IS_VALID_CHAR(current_char)) {
                    if (char_index >= MAX_WORD_LEN) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    words[word_count][char_index] = current_char;
                    char_index++;
                    word_index++;
                } else {
                    if (char_index > 0) {
                        words[word_count][char_index] = '\0';
                        word_count++;
                        char_index = 0;
                    }
                    word_index++;
                }
                break;
                
            case STATE_PROCESSING:
                if (word_count == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (int i = 0; i < word_count; i++) {
                    int word_len = (int)strlen(words[i]);
                    if (word_len == 0) continue;
                    
                    for (int j = 0; j < word_len; j++) {
                        words[i][j] = TO_UPPER(words[i][j]);
                    }
                }
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed words (%d):\n", word_count);
                for (int i = 0; i < word_count; i++) {
                    if (strlen(words[i]) > 0) {
                        printf("%s\n", words[i]);
                    }
                }
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