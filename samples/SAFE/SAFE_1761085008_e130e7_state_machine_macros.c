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
    
    const char* ptr = input;
    char current_word[MAX_WORD_LEN] = {0};
    int word_len = 0;
    
    while (*ptr != '\0' && data->word_count < MAX_WORDS) {
        if (*ptr == ' ') {
            if (word_len > 0) {
                if (word_len < MAX_WORD_LEN) {
                    strncpy(data->words[data->word_count], current_word, word_len);
                    data->word_count++;
                }
                memset(current_word, 0, MAX_WORD_LEN);
                word_len = 0;
            }
        } else {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = *ptr;
            }
        }
        ptr++;
    }
    
    if (word_len > 0 && data->word_count < MAX_WORDS) {
        if (word_len < MAX_WORD_LEN) {
            strncpy(data->words[data->word_count], current_word, word_len);
            data->word_count++;
        }
    }
    
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
        printf("%d: %s\n", i + 1, data->words[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    int current_state = STATE_IDLE;
    WordData word_data;
    
    initialize_word_data(&word_data);
    
    printf("Enter text (alphanumeric characters and spaces only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (!validate_input(input)) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    while (current_state != STATE_ERROR && current_state != STATE_WRITING) {
        switch (current_state) {
            case STATE_READING:
                if (parse_words(input, &word_data)) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                process_words(&word_data);
                if (word_data.processed_count > 0) {
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_WRITING) {
        output_results(&word_data);
        return EXIT_SUCCESS;
    } else {
        printf("Error: Invalid input or processing failure\n");
        return EXIT_FAILURE;
    }
}