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
    int word_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_pos = 0;
    int i = 0;
    
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
    
    while (current_state != STATE_ERROR && i < input_len) {
        switch (current_state) {
            case STATE_IDLE:
                if (IS_VALID_CHAR(input[i])) {
                    if (word_count >= MAX_WORDS) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    word_pos = 0;
                    words[word_count][word_pos++] = TO_UPPER(input[i]);
                    TRANSITION(STATE_READING);
                }
                i++;
                break;
                
            case STATE_READING:
                if (IS_VALID_CHAR(input[i])) {
                    if (word_pos >= MAX_WORD_LEN) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    words[word_count][word_pos++] = TO_UPPER(input[i]);
                } else {
                    words[word_count][word_pos] = '\0';
                    TRANSITION(STATE_PROCESSING);
                }
                i++;
                break;
                
            case STATE_PROCESSING:
                word_count++;
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_READING && word_pos > 0) {
        words[word_count][word_pos] = '\0';
        word_count++;
        TRANSITION(STATE_WRITING);
    } else if (current_state == STATE_PROCESSING) {
        TRANSITION(STATE_WRITING);
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Input processing failed\n");
        return 1;
    }
    
    printf("Processed %d words:\n", word_count);
    for (int j = 0; j < word_count; j++) {
        if (strlen(words[j]) > 0) {
            printf("%s\n", words[j]);
        }
    }
    
    return 0;
}