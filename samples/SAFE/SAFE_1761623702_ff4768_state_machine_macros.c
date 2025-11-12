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
        printf("Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        TRANSITION(STATE_ERROR);
        printf("Input too long\n");
        return 1;
    }
    
    for (int i = 0; i < input_len; i++) {
        if (!IS_VALID_CHAR(input[i])) {
            TRANSITION(STATE_ERROR);
            printf("Invalid character in input\n");
            return 1;
        }
    }
    
    TRANSITION(STATE_READING);
    
    for (int i = 0; i <= input_len; i++) {
        if (current_state == STATE_READING) {
            if (input[i] == ' ' || input[i] == '\0') {
                if (word_start != -1) {
                    int word_len = i - word_start;
                    if (word_len >= MAX_WORD_LEN) {
                        TRANSITION(STATE_ERROR);
                        printf("Word too long\n");
                        return 1;
                    }
                    if (word_count >= MAX_WORDS) {
                        TRANSITION(STATE_ERROR);
                        printf("Too many words\n");
                        return 1;
                    }
                    
                    TRANSITION(STATE_PROCESSING);
                    strncpy(word_list.words[word_count], &input[word_start], word_len);
                    word_list.words[word_count][word_len] = '\0';
                    word_list.count = ++word_count;
                    word_start = -1;
                    TRANSITION(STATE_READING);
                }
            } else if (word_start == -1) {
                word_start = i;
            }
        }
    }
    
    TRANSITION(STATE_WRITING);
    
    printf("Found %d words:\n", word_list.count);
    for (int i = 0; i < word_list.count; i++) {
        printf("%d: %s\n", i + 1, word_list.words[i]);
    }
    
    return 0;
}