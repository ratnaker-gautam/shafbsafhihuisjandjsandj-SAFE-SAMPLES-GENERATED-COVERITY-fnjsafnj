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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '\n' || c == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int current_word_len;
} WordData;

void initialize_word_data(WordData *data) {
    if (data == NULL) return;
    data->word_count = 0;
    data->current_word_len = 0;
    memset(data->words, 0, sizeof(data->words));
}

int process_input(const char *input, WordData *data) {
    if (input == NULL || data == NULL) {
        return STATE_ERROR;
    }
    
    int current_state = STATE_IDLE;
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        return STATE_ERROR;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (!IS_VALID_CHAR(c)) {
            return STATE_ERROR;
        }
        
        switch (current_state) {
            case STATE_IDLE:
                if (c != ' ' && c != '\n' && c != '\0') {
                    if (data->word_count >= MAX_WORDS) {
                        return STATE_ERROR;
                    }
                    data->current_word_len = 0;
                    if (data->current_word_len < MAX_WORD_LEN - 1) {
                        data->words[data->word_count][data->current_word_len++] = c;
                    }
                    TRANSITION(current_state, STATE_READING);
                }
                break;
                
            case STATE_READING:
                if (c == ' ' || c == '\n' || c == '\0') {
                    if (data->current_word_len > 0) {
                        if (data->current_word_len < MAX_WORD_LEN) {
                            data->words[data->word_count][data->current_word_len] = '\0';
                        }
                        data->word_count++;
                        if (data->word_count <= MAX_WORDS) {
                            TRANSITION(current_state, STATE_PROCESSING);
                        } else {
                            return STATE_ERROR;
                        }
                    }
                } else {
                    if (data->current_word_len >= MAX_WORD_LEN - 1) {
                        return STATE_ERROR;
                    }
                    if (data->current_word_len < MAX_WORD_LEN - 1) {
                        data->words[data->word_count][data->current_word_len++] = c;
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (c != ' ' && c != '\n' && c != '\0') {
                    if (data->word_count >= MAX_WORDS) {
                        return STATE_ERROR;
                    }
                    data->current_word_len = 0;
                    if (data->current_word_len < MAX_WORD_LEN - 1) {
                        data->words[data->word_count][data->current_word_len++] = c;
                    }
                    TRANSITION(current_state, STATE_READING);
                } else if (c == '\0') {
                    TRANSITION(current_state, STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                break;
                
            default:
                return STATE_ERROR;
        }
        
        if (current_state == STATE_WRITING) {
            break;
        }
    }
    
    return current_state;
}

void print_words(const WordData *data) {
    if (data == NULL) return;
    printf("Found %d words:\n", data->word_count);
    for (int i = 0; i < data->word_count && i < MAX_WORDS; i++) {
        if (data->words[i][0] != '\0') {
            printf("%d: %s\n", i + 1, data->words[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    WordData word_data;
    
    printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return EXIT_FAILURE;
    }
    
    initialize_word_data(&word_data);
    
    int final_state = process_input(input, &word_data);
    
    switch (final_state) {
        case STATE_WRITING:
            print_words(&word_data);
            break;
        case STATE_ERROR:
            printf("Error: Invalid input or buffer overflow\n");
            return EXIT_FAILURE;
        default:
            printf("Error: