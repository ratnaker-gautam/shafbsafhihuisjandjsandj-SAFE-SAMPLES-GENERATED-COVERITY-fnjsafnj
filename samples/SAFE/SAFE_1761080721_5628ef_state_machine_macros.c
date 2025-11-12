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

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
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
    if (!validate_input(input)) return 0;
    
    int word_idx = 0;
    int char_idx = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < strlen(input); i++) {
        char c = input[i];
        
        if (IS_VALID_CHAR(c)) {
            if (!in_word) {
                in_word = 1;
                if (word_idx >= MAX_WORDS) return 0;
                char_idx = 0;
            }
            if (char_idx < MAX_WORD_LEN - 1) {
                data->words[word_idx][char_idx++] = c;
            }
        } else {
            if (in_word) {
                data->words[word_idx][char_idx] = '\0';
                word_idx++;
                in_word = 0;
                char_idx = 0;
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
    printf("Processed %d words:\n", data->processed_count);
    for (int i = 0; i < data->processed_count; i++) {
        printf("%s\n", data->words[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    WordData word_data;
    
    initialize_word_data(&word_data);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    size_t len = strlen(input_buffer);
                    if (len > 0 && input_buffer[len - 1] == '\n') {
                        input_buffer[len - 1] = '\0';
                    }
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (extract_words(input_buffer, &word_data)) {
                    if (word_data.word_count > 0) {
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        printf("No valid words found.\n");
                        TRANSITION(STATE_IDLE);
                    }
                } else {
                    printf("Invalid input or too many words.\n");
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                process_words(&word_data);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                output_results(&word_data);
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    printf("State machine terminated with error.\n");
    return 1;
}