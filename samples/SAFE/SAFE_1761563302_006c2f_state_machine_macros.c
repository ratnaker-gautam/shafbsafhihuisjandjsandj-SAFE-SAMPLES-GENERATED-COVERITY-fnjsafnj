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

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int processed_count;
} WordData;

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordData data = {0};
    int current_state = STATE_IDLE;
    int input_len = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        input_len = (int)strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
            input_len--;
        }
        if (input_len > MAX_INPUT_LEN) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    while (current_state != STATE_ERROR && current_state != STATE_IDLE) {
        switch (current_state) {
            case STATE_READING: {
                int pos = 0;
                int word_start = -1;
                
                while (pos < input_len && data.word_count < MAX_WORDS) {
                    if (IS_VALID_CHAR(input[pos])) {
                        if (word_start == -1) {
                            word_start = pos;
                        }
                    } else {
                        if (word_start != -1) {
                            int word_len = pos - word_start;
                            if (word_len < MAX_WORD_LEN) {
                                strncpy(data.words[data.word_count], 
                                       &input[word_start], word_len);
                                data.words[data.word_count][word_len] = '\0';
                                data.word_count++;
                            }
                            word_start = -1;
                        }
                    }
                    pos++;
                }
                
                if (word_start != -1 && data.word_count < MAX_WORDS) {
                    int word_len = input_len - word_start;
                    if (word_len < MAX_WORD_LEN) {
                        strncpy(data.words[data.word_count], 
                               &input[word_start], word_len);
                        data.words[data.word_count][word_len] = '\0';
                        data.word_count++;
                    }
                }
                
                if (data.word_count > 0) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
            }
            
            case STATE_PROCESSING: {
                for (int i = 0; i < data.word_count; i++) {
                    for (int j = 0; data.words[i][j] != '\0'; j++) {
                        if (data.words[i][j] >= 'a' && data.words[i][j] <= 'z') {
                            data.words[i][j] = data.words[i][j] - 'a' + 'A';
                        }
                    }
                    data.processed_count++;
                }
                TRANSITION(STATE_WRITING);
                break;
            }
            
            case STATE_WRITING: {
                printf("Processed words (%d):\n", data.processed_count);
                for (int i = 0; i < data.word_count; i++) {
                    printf("%s\n", data.words[i]);
                }
                TRANSITION(STATE_IDLE);
                break;
            }
            
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error processing input\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}