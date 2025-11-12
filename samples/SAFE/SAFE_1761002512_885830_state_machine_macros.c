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

static void initialize_data(text_data_t* data) {
    data->word_count = 0;
    data->char_count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(data->words[i], 0, MAX_WORD_LEN);
    }
}

static int process_character(char c, text_data_t* data, int* char_index) {
    if (!IS_VALID_CHAR(c)) {
        if (data->word_count > 0 && *char_index > 0) {
            data->words[data->word_count - 1][*char_index] = '\0';
            *char_index = 0;
        }
        return IS_TERMINATOR(c) ? STATE_PROCESSING : STATE_READING;
    }
    
    if (data->word_count >= MAX_WORDS) return STATE_ERROR;
    
    if (*char_index == 0) {
        data->word_count++;
        if (data->word_count >= MAX_WORDS) return STATE_ERROR;
    }
    
    if (*char_index >= MAX_WORD_LEN - 1) return STATE_ERROR;
    
    data->words[data->word_count - 1][*char_index] = c;
    (*char_index)++;
    data->char_count++;
    
    return STATE_READING;
}

static void display_results(const text_data_t* data) {
    if (data->word_count == 0) {
        printf("No words found.\n");
        return;
    }
    
    printf("Found %d words:\n", data->word_count);
    for (int i = 0; i < data->word_count; i++) {
        if (strlen(data->words[i]) > 0) {
            printf("%d: %s\n", i + 1, data->words[i]);
        }
    }
    printf("Total characters: %d\n", data->char_count);
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    text_data_t data;
    int current_state = STATE_IDLE;
    int char_index = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
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
    
    initialize_data(&data);
    
    for (size_t i = 0; i < strlen(input) && current_state != STATE_ERROR; i++) {
        switch (current_state) {
            case STATE_READING:
                current_state = process_character(input[i], &data, &char_index);
                break;
            case STATE_PROCESSING:
                if (i < strlen(input) - 1) {
                    current_state = process_character(input[i], &data, &char_index);
                }
                break;
            default:
                break;
        }
    }
    
    if (current_state == STATE_READING || current_state == STATE_PROCESSING) {
        if (char_index > 0 && data.word_count > 0) {
            data.words[data.word_count - 1][char_index] = '\0';
        }
        TRANSITION(STATE_WRITING);
    }
    
    if (current_state == STATE_WRITING) {
        display_results(&data);
    } else if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing error.\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}