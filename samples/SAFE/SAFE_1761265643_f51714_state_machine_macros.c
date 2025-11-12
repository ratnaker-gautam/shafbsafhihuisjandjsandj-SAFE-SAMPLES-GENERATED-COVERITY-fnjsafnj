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
#define IS_ALPHA(c) (isalpha((unsigned char)(c)))
#define IS_SPACE(c) (isspace((unsigned char)(c)))

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int current_state = STATE_IDLE;
    int pos = 0;
    int word_pos = 0;
    int i;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        TRANSITION(STATE_READING);
    }
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (pos >= MAX_INPUT_LEN || input[pos] == '\0') {
                    TRANSITION(STATE_PROCESSING);
                } else if (input[pos] == '\n') {
                    input[pos] = '\0';
                    TRANSITION(STATE_PROCESSING);
                } else {
                    pos++;
                }
                break;
                
            case STATE_PROCESSING:
                if (pos >= MAX_INPUT_LEN || input[pos] == '\0') {
                    if (word_pos > 0 && word_count < MAX_WORDS) {
                        words[word_count][word_pos] = '\0';
                        word_count++;
                    }
                    TRANSITION(STATE_DONE);
                } else if (IS_SPACE(input[pos])) {
                    if (word_pos > 0 && word_count < MAX_WORDS) {
                        words[word_count][word_pos] = '\0';
                        word_count++;
                        word_pos = 0;
                    }
                    pos++;
                } else if (IS_ALPHA(input[pos]) && word_count < MAX_WORDS) {
                    if (word_pos < MAX_WORD_LEN) {
                        words[word_count][word_pos] = tolower((unsigned char)input[pos]);
                        word_pos++;
                    }
                    pos++;
                } else {
                    pos++;
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error processing input\n");
        return 1;
    }
    
    printf("Found %d words:\n", word_count);
    for (i = 0; i < word_count; i++) {
        if (words[i][0] != '\0') {
            printf("%d: %s\n", i + 1, words[i]);
        }
    }
    
    return 0;
}