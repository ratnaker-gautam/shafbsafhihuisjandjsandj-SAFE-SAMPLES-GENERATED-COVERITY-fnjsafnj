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
    int word_pos = 0;
    int i = 0;
    
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
                if (input_len <= 0) {
                    TRANSITION(STATE_PROCESSING);
                    break;
                }
                
                if (word_count >= MAX_WORDS) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                while (i < input_len && !IS_VALID_CHAR(input_buffer[i])) {
                    i++;
                }
                
                if (i >= input_len) {
                    TRANSITION(STATE_PROCESSING);
                    break;
                }
                
                word_pos = 0;
                while (i < input_len && IS_VALID_CHAR(input_buffer[i]) && word_pos < MAX_WORD_LEN) {
                    words[word_count][word_pos++] = input_buffer[i++];
                }
                
                if (word_pos > 0) {
                    words[word_count][word_pos] = '\0';
                    word_count++;
                }
                
                if (word_pos >= MAX_WORD_LEN && i < input_len && IS_VALID_CHAR(input_buffer[i])) {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (word_count == 0) {
                    TRANSITION(STATE_IDLE);
                    break;
                }
                
                for (int j = 0; j < word_count; j++) {
                    for (int k = 0; words[j][k] != '\0'; k++) {
                        words[j][k] = TO_UPPER(words[j][k]);
                    }
                }
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed words (%d):\n", word_count);
                for (int j = 0; j < word_count; j++) {
                    if (printf("%s\n", words[j]) < 0) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                }
                if (current_state == STATE_WRITING) {
                    TRANSITION(STATE_IDLE);
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Processing failed\n");
                return EXIT_FAILURE;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_IDLE) {
        printf("Processing completed successfully\n");
    }
    
    return EXIT_SUCCESS;
}