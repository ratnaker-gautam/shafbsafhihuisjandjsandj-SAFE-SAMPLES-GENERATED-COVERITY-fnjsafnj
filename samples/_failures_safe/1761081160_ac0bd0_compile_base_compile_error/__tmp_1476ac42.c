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
    int word_start = -1;
    int i = 0;
    int k, j;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
        printf("Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        TRANSITION(STATE_ERROR);
        printf("Empty input\n");
        return 1;
    }
    
    TRANSITION(STATE_READING);
    
    for (i = 0; i < input_len && current_state != STATE_ERROR; i++) {
        switch (current_state) {
            case STATE_READING:
                if (IS_VALID_CHAR(input[i])) {
                    if (word_start == -1) {
                        word_start = i;
                    }
                } else {
                    if (word_start != -1) {
                        int word_len = i - word_start;
                        if (word_len >= MAX_WORD_LEN) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        if (data.word_count >= MAX_WORDS) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        strncpy(data.words[data.word_count], &input[word_start], word_len);
                        data.words[data.word_count][word_len] = '\0';
                        data.word_count++;
                        word_start = -1;
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (data.processed_count < data.word_count) {
                    char *word = data.words[data.processed_count];
                    int len = strlen(word);
                    for (j = 0; j < len; j++) {
                        if (word[j] >= 'a' && word[j] <= 'z') {
                            word[j] = word[j] - 'a' + 'A';
                        }
                    }
                    data.processed_count++;
                } else {
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed words (%d):\n", data.word_count);
                for (k = 0; k < data.word_count; k++) {
                    printf("%d: %s\n", k + 1, data.words[k]);
                }
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (i == input_len - 1 && current_state == STATE_READING) {
            if (word_start != -1) {
                int word_len = i - word_start + 1;
                if (word_len >= MAX_WORD_LEN) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                if (data.word_count >= MAX_WORDS) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                strncpy(data.words[data.word_count], &input[word_start], word_len);
                data.words[data.word_count][word_len] = '\0';
                data.word_count++;
            }
            TRANSITION(STATE_PROCESSING);
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error processing input\n");
        return 1;
    }
    
    while (current_state != STATE_IDLE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_PROCESSING:
                if (data.processed_count < data.word_count) {
                    char *word = data.words[data.processed_count];
                    int len = strlen(word);
                    for (j = 0; j < len; j++) {
                        if (word[j] >= 'a' && word[j] <= 'z') {
                            word[j] = word[j] - 'a' + 'A';
                        }
                    }
                    data.processed_count++;
                } else {
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed words (%d):\n", data.word_count);
                for (k = 0; k < data.word_count; k++) {
                    printf("%d: %s\n", k + 1, data.words[k]);
                }
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;