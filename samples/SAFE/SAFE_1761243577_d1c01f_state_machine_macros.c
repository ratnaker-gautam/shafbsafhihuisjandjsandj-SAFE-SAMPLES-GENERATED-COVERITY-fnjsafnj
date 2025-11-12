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
    char words[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordList;

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordList word_list = {0};
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_start = -1;
    int word_len = 0;
    
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
                if (i < input_len) {
                    char c = input[i];
                    if (IS_VALID_CHAR(c)) {
                        if (word_start == -1) {
                            word_start = i;
                            word_len = 0;
                        }
                        if (word_len < MAX_WORD_LEN - 1) {
                            word_len++;
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    } else {
                        if (word_start != -1) {
                            TRANSITION(STATE_PROCESSING);
                            i--;
                        }
                    }
                } else {
                    if (word_start != -1) {
                        TRANSITION(STATE_PROCESSING);
                        i--;
                    } else {
                        TRANSITION(STATE_WRITING);
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (word_list.count < MAX_WORDS && word_start != -1 && word_len > 0) {
                    int copy_len = word_len;
                    if (copy_len >= MAX_WORD_LEN) {
                        copy_len = MAX_WORD_LEN - 1;
                    }
                    
                    for (int j = 0; j < copy_len; j++) {
                        char c = input[word_start + j];
                        word_list.words[word_list.count][j] = TO_UPPER(c);
                    }
                    word_list.words[word_list.count][copy_len] = '\0';
                    word_list.count++;
                    
                    word_start = -1;
                    word_len = 0;
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed words (%d):\n", word_list.count);
                for (int j = 0; j < word_list.count; j++) {
                    printf("%d: %s\n", j + 1, word_list.words[j]);
                }
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                return 1;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_READING || current_state == STATE_PROCESSING) {
        TRANSITION(STATE_WRITING);
    }
    
    if (current_state == STATE_WRITING) {
        printf("Processed words (%d):\n", word_list.count);
        for (int j = 0; j < word_list.count; j++) {
            printf("%d: %s\n", j + 1, word_list.words[j]);
        }
    }
    
    return 0;
}