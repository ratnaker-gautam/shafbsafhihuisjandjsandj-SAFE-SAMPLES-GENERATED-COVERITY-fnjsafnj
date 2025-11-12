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
#define IS_VALID_CHAR(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')

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

int parse_words(const char* input, WordData* data) {
    if (input == NULL || data == NULL) return 0;
    
    data->word_count = 0;
    const char* ptr = input;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    while (*ptr && data->word_count < MAX_WORDS) {
        if (*ptr == ' ') {
            if (word_len > 0) {
                if (word_len >= MAX_WORD_LEN) return 0;
                memcpy(data->words[data->word_count], current_word, word_len);
                data->words[data->word_count][word_len] = '\0';
                data->word_count++;
                word_len = 0;
            }
        } else {
            if (word_len >= MAX_WORD_LEN - 1) return 0;
            current_word[word_len++] = *ptr;
        }
        ptr++;
    }
    
    if (word_len > 0) {
        if (word_len >= MAX_WORD_LEN) return 0;
        memcpy(data->words[data->word_count], current_word, word_len);
        data->words[data->word_count][word_len] = '\0';
        data->word_count++;
    }
    
    return data->word_count > 0;
}

void process_words(WordData* data) {
    if (data == NULL) return;
    
    data->processed_count = 0;
    for (int i = 0; i < data->word_count; i++) {
        if (strlen(data->words[i]) > 0) {
            for (char* p = data->words[i]; *p; p++) {
                if (*p >= 'a' && *p <= 'z') {
                    *p = *p - 'a' + 'A';
                }
            }
            data->processed_count++;
        }
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
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    WordData word_data;
    int result;
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                size_t len = strlen(input_buffer);
                if (len > 0 && input_buffer[len - 1] == '\n') {
                    input_buffer[len - 1] = '\0';
                }
                
                if (validate_input(input_buffer)) {
                    TRANSITION(STATE_READING);
                } else {
                    printf("Invalid input. Only letters and spaces allowed.\n");
                    TRANSITION(STATE_IDLE);
                }
                break;
                
            case STATE_READING:
                if (parse_words(input_buffer, &word_data)) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    printf("Failed to parse words.\n");
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                process_words(&word_data);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                output_results(&word_data);
                printf("Processing complete. Continue? (y/n): ");
                
                char response[10];
                if (fgets(response, sizeof(response), stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (response[0] == 'y' || response[0] == 'Y') {
                    TRANSITION(STATE_IDLE);
                } else {
                    printf("Exiting.\n");
                    return 0;
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    printf("Error occurred during processing.\n");
    return 1;
}