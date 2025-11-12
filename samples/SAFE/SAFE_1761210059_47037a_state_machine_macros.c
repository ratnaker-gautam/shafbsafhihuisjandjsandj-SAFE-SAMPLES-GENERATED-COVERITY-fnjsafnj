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
    return STATE_READING;
}

int process_state_reading(const char* input, WordData* data) {
    int input_len = (int)strlen(input);
    int word_start = -1;
    
    for (int i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (IS_VALID_CHAR(c) && word_start == -1) {
            word_start = i;
        } else if ((!IS_VALID_CHAR(c) || i == input_len) && word_start != -1) {
            int word_len = i - word_start;
            if (word_len > 0 && word_len < MAX_WORD_LEN && data->word_count < MAX_WORDS) {
                strncpy(data->words[data->word_count], &input[word_start], word_len);
                data->words[data->word_count][word_len] = '\0';
                data->word_count++;
            }
            word_start = -1;
        }
    }
    
    return data->word_count > 0 ? STATE_PROCESSING : STATE_ERROR;
}

int process_state_processing(WordData* data) {
    if (data->word_count <= 0) {
        return STATE_ERROR;
    }
    
    for (int i = 0; i < data->word_count; i++) {
        for (int j = 0; data->words[i][j] != '\0'; j++) {
            data->words[i][j] = TO_UPPER(data->words[i][j]);
        }
        data->processed_count++;
    }
    
    return STATE_WRITING;
}

int process_state_writing(const WordData* data) {
    if (data->processed_count <= 0) {
        return STATE_ERROR;
    }
    
    printf("Processed %d words:\n", data->processed_count);
    for (int i = 0; i < data->word_count; i++) {
        printf("%d: %s\n", i + 1, data->words[i]);
    }
    
    return STATE_IDLE;
}

int process_state_error(void) {
    printf("Error: Invalid input or processing error occurred\n");
    return STATE_IDLE;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordData word_data;
    int current_state = STATE_IDLE;
    int running = 1;
    
    printf("Word Processor State Machine\n");
    printf("Enter text (max %d characters) or 'quit' to exit:\n", MAX_INPUT_LEN);
    
    while (running) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                    }
                    
                    if (strcmp(input, "quit") == 0) {
                        running = 0;
                        break;
                    }
                    
                    TRANSITION(process_state_idle(input, &word_data));
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                TRANSITION(process_state_reading(input, &word_data));
                break;
                
            case STATE_PROCESSING:
                TRANSITION(process_state_processing(&word_data));
                break;
                
            case STATE_WRITING:
                TRANSITION(process_state_writing(&word_data));
                break;
                
            case STATE_ERROR:
                TRANSITION(process_state_error());
                break;
        }
        
        if (current_state == STATE_IDLE && running) {
            printf("\nEnter text or 'quit' to exit:\n");
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}