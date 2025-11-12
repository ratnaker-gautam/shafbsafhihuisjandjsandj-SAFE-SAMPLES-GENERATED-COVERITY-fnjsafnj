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
#define IS_TERMINATOR(c) (c == '\n' || c == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int current_word_len;
} WordData;

void initialize_word_data(WordData *data) {
    data->word_count = 0;
    data->current_word_len = 0;
    memset(data->words, 0, sizeof(data->words));
}

int process_input(const char *input, WordData *data) {
    int current_state = STATE_IDLE;
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        return STATE_ERROR;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_IDLE:
                if (IS_VALID_CHAR(c)) {
                    if (data->word_count >= MAX_WORDS) {
                        return STATE_ERROR;
                    }
                    if (data->current_word_len >= MAX_WORD_LEN - 1) {
                        return STATE_ERROR;
                    }
                    data->words[data->word_count][0] = c;
                    data->current_word_len = 1;
                    TRANSITION(current_state, STATE_READING);
                } else if (!IS_TERMINATOR(c) && c != ' ') {
                    return STATE_ERROR;
                }
                break;
                
            case STATE_READING:
                if (IS_VALID_CHAR(c)) {
                    if (data->current_word_len >= MAX_WORD_LEN - 1) {
                        return STATE_ERROR;
                    }
                    data->words[data->word_count][data->current_word_len++] = c;
                } else if (c == ' ' || IS_TERMINATOR(c)) {
                    if (data->current_word_len >= MAX_WORD_LEN) {
                        return STATE_ERROR;
                    }
                    data->words[data->word_count][data->current_word_len] = '\0';
                    data->word_count++;
                    data->current_word_len = 0;
                    TRANSITION(current_state, c == ' ' ? STATE_IDLE : STATE_PROCESSING);
                } else {
                    return STATE_ERROR;
                }
                break;
                
            case STATE_PROCESSING:
                if (data->word_count > 0) {
                    TRANSITION(current_state, STATE_WRITING);
                } else {
                    return STATE_ERROR;
                }
                break;
                
            case STATE_WRITING:
                return current_state;
                
            case STATE_ERROR:
                return current_state;
        }
    }
    
    return current_state;
}

void print_words(const WordData *data) {
    printf("Found %d words:\n", data->word_count);
    for (int i = 0; i < data->word_count; i++) {
        printf("%d: %s\n", i + 1, data->words[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    WordData word_data;
    
    initialize_word_data(&word_data);
    
    printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("Empty input\n");
        return EXIT_FAILURE;
    }
    
    int final_state = process_input(input, &word_data);
    
    switch (final_state) {
        case STATE_WRITING:
            print_words(&word_data);
            break;
        case STATE_ERROR:
            printf("Error: Invalid input format\n");
            return EXIT_FAILURE;
        default:
            printf("Error: Unexpected state %d\n", final_state);
            return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}