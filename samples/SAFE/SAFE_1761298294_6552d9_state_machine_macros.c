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
    if (input == NULL) return 0;
    int len = strlen(input);
    return CHECK_BOUNDS(len, max_len);
}

void initialize_data(WordData* data) {
    memset(data, 0, sizeof(WordData));
}

int parse_words(const char* input, WordData* data) {
    if (!validate_input(input, MAX_INPUT_LEN)) return 0;
    
    int input_len = strlen(input);
    int word_start = -1;
    data->word_count = 0;
    
    for (int i = 0; i <= input_len && data->word_count < MAX_WORDS; i++) {
        char c = input[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (CHECK_BOUNDS(word_len, MAX_WORD_LEN)) {
                    strncpy(data->words[data->word_count], &input[word_start], word_len);
                    data->words[data->word_count][word_len] = '\0';
                    data->word_count++;
                }
                word_start = -1;
            }
        }
    }
    
    return data->word_count > 0;
}

void process_words(WordData* data) {
    data->processed_count = 0;
    for (int i = 0; i < data->word_count; i++) {
        if (strlen(data->words[i]) > 0) {
            for (int j = 0; data->words[i][j] != '\0'; j++) {
                if (data->words[i][j] >= 'a' && data->words[i][j] <= 'z') {
                    data->words[i][j] = data->words[i][j] - 'a' + 'A';
                }
            }
            data->processed_count++;
        }
    }
}

void output_results(const WordData* data) {
    printf("Processed %d words:\n", data->processed_count);
    for (int i = 0; i < data->word_count; i++) {
        if (strlen(data->words[i]) > 0) {
            printf("%s\n", data->words[i]);
        }
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    WordData word_data;
    
    initialize_data(&word_data);
    
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
                if (validate_input(input_buffer, MAX_INPUT_LEN)) {
                    if (parse_words(input_buffer, &word_data)) {
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        printf("No valid words found.\n");
                        TRANSITION(STATE_IDLE);
                    }
                } else {
                    printf("Invalid input.\n");
                    TRANSITION(STATE_IDLE);
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
    
    printf("State machine encountered an error.\n");
    return 1;
}