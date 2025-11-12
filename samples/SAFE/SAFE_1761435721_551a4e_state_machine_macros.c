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
#define IS_TERMINATOR(c) (c == '\n' || c == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int char_count;
} text_data_t;

static int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

static void reset_data(text_data_t* data) {
    data->word_count = 0;
    data->char_count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(data->words[i], 0, MAX_WORD_LEN);
    }
}

static int process_character(char c, text_data_t* data, int* char_index) {
    if (!IS_VALID_CHAR(c)) return 0;
    
    if (data->word_count >= MAX_WORDS) return 0;
    if (*char_index >= MAX_WORD_LEN - 1) return 0;
    
    data->words[data->word_count][*char_index] = c;
    (*char_index)++;
    data->char_count++;
    return 1;
}

static void process_text(const char* input, text_data_t* data) {
    reset_data(data);
    int current_word = 0;
    int char_index = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < strlen(input) && i < MAX_INPUT_LEN; i++) {
        char c = input[i];
        
        if (IS_VALID_CHAR(c)) {
            if (!in_word) {
                in_word = 1;
                current_word = data->word_count;
                char_index = 0;
            }
            if (!process_character(c, data, &char_index)) {
                break;
            }
        } else if (IS_TERMINATOR(c)) {
            break;
        } else {
            if (in_word) {
                data->word_count++;
                in_word = 0;
                char_index = 0;
            }
        }
    }
    
    if (in_word && data->word_count < MAX_WORDS) {
        data->word_count++;
    }
}

static void print_results(const text_data_t* data) {
    printf("Processed %d words with %d total characters:\n", 
           data->word_count, data->char_count);
    for (int i = 0; i < data->word_count && i < MAX_WORDS; i++) {
        if (strlen(data->words[i]) > 0) {
            printf("Word %d: %s\n", i + 1, data->words[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    text_data_t data;
    int current_state = STATE_IDLE;
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        TRANSITION(STATE_READING);
    }
    
    if (current_state == STATE_READING) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (!validate_input(input)) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_PROCESSING);
        }
    }
    
    if (current_state == STATE_PROCESSING) {
        process_text(input, &data);
        if (data.word_count == 0) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_WRITING);
        }
    }
    
    if (current_state == STATE_WRITING) {
        print_results(&data);
    } else if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}