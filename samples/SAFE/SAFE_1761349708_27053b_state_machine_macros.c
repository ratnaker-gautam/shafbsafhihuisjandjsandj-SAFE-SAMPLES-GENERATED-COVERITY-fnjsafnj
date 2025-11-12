//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_WORDS 20
#define MAX_WORD_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_ALPHA(c) (isalpha((unsigned char)c))
#define IS_SPACE(c) (isspace((unsigned char)c))

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int word_index = 0;
    int char_index = 0;
    
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
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (input_len == 0) {
                    TRANSITION(STATE_DONE);
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (word_count >= MAX_WORDS) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                while (word_index < input_len && IS_SPACE(input[word_index])) {
                    word_index++;
                }
                
                if (word_index >= input_len) {
                    TRANSITION(STATE_DONE);
                    break;
                }
                
                char_index = 0;
                while (word_index < input_len && char_index < MAX_WORD_LEN && 
                       IS_ALPHA(input[word_index])) {
                    words[word_count][char_index] = tolower((unsigned char)input[word_index]);
                    char_index++;
                    word_index++;
                }
                
                if (char_index > 0) {
                    words[word_count][char_index] = '\0';
                    word_count++;
                }
                
                if (word_index < input_len && !IS_SPACE(input[word_index])) {
                    TRANSITION(STATE_ERROR);
                } else {
                    TRANSITION(STATE_READING);
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                return 1;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_DONE) {
        printf("Found %d words:\n", word_count);
        for (int i = 0; i < word_count; i++) {
            if (i < MAX_WORDS) {
                printf("%d: %s\n", i + 1, words[i]);
            }
        }
    }
    
    return 0;
}