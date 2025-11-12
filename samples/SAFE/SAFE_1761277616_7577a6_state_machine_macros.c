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

#define TRANSITION(state_var, new_state) do { state_var = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? (c - 'a' + 'A') : c)

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int char_count;
} TextData;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

void initialize_data(TextData* data) {
    if (data == NULL) return;
    data->word_count = 0;
    data->char_count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(data->words[i], 0, MAX_WORD_LEN);
    }
}

int process_character(char c, TextData* data, int* char_index) {
    if (data == NULL || char_index == NULL) return 0;
    if (!IS_VALID_CHAR(c)) return 0;
    
    if (data->word_count >= MAX_WORDS) return 0;
    if (*char_index < 0 || *char_index >= MAX_WORD_LEN - 1) return 0;
    
    data->words[data->word_count][*char_index] = TO_UPPER(c);
    (*char_index)++;
    data->char_count++;
    
    return 1;
}

int process_text(const char* input, TextData* data, int* state_var) {
    if (input == NULL || data == NULL || state_var == NULL) {
        if (state_var) *state_var = STATE_ERROR;
        return 0;
    }
    
    int current_word = 0;
    int char_index = 0;
    int in_word = 0;
    size_t input_len = strlen(input);
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        if (IS_VALID_CHAR(c)) {
            if (!in_word) {
                in_word = 1;
                current_word = data->word_count;
                char_index = 0;
            }
            
            if (!process_character(c, data, &char_index)) {
                *state_var = STATE_ERROR;
                return 0;
            }
        } else {
            if (in_word) {
                if (data->word_count < MAX_WORDS) {
                    data->word_count++;
                }
                in_word = 0;
                if (data->word_count >= MAX_WORDS) break;
            }
        }
    }
    
    if (in_word && data->word_count < MAX_WORDS) {
        data->word_count++;
    }
    
    return 1;
}

void display_results(const TextData* data) {
    if (data == NULL) return;
    printf("Processed %d words, %d characters:\n", data->word_count, data->char_count);
    for (int i = 0; i < data->word_count && i < MAX_WORDS; i++) {
        if (strlen(data->words[i]) > 0) {
            printf("Word %d: %s\n", i + 1, data->words[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    TextData data;
    int current_state = STATE_IDLE;
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(current_state, STATE_ERROR);
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    TRANSITION(current_state, STATE_READING);
    
    if (!validate_input(input)) {
        TRANSITION(current_state, STATE_ERROR);
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    TRANSITION(current_state, STATE_PROCESSING);
    initialize_data(&data);
    
    if (!process_text(input, &data, &current_state)) {
        printf("Error processing text\n");
        return EXIT_FAILURE;
    }
    
    TRANSITION(current_state, STATE_WRITING);
    display_results(&data);
    
    return EXIT_SUCCESS;
}