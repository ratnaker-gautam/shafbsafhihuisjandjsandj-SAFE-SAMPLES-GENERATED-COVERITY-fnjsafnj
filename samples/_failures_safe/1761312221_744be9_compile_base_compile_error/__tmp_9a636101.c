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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '\n' || c == '\0')

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

void state_idle(WordData* data) {
    if (data == NULL) return;
    data->word_count = 0;
    data->processed_count = 0;
    printf("State: IDLE - Ready to process input\n");
}

int state_reading(const char* input, WordData* data) {
    if (input == NULL || data == NULL) return 0;
    
    const char* ptr = input;
    int word_len = 0;
    data->word_count = 0;
    
    while (*ptr != '\0' && data->word_count < MAX_WORDS) {
        if (*ptr == ' ' || *ptr == '\n') {
            if (word_len > 0) {
                if (word_len < MAX_WORD_LEN) {
                    data->words[data->word_count][word_len] = '\0';
                    data->word_count++;
                }
                word_len = 0;
            }
        } else {
            if (word_len < MAX_WORD_LEN - 1) {
                data->words[data->word_count][word_len] = *ptr;
                word_len++;
            }
        }
        ptr++;
    }
    
    if (word_len > 0 && data->word_count < MAX_WORDS && word_len < MAX_WORD_LEN) {
        data->words[data->word_count][word_len] = '\0';
        data->word_count++;
    }
    
    return data->word_count > 0;
}

void state_processing(WordData* data) {
    if (data == NULL) return;
    
    data->processed_count = 0;
    for (int i = 0; i < data->word_count; i++) {
        int len = strlen(data->words[i]);
        if (len > 0) {
            for (int j = 0; j < len; j++) {
                if (data->words[i][j] >= 'a' && data->words[i][j] <= 'z') {
                    data->words[i][j] = data->words[i][j] - 'a' + 'A';
                }
            }
            data->processed_count++;
        }
    }
}

void state_writing(const WordData* data) {
    if (data == NULL) return;
    
    printf("Processed words (%d):\n", data->processed_count);
    for (int i = 0; i < data->word_count; i++) {
        if (strlen(data->words[i]) > 0) {
            printf("%s\n", data->words[i]);
        }
    }
}

void state_error(void) {
    printf("State: ERROR - Invalid input detected\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordData data;
    int current_state = STATE_IDLE;
    int running = 1;
    
    while (running) {
        switch (current_state) {
            case STATE_IDLE:
                state_idle(&data);
                printf("Enter text (or 'quit' to exit): ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                    }
                    if (strcmp(input, "quit") == 0) {
                        running = 0;
                        break;
                    }
                    if (validate_input(input)) {
                        TRANSITION(STATE_READING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (state_reading(input, &data)) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                state_processing(&data);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                state_writing(&data);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                state_error();
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    printf