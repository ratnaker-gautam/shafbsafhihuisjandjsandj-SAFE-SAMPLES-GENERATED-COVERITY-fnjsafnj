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
    int word_count = 0;
    int current_char = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        input_len = strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
            input_len--;
        }
        TRANSITION(STATE_READING);
    }
    
    int i = 0;
    while (current_state != STATE_ERROR && i < input_len) {
        switch (current_state) {
            case STATE_READING:
                if (word_count >= MAX_WORDS) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (IS_VALID_CHAR(input[i])) {
                    if (current_char >= MAX_WORD_LEN) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    words[word_count][current_char] = input[i];
                    current_char++;
                    i++;
                } else {
                    if (current_char > 0) {
                        words[word_count][current_char] = '\0';
                        word_count++;
                        current_char = 0;
                    }
                    i++;
                }
                break;
                
            case STATE_PROCESSING:
                if (word_count == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (int w = 0; w < word_count; w++) {
                    int len = strlen(words[w]);
                    for (int c = 0; c < len; c++) {
                        words[w][c] = TO_UPPER(words[w][c]);
                    }
                }
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed words (%d):\n", word_count);
                for (int w = 0; w < word_count; w++) {
                    printf("%s\n", words[w]);
                }
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or processing error\n");
                return 1;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (current_state == STATE_READING && i >= input_len && current_char > 0) {
            if (word_count >= MAX_WORDS) {
                TRANSITION(STATE_ERROR);
            } else {
                words[word_count][current_char] = '\0';
                word_count++;
                TRANSITION(STATE_PROCESSING);
            }
        }
    }
    
    if (current_state == STATE_READING && word_count == 0) {
        TRANSITION(STATE_ERROR);
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing error\n");
        return 1;
    }
    
    return 0;
}