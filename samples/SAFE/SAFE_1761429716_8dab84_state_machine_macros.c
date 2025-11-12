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
#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? (c - 'a' + 'A') : c)

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
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                i = 0;
                while (i < input_len && word_count < MAX_WORDS) {
                    while (i < input_len && !IS_VALID_CHAR(input_buffer[i])) {
                        i++;
                    }
                    
                    if (i >= input_len) break;
                    
                    int word_start = i;
                    int word_len = 0;
                    
                    while (i < input_len && IS_VALID_CHAR(input_buffer[i]) && word_len < MAX_WORD_LEN) {
                        word_len++;
                        i++;
                    }
                    
                    if (word_len > 0 && word_count < MAX_WORDS) {
                        if (word_len > MAX_WORD_LEN) {
                            word_len = MAX_WORD_LEN;
                        }
                        
                        for (int j = 0; j < word_len; j++) {
                            words[word_count][j] = input_buffer[word_start + j];
                        }
                        words[word_count][word_len] = '\0';
                        word_count++;
                    }
                }
                TRANSITION(STATE_PROCESSING);
                break;
                
            case STATE_PROCESSING:
                if (word_count <= 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (int w = 0; w < word_count; w++) {
                    int len = (int)strlen(words[w]);
                    for (int c = 0; c < len; c++) {
                        words[w][c] = TO_UPPER(words[w][c]);
                    }
                }
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                if (word_count <= 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                printf("Processed words (%d):\n", word_count);
                for (int w = 0; w < word_count; w++) {
                    printf("%s\n", words[w]);
                }
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error occurred during processing\n");
                return EXIT_FAILURE;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return EXIT_SUCCESS;
}