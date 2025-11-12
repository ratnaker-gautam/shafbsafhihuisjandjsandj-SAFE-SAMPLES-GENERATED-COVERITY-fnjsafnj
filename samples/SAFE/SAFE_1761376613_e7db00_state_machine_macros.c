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

typedef struct {
    char data[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordList;

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordList words;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_start = -1;
    int word_len = 0;
    
    words.count = 0;
    memset(words.data, 0, sizeof(words.data));
    memset(input, 0, sizeof(input));
    
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
    
    for (int i = 0; i <= input_len && current_state != STATE_ERROR; i++) {
        switch (current_state) {
            case STATE_READING:
                if (i < input_len && IS_VALID_CHAR(input[i])) {
                    if (word_start == -1) {
                        word_start = i;
                        word_len = 0;
                    }
                    word_len++;
                } else {
                    if (word_start != -1) {
                        if (words.count < MAX_WORDS && word_len < MAX_WORD_LEN) {
                            TRANSITION(STATE_PROCESSING);
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (word_start >= 0 && word_len > 0 && word_len < MAX_WORD_LEN) {
                    for (int j = 0; j < word_len; j++) {
                        words.data[words.count][j] = TO_UPPER(input[word_start + j]);
                    }
                    words.data[words.count][word_len] = '\0';
                    words.count++;
                    word_start = -1;
                    word_len = 0;
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_READING || current_state == STATE_PROCESSING) {
        TRANSITION(STATE_WRITING);
    }
    
    if (current_state == STATE_WRITING) {
        printf("Processed words (%d found):\n", words.count);
        for (int i = 0; i < words.count; i++) {
            if (words.data[i][0] != '\0') {
                printf("%d: %s\n", i + 1, words.data[i]);
            }
        }
    } else {
        printf("Error: Invalid input or processing error\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}