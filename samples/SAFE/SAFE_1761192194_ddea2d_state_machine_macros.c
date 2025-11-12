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

int process_word(WordData* data, const char* word) {
    if (data->word_count >= MAX_WORDS) return 0;
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    
    strncpy(data->words[data->word_count], word, MAX_WORD_LEN - 1);
    data->words[data->word_count][MAX_WORD_LEN - 1] = '\0';
    data->word_count++;
    return 1;
}

int extract_words(const char* input, WordData* data) {
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    char* token = strtok(buffer, " \t\n");
    while (token != NULL && data->word_count < MAX_WORDS) {
        if (!process_word(data, token)) {
            return 0;
        }
        token = strtok(NULL, " \t\n");
    }
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
        printf("%d: %s\n", i + 1, data->words[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    WordData word_data;
    
    initialize_word_data(&word_data);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                
                if (!validate_input(input)) {
                    printf("Invalid input\n");
                    TRANSITION(STATE_ERROR);
                    break;
                }
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                if (!extract_words(input, &word_data)) {
                    printf("Failed to extract words\n");
                    TRANSITION(STATE_ERROR);
                    break;
                }
                TRANSITION(STATE_PROCESSING);
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
        
        if (current_state == STATE_IDLE) {
            char response[10];
            printf("Process another? (y/n): ");
            if (fgets(response, sizeof(response), stdin) == NULL) {
                TRANSITION(STATE_ERROR);
                break;
            }
            if (response[0] != 'y' && response[0] != 'Y') {
                break;
            }
            initialize_word_data(&word_data);
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("State machine encountered an error\n");
        return 1;
    }
    
    printf("State machine completed successfully\n");
    return 0;
}