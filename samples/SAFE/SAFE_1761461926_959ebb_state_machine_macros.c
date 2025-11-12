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
#define IS_TERMINATOR(c) (c == '\n' || c == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int char_count;
} TextData;

int main(void) {
    TextData data = {0};
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1] = {0};
    int input_len = 0;
    int word_index = 0;
    int char_index = 0;
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
                if (input_len >= MAX_INPUT_LEN) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (i = 0; i < input_len && word_index < MAX_WORDS; i++) {
                    if (IS_VALID_CHAR(input[i])) {
                        if (char_index < MAX_WORD_LEN - 1) {
                            data.words[word_index][char_index] = input[i];
                            char_index++;
                            data.char_count++;
                        }
                    } else if (IS_TERMINATOR(input[i])) {
                        if (char_index > 0) {
                            data.words[word_index][char_index] = '\0';
                            word_index++;
                            data.word_count++;
                            char_index = 0;
                        }
                        break;
                    } else {
                        if (char_index > 0) {
                            data.words[word_index][char_index] = '\0';
                            word_index++;
                            data.word_count++;
                            char_index = 0;
                        }
                    }
                }
                
                if (char_index > 0 && word_index < MAX_WORDS) {
                    data.words[word_index][char_index] = '\0';
                    word_index++;
                    data.word_count++;
                }
                
                TRANSITION(STATE_PROCESSING);
                break;
                
            case STATE_PROCESSING:
                if (data.word_count == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (i = 0; i < data.word_count; i++) {
                    for (int j = 0; data.words[i][j] != '\0'; j++) {
                        if (data.words[i][j] >= 'a' && data.words[i][j] <= 'z') {
                            data.words[i][j] = data.words[i][j] - 'a' + 'A';
                        }
                    }
                }
                
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed %d words (%d characters):\n", data.word_count, data.char_count);
                for (i = 0; i < data.word_count; i++) {
                    if (i > 0) printf(" ");
                    printf("%s", data.words[i]);
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