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
#define IS_VALID_CHAR(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ' || c == '\n' || c == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int processed_count;
} WordData;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

void initialize_word_data(WordData* data) {
    data->word_count = 0;
    data->processed_count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(data->words[i], 0, MAX_WORD_LEN);
    }
}

int extract_words(const char* input, WordData* data) {
    if (input == NULL || data == NULL) return 0;
    
    int word_idx = 0;
    int char_idx = 0;
    int in_word = 0;
    
    for (size_t i = 0; input[i] != '\0' && i < MAX_INPUT_LEN; i++) {
        char c = input[i];
        
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            if (!in_word) {
                in_word = 1;
                if (word_idx >= MAX_WORDS) return 0;
                char_idx = 0;
            }
            if (char_idx < MAX_WORD_LEN - 1) {
                data->words[word_idx][char_idx++] = c;
            }
        } else if (c == ' ' || c == '\n') {
            if (in_word) {
                data->words[word_idx][char_idx] = '\0';
                word_idx++;
                in_word = 0;
            }
        }
    }
    
    if (in_word && word_idx < MAX_WORDS) {
        data->words[word_idx][char_idx] = '\0';
        word_idx++;
    }
    
    data->word_count = word_idx;
    return 1;
}

void process_words(WordData* data) {
    if (data == NULL) return;
    
    for (int i = 0; i < data->word_count; i++) {
        for (int j = 0; data->words[i][j] != '\0'; j++) {
            if (data->words[i][j] >= 'a' && data->words[i][j] <= 'z') {
                data->words[i][j] = data->words[i][j] - 'a' + 'A';
            }
        }
    }
    data->processed_count = data->word_count;
}

void output_results(const WordData* data) {
    if (data == NULL) return;
    
    printf("Processed %d words:\n", data->processed_count);
    for (int i = 0; i < data->processed_count; i++) {
        printf("%s\n", data->words[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordData word_data;
    int current_state = STATE_IDLE;
    int result;
    
    printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        TRANSITION(STATE_READING);
    }
    
    if (current_state == STATE_READING) {
        if (!validate_input(input)) {
            TRANSITION(STATE_ERROR);
        } else {
            initialize_word_data(&word_data);
            result = extract_words(input, &word_data);
            if (!result || word_data.word_count == 0) {
                TRANSITION(STATE_ERROR);
            } else {
                TRANSITION(STATE_PROCESSING);
            }
        }
    }
    
    if (current_state == STATE_PROCESSING) {
        process_words(&word_data);
        TRANSITION(STATE_WRITING);
    }
    
    if (current_state == STATE_WRITING) {
        output_results(&word_data);
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing failed\n");
        return 1;
    }
    
    return 0;
}