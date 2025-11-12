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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ')

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

int parse_words(const char* input, WordData* data) {
    if (input == NULL || data == NULL) return 0;
    
    int word_idx = 0;
    int char_idx = 0;
    int in_word = 0;
    
    for (size_t i = 0; input[i] != '\0' && i < MAX_INPUT_LEN; i++) {
        char c = input[i];
        
        if (c == ' ') {
            if (in_word) {
                if (word_idx < MAX_WORDS - 1) {
                    word_idx++;
                    char_idx = 0;
                }
                in_word = 0;
            }
        } else {
            if (!in_word) {
                in_word = 1;
            }
            if (char_idx < MAX_WORD_LEN - 1) {
                data->words[word_idx][char_idx++] = c;
            }
        }
    }
    
    data->word_count = in_word ? word_idx + 1 : word_idx;
    return data->word_count > 0;
}

void process_words(WordData* data) {
    if (data == NULL) return;
    
    for (int i = 0; i < data->word_count; i++) {
        for (int j = 0; data->words[i][j] != '\0'; j++) {
            if (data->words[i][j] >= 'a' && data->words[i][j] <= 'z') {
                data->words[i][j] = data->words[i][j] - 'a' + 'A';
            }
        }
        data->processed_count++;
    }
}

void output_results(const WordData* data) {
    if (data == NULL) return;
    
    printf("Processed %d words:\n", data->processed_count);
    for (int i = 0; i < data->word_count; i++) {
        if (data->words[i][0] != '\0') {
            printf("%d: %s\n", i + 1, data->words[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    WordData word_data;
    int current_state = STATE_IDLE;
    int should_continue = 1;
    
    printf("Enter text to process (alphanumeric characters and spaces only): ");
    
    while (should_continue) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                    }
                    if (validate_input(input)) {
                        TRANSITION(STATE_READING);
                    } else {
                        printf("Invalid input. Please use only alphanumeric characters and spaces.\n");
                        printf("Enter text: ");
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                initialize_word_data(&word_data);
                if (parse_words(input, &word_data)) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    printf("No valid words found.\n");
                    TRANSITION(STATE_IDLE);
                    printf("Enter text: ");
                }
                break;
                
            case STATE_PROCESSING:
                process_words(&word_data);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                output_results(&word_data);
                TRANSITION(STATE_IDLE);
                printf("\nEnter text to process (or 'quit' to exit): ");
                break;
                
            case STATE_ERROR:
                printf("Input error occurred.\n");
                should_continue = 0;
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (current_state == STATE_IDLE) {
            if (