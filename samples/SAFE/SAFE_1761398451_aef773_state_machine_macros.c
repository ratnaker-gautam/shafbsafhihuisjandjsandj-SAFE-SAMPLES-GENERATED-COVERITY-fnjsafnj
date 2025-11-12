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
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int processed_count;
} WordData;

int validate_input(const char* input, int max_len) {
    if (!input) return 0;
    int len = strlen(input);
    return CHECK_BOUNDS(len, max_len);
}

void initialize_data(WordData* data) {
    memset(data, 0, sizeof(WordData));
}

int parse_words(const char* input, WordData* data) {
    if (!input || !data) return 0;
    
    int input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) return 0;
    
    int word_start = -1;
    data->word_count = 0;
    
    for (int i = 0; i <= input_len; i++) {
        char c = input[i];
        if (c == ' ' || c == '\t' || c == '\0') {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (word_len >= MAX_WORD_LEN || data->word_count >= MAX_WORDS) {
                    return 0;
                }
                strncpy(data->words[data->word_count], &input[word_start], word_len);
                data->words[data->word_count][word_len] = '\0';
                data->word_count++;
                word_start = -1;
            }
        } else if (word_start == -1) {
            word_start = i;
        }
    }
    return 1;
}

int process_words(WordData* data) {
    if (!data || data->word_count == 0) return 0;
    
    data->processed_count = 0;
    for (int i = 0; i < data->word_count; i++) {
        if (strlen(data->words[i]) > 0) {
            for (int j = 0; data->words[i][j]; j++) {
                if (data->words[i][j] >= 'a' && data->words[i][j] <= 'z') {
                    data->words[i][j] = data->words[i][j] - 'a' + 'A';
                }
            }
            data->processed_count++;
        }
    }
    return data->processed_count > 0;
}

int write_output(const WordData* data) {
    if (!data || data->processed_count == 0) return 0;
    
    printf("Processed words (%d):\n", data->processed_count);
    for (int i = 0; i < data->word_count; i++) {
        if (strlen(data->words[i]) > 0) {
            printf("%s\n", data->words[i]);
        }
    }
    return 1;
}

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    WordData word_data;
    
    initialize_data(&word_data);
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (!validate_input(input, MAX_INPUT_LEN)) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    switch (current_state) {
        case STATE_READING:
            if (parse_words(input, &word_data)) {
                TRANSITION(STATE_PROCESSING);
            } else {
                TRANSITION(STATE_ERROR);
            }
            break;
        default:
            break;
    }
    
    switch (current_state) {
        case STATE_PROCESSING:
            if (process_words(&word_data)) {
                TRANSITION(STATE_WRITING);
            } else {
                TRANSITION(STATE_ERROR);
            }
            break;
        default:
            break;
    }
    
    switch (current_state) {
        case STATE_WRITING:
            if (!write_output(&word_data)) {
                TRANSITION(STATE_ERROR);
            }
            break;
        default:
            break;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing failed\n");
        return 1;
    }
    
    return 0;
}