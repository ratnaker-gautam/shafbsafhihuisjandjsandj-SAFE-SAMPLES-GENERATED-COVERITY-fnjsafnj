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
    printf("State: IDLE - Ready to process input\n");
    data->word_count = 0;
    data->processed_count = 0;
    memset(data->words, 0, sizeof(data->words));
}

void state_reading(const char* input, WordData* data) {
    printf("State: READING - Parsing input\n");
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    char* token = strtok(buffer, " \n");
    while (token != NULL && data->word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len < MAX_WORD_LEN) {
            strncpy(data->words[data->word_count], token, MAX_WORD_LEN - 1);
            data->words[data->word_count][MAX_WORD_LEN - 1] = '\0';
            data->word_count++;
        }
        token = strtok(NULL, " \n");
    }
}

void state_processing(WordData* data) {
    printf("State: PROCESSING - Converting to uppercase\n");
    for (int i = 0; i < data->word_count && i < MAX_WORDS; i++) {
        for (int j = 0; data->words[i][j] != '\0' && j < MAX_WORD_LEN - 1; j++) {
            if (data->words[i][j] >= 'a' && data->words[i][j] <= 'z') {
                data->words[i][j] = data->words[i][j] - 'a' + 'A';
            }
        }
        data->processed_count++;
    }
}

void state_writing(const WordData* data) {
    printf("State: WRITING - Outputting results\n");
    printf("Processed %d words:\n", data->processed_count);
    for (int i = 0; i < data->processed_count && i < MAX_WORDS; i++) {
        printf("%s ", data->words[i]);
    }
    printf("\n");
}

void state_error(void) {
    printf("State: ERROR - Invalid input detected\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordData data;
    int current_state = STATE_IDLE;
    
    printf("Enter text to process (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        state_error();
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        state_error();
        return EXIT_FAILURE;
    }
    
    TRANSITION(STATE_IDLE);
    state_idle(&data);
    
    TRANSITION(STATE_READING);
    state_reading(input, &data);
    
    if (data.word_count == 0) {
        TRANSITION(STATE_ERROR);
        state_error();
        return EXIT_FAILURE;
    }
    
    TRANSITION(STATE_PROCESSING);
    state_processing(&data);
    
    TRANSITION(STATE_WRITING);
    state_writing(&data);
    
    return EXIT_SUCCESS;
}