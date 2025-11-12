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
    char input[MAX_INPUT_LEN + 1];
    WordList word_list = {0};
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_start = -1;
    int word_count = 0;
    
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
                            if (word_len >= MAX_WORD_LEN || word_count >= MAX_WORDS) {
                                TRANSITION(STATE_ERROR);
                                break;
                            }
                            strncpy(word_list.words[word_count], &input[word_start], word_len);
                            word_list.words[word_count][word_len] = '\0';
                            word_count++;
                            word_list.count = word_count;
                            word_start = -1;
                            TRANSITION(STATE_PROCESSING);
                        }
                    }
                } else {
                    if (word_start != -1) {
                        int word_len = i - word_start;
                        if (word_len >= MAX_WORD_LEN || word_count >= MAX_WORDS) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        strncpy(word_list.words[word_count], &input[word_start], word_len);
                        word_list.words[word_count][word_len] = '\0';
                        word_count++;
                        word_list.count = word_count;
                    }
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_PROCESSING:
                for (int j = 0; j < strlen(word_list.words[word_count - 1]); j++) {
                    if (word_list.words[word_count - 1][j] >= 'a' && word_list.words[word_count - 1][j] <= 'z') {
                        word_list.words[word_count - 1][j] = word_list.words[word_count - 1][j] - 'a' + 'A';
                    }
                }
                TRANSITION(STATE_READING);
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
                return EXIT_FAILURE;
                
            case STATE_IDLE:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or buffer overflow\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}