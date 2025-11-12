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
    int valid_input = 1;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
        valid_input = 0;
    }
    
    if (valid_input) {
        input_len = (int)strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[input_len - 1] = '\0';
            input_len--;
        }
        
        if (input_len > MAX_INPUT_LEN) {
            TRANSITION(STATE_ERROR);
            valid_input = 0;
        }
    }
    
    if (!valid_input) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    TRANSITION(STATE_READING);
    
    int i = 0;
    int word_index = 0;
    int char_index = 0;
    
    while (current_state != STATE_ERROR && i < input_len) {
        switch (current_state) {
            case STATE_READING:
                if (IS_VALID_CHAR(input_buffer[i])) {
                    if (word_count >= MAX_WORDS) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    if (char_index >= MAX_WORD_LEN) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    words[word_count][char_index] = input_buffer[i];
                    char_index++;
                    i++;
                } else if (input_buffer[i] == ' ') {
                    if (char_index > 0) {
                        words[word_count][char_index] = '\0';
                        word_count++;
                        char_index = 0;
                    }
                    i++;
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (word_index < word_count) {
                    for (int j = 0; j < (int)strlen(words[word_index]); j++) {
                        words[word_index][j] = TO_UPPER(words[word_index][j]);
                    }
                    word_index++;
                    if (word_index >= word_count) {
                        TRANSITION(STATE_WRITING);
                    }
                }
                break;
                
            case STATE_WRITING:
                printf("Processed words (%d):\n", word_count);
                for (int j = 0; j < word_count; j++) {
                    printf("%s\n", words[j]);
                }
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_READING && char_index > 0) {
        words[word_count][char_index] = '\0';
        word_count++;
        TRANSITION(STATE_PROCESSING);
    }
    
    if (current_state == STATE_PROCESSING && word_count > 0) {
        TRANSITION(STATE_WRITING);
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Processing failed\n");
        return 1;
    }
    
    if (current_state == STATE_WRITING) {
        printf("Processed words (%d):\n", word_count);
        for (int j = 0; j < word_count; j++) {
            printf("%s\n", words[j]);
        }
    }
    
    return 0;
}