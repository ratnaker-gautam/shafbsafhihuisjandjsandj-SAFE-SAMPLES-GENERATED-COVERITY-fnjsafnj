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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))

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
    int word_start = -1;
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    
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
    
    for (int i = 0; current_state != STATE_ERROR && i <= input_len; i++) {
        switch (current_state) {
            case STATE_READING:
                if (i < input_len && IS_VALID_CHAR(input[i])) {
                    if (word_start == -1) {
                        word_start = i;
                    }
                } else {
                    if (word_start != -1) {
                        int word_len = i - word_start;
                        if (word_len >= MAX_WORD_LEN) {
                            word_len = MAX_WORD_LEN - 1;
                        }
                        if (data.word_count < MAX_WORDS) {
                            strncpy(data.words[data.word_count], &input[word_start], word_len);
                            data.words[data.word_count][word_len] = '\0';
                            data.word_count++;
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                        word_start = -1;
                    }
                }
                if (i == input_len) {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (data.word_count > 0) {
                    for (int j = 0; j < data.word_count; j++) {
                        for (int k = 0; data.words[j][k] != '\0'; k++) {
                            if (data.words[j][k] >= 'a' && data.words[j][k] <= 'z') {
                                data.words[j][k] = data.words[j][k] - 'a' + 'A';
                            }
                        }
                        data.processed_count++;
                    }
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed %d words:\n", data.processed_count);
                for (int j = 0; j < data.word_count; j++) {
                    printf("%s\n", data.words[j]);
                }
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error occurred during processing\n");
                return EXIT_FAILURE;
                
            case STATE_IDLE:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error occurred during processing\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}