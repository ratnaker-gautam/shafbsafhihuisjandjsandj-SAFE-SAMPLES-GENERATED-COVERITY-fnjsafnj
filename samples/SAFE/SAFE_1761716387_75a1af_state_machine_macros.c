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
                if (input_len <= 0 || input_len > MAX_INPUT_LEN) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                word_count = 0;
                word_pos = 0;
                i = 0;
                
                while (i < input_len && word_count < MAX_WORDS) {
                    if (IS_VALID_CHAR(input[i])) {
                        if (word_pos < MAX_WORD_LEN) {
                            words[word_count][word_pos] = TO_UPPER(input[i]);
                            word_pos++;
                        }
                        i++;
                    } else {
                        if (word_pos > 0) {
                            words[word_count][word_pos] = '\0';
                            word_count++;
                            word_pos = 0;
                        }
                        while (i < input_len && !IS_VALID_CHAR(input[i])) {
                            i++;
                        }
                    }
                }
                
                if (word_pos > 0 && word_count < MAX_WORDS) {
                    words[word_count][word_pos] = '\0';
                    word_count++;
                }
                
                if (word_count > 0) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (word_count <= 0 || word_count > MAX_WORDS) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (i = 0; i < word_count; i++) {
                    int len = (int)strlen(words[i]);
                    if (len > 0) {
                        for (int j = 0; j < len / 2; j++) {
                            char temp = words[i][j];
                            words[i][j] = words[i][len - 1 - j];
                            words[i][len - 1 - j] = temp;
                        }
                    }
                }
                
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                if (word_count <= 0 || word_count > MAX_WORDS) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                printf("Processed words (%d):\n", word_count);
                for (i = 0; i < word_count; i++) {
                    if (strlen(words[i]) > 0) {
                        printf("%s ", words[i]);
                    }
                }
                printf("\n");
                
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