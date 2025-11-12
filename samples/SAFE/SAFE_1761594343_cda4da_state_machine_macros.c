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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '\n')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordList;

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    WordList word_list = {0};
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_start = -1;
    int word_count = 0;
    
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
                    if (!IS_VALID_CHAR(input[i])) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    if (input[i] != ' ') {
                        if (word_start == -1) {
                            word_start = i;
                        }
                    } else {
                        if (word_start != -1) {
                            int word_len = i - word_start;
                            if (word_len < MAX_WORD_LEN && word_count < MAX_WORDS) {
                                strncpy(word_list.words[word_count], &input[word_start], word_len);
                                word_list.words[word_count][word_len] = '\0';
                                word_count++;
                                word_list.count = word_count;
                                word_start = -1;
                            } else {
                                TRANSITION(STATE_ERROR);
                            }
                        }
                    }
                } else {
                    if (word_start != -1) {
                        int word_len = i - word_start;
                        if (word_len < MAX_WORD_LEN && word_count < MAX_WORDS) {
                            strncpy(word_list.words[word_count], &input[word_start], word_len);
                            word_list.words[word_count][word_len] = '\0';
                            word_count++;
                            word_list.count = word_count;
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    }
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (word_list.count > 0) {
                    for (int j = 0; j < word_list.count - 1; j++) {
                        for (int k = j + 1; k < word_list.count; k++) {
                            if (strcmp(word_list.words[j], word_list.words[k]) > 0) {
                                char temp[MAX_WORD_LEN];
                                strncpy(temp, word_list.words[j], sizeof(temp));
                                strncpy(word_list.words[j], word_list.words[k], sizeof(word_list.words[j]));
                                strncpy(word_list.words[k], temp, sizeof(word_list.words[k]));
                            }
                        }
                    }
                }
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Sorted words:\n");
                for (int j = 0; j < word_list.count; j++) {
                    printf("%d: %s\n", j + 1, word_list.words[j]);
                }
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or processing error\n");
                return EXIT_FAILURE;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_IDLE) {
        printf("Processing complete.\n");
    }
    
    return EXIT_SUCCESS;
}