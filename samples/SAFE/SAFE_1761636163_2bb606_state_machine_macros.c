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
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int processed_count;
} WordData;

int validate_input(const char* input, int max_len) {
    if (!input) return 0;
    size_t len = strlen(input);
    return CHECK_BOUNDS(len, max_len);
}

void state_idle(WordData* data) {
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
}

int state_reading(WordData* data, const char* input) {
    if (!validate_input(input, MAX_INPUT_LEN)) {
        return STATE_ERROR;
    }
    
    data->word_count = 0;
    char temp[MAX_INPUT_LEN];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char* token = strtok(temp, " \t\n");
    while (token && data->word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len < MAX_WORD_LEN) {
            strncpy(data->words[data->word_count], token, MAX_WORD_LEN - 1);
            data->words[data->word_count][MAX_WORD_LEN - 1] = '\0';
            data->word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    return STATE_PROCESSING;
}

int state_processing(WordData* data) {
    if (data->word_count <= 0) {
        return STATE_ERROR;
    }
    
    data->processed_count = 0;
    for (int i = 0; i < data->word_count; i++) {
        if (strlen(data->words[i]) > 0) {
            for (int j = 0; data->words[i][j]; j++) {
                if (data->words[i][j] >= 'a' && data->words[i][j] <= 'z') {
                    data->words[i][j] = data->words[i][j] - 'a' + 'A';
                }
            }
            data->processed_count++;
        }
    }
    
    return STATE_WRITING;
}

int state_writing(WordData* data) {
    printf("Processed words (%d):\n", data->processed_count);
    for (int i = 0; i < data->word_count && i < data->processed_count; i++) {
        if (strlen(data->words[i]) > 0) {
            printf("%s ", data->words[i]);
        }
    }
    printf("\n");
    return STATE_IDLE;
}

int state_error(void) {
    printf("Error: Invalid input or processing error\n");
    return STATE_IDLE;
}

int main(void) {
    int current_state = STATE_IDLE;
    WordData data = {0};
    char input[MAX_INPUT_LEN] = {0};
    
    printf("Text Processor State Machine\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        switch (current_state) {
            case STATE_IDLE:
                state_idle(&data);
                if (fgets(input, sizeof(input), stdin)) {
                    input[strcspn(input, "\n")] = '\0';
                    if (strcmp(input, "quit") == 0) {
                        return 0;
                    }
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                TRANSITION(state_reading(&data, input));
                break;
                
            case STATE_PROCESSING:
                TRANSITION(state_processing(&data));
                break;
                
            case STATE_WRITING:
                TRANSITION(state_writing(&data));
                break;
                
            case STATE_ERROR:
                TRANSITION(state_error());
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        memset(input, 0, sizeof(input));
    }
    
    return 0;
}