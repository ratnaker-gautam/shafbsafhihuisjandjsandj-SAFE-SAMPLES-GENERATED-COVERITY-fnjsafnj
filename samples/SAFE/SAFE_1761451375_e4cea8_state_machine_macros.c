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
    char output[MAX_INPUT_LEN + 1];
    int word_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_pos = 0;
    int char_pos = 0;
    int i, j;
    
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
    
    while (current_state != STATE_ERROR && current_state != STATE_IDLE) {
        switch (current_state) {
            case STATE_READING:
                if (input_len >= MAX_INPUT_LEN) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (i = 0; i < input_len && word_count < MAX_WORDS; i++) {
                    if (IS_VALID_CHAR(input[i])) {
                        if (word_pos == 0) {
                            char_pos = 0;
                        }
                        if (char_pos < MAX_WORD_LEN) {
                            words[word_count][char_pos++] = input[i];
                        }
                        word_pos = 1;
                    } else {
                        if (word_pos > 0 && char_pos > 0) {
                            words[word_count][char_pos] = '\0';
                            word_count++;
                            word_pos = 0;
                            char_pos = 0;
                        }
                    }
                }
                
                if (word_pos > 0 && char_pos > 0 && word_count < MAX_WORDS) {
                    words[word_count][char_pos] = '\0';
                    word_count++;
                }
                
                if (word_count > 0) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                output[0] = '\0';
                for (i = 0; i < word_count; i++) {
                    int word_len = strlen(words[i]);
                    if (word_len == 0) continue;
                    
                    words[i][0] = TO_UPPER(words[i][0]);
                    for (j = 1; j < word_len; j++) {
                        words[i][j] = TO_UPPER(words[i][j]);
                    }
                }
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                for (i = 0; i < word_count; i++) {
                    if (strlen(output) + strlen(words[i]) + 2 < sizeof(output)) {
                        if (i > 0) {
                            strcat(output, " ");
                        }
                        strcat(output, words[i]);
                    }
                }
                
                printf("Processed text: %s\n", output);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or processing error\n");
                return EXIT_FAILURE;
                break;
        }
    }
    
    return EXIT_SUCCESS;
}