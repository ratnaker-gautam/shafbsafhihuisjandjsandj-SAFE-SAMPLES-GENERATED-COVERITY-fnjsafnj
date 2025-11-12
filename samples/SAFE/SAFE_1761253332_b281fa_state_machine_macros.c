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

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int current_word_len;
} word_data_t;

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    word_data_t data;
    int current_state = STATE_IDLE;
    int i = 0;
    char c;
    
    memset(&data, 0, sizeof(data));
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    }
    
    if (current_state != STATE_ERROR) {
        input[strcspn(input, "\n")] = '\0';
        
        if (strlen(input) == 0) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    while (current_state != STATE_ERROR && i < (int)strlen(input)) {
        c = input[i];
        
        switch (current_state) {
            case STATE_READING:
                if (IS_VALID_CHAR(c)) {
                    if (data.current_word_len < MAX_WORD_LEN - 1) {
                        data.words[data.word_count][data.current_word_len] = TO_UPPER(c);
                        data.current_word_len++;
                        data.words[data.word_count][data.current_word_len] = '\0';
                    }
                } else {
                    if (data.current_word_len > 0) {
                        data.word_count++;
                        if (data.word_count >= MAX_WORDS) {
                            TRANSITION(STATE_ERROR);
                        } else {
                            data.current_word_len = 0;
                            TRANSITION(STATE_PROCESSING);
                        }
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (IS_VALID_CHAR(c)) {
                    if (data.current_word_len < MAX_WORD_LEN - 1) {
                        data.words[data.word_count][data.current_word_len] = TO_UPPER(c);
                        data.current_word_len++;
                        data.words[data.word_count][data.current_word_len] = '\0';
                    }
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_READING);
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        i++;
    }
    
    if (current_state != STATE_ERROR) {
        if (data.current_word_len > 0) {
            data.word_count++;
            if (data.word_count > MAX_WORDS) {
                TRANSITION(STATE_ERROR);
            }
        }
        
        if (current_state != STATE_ERROR) {
            TRANSITION(STATE_WRITING);
        }
    }
    
    switch (current_state) {
        case STATE_WRITING:
            printf("Processed %d words:\n", data.word_count);
            for (int j = 0; j < data.word_count; j++) {
                if (j < MAX_WORDS) {
                    printf("%d: %s\n", j + 1, data.words[j]);
                }
            }
            break;
            
        case STATE_ERROR:
            printf("Error: Invalid input or processing error\n");
            break;
            
        default:
            printf("Unexpected state: %d\n", current_state);
            break;
    }
    
    return current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}