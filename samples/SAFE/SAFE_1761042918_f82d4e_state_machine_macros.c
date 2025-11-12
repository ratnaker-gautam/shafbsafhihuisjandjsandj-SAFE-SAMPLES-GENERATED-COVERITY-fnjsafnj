//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == ' '))

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordList;

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordList word_list = {0};
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_index = 0;
    int char_index = 0;
    int valid_input = 1;
    
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
    
    while (current_state != STATE_ERROR && current_state != STATE_WRITING) {
        switch (current_state) {
            case STATE_READING:
                for (int i = 0; i < input_len && word_index < MAX_WORDS; i++) {
                    if (!IS_VALID_CHAR(input[i])) {
                        valid_input = 0;
                        break;
                    }
                    
                    if (input[i] == ' ') {
                        if (char_index > 0) {
                            word_list.words[word_index][char_index] = '\0';
                            word_list.count++;
                            word_index++;
                            char_index = 0;
                        }
                    } else {
                        if (char_index < MAX_WORD_LEN - 1) {
                            word_list.words[word_index][char_index] = input[i];
                            char_index++;
                        } else {
                            valid_input = 0;
                            break;
                        }
                    }
                }
                
                if (char_index > 0 && word_index < MAX_WORDS) {
                    word_list.words[word_index][char_index] = '\0';
                    word_list.count++;
                }
                
                if (valid_input && word_list.count > 0) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                for (int i = 0; i < word_list.count; i++) {
                    for (int j = 0; word_list.words[i][j] != '\0'; j++) {
                        if (word_list.words[i][j] >= 'a' && word_list.words[i][j] <= 'z') {
                            word_list.words[i][j] = (char)(word_list.words[i][j] - 32);
                        }
                    }
                }
                TRANSITION(STATE_WRITING);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_WRITING) {
        printf("Processed words (%d):\n", word_list.count);
        for (int i = 0; i < word_list.count; i++) {
            printf("%d: %s\n", i + 1, word_list.words[i]);
        }
    } else {
        printf("Error: Invalid input or processing error\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}