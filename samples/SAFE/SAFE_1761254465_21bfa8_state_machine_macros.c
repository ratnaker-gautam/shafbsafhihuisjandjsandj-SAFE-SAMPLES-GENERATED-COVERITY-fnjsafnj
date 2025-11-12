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
#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? (c - 32) : c)

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

int process_state_idle(const char* input, WordData* data) {
    if (!validate_input(input)) {
        return STATE_ERROR;
    }
    
    initialize_word_data(data);
    int word_idx = 0;
    int char_idx = 0;
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (IS_VALID_CHAR(input[i])) {
            if (word_idx >= MAX_WORDS || char_idx >= MAX_WORD_LEN - 1) {
                return STATE_ERROR;
            }
            data->words[word_idx][char_idx++] = input[i];
        } else if (char_idx > 0) {
            data->words[word_idx][char_idx] = '\0';
            word_idx++;
            char_idx = 0;
        }
    }
    
    if (char_idx > 0) {
        data->words[word_idx][char_idx] = '\0';
        data->word_count = word_idx + 1;
    } else {
        data->word_count = word_idx;
    }
    
    return STATE_READING;
}

int process_state_reading(WordData* data) {
    if (data->word_count == 0) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

int process_state_processing(WordData* data) {
    if (data->word_count <= 0 || data->word_count > MAX_WORDS) {
        return STATE_ERROR;
    }
    
    for (int i = 0; i < data->word_count; i++) {
        for (int j = 0; data->words[i][j] != '\0'; j++) {
            if (j >= MAX_WORD_LEN - 1) {
                return STATE_ERROR;
            }
            data->words[i][j] = TO_UPPER(data->words[i][j]);
        }
        data->processed_count++;
    }
    
    return STATE_WRITING;
}

int process_state_writing(WordData* data) {
    if (data->processed_count != data->word_count) {
        return STATE_ERROR;
    }
    
    printf("Processed words (%d):\n", data->word_count);
    for (int i = 0; i < data->word_count; i++) {
        if (printf("%s ", data->words[i]) < 0) {
            return STATE_ERROR;
        }
    }
    printf("\n");
    
    return STATE_IDLE;
}

int main(void) {
    int current_state = STATE_IDLE;
    WordData word_data;
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                TRANSITION(process_state_idle(input_buffer, &word_data));
                break;
            case STATE_READING:
                TRANSITION(process_state_reading(&word_data));
                break;
            case STATE_PROCESSING:
                TRANSITION(process_state_processing(&word_data));
                break;
            case STATE_WRITING:
                TRANSITION(process_state_writing(&word_data));
                break;
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (current_state == STATE_IDLE) {
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error occurred during processing\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}