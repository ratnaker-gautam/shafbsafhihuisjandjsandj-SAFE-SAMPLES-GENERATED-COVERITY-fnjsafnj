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
    int char_count;
} text_data_t;

static int validate_input(const char* input) {
    if (!input) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

static void process_word(char* word, text_data_t* data) {
    if (!word || !data || data->word_count >= MAX_WORDS) return;
    
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) return;
    
    for (size_t i = 0; i < len; i++) {
        word[i] = TO_UPPER(word[i]);
    }
    
    strncpy(data->words[data->word_count], word, MAX_WORD_LEN - 1);
    data->words[data->word_count][MAX_WORD_LEN - 1] = '\0';
    data->char_count += (int)len;
    data->word_count++;
}

static void state_idle(text_data_t* data, int* current_state) {
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    TRANSITION(STATE_READING);
    data->word_count = 0;
    data->char_count = 0;
}

static void state_reading(text_data_t* data, int* current_state) {
    char input[MAX_INPUT_LEN + 2];
    if (!fgets(input, sizeof(input), stdin)) {
        TRANSITION(STATE_ERROR);
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input)) {
        TRANSITION(STATE_ERROR);
        return;
    }
    
    char temp[MAX_INPUT_LEN + 1];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    TRANSITION(STATE_PROCESSING);
}

static void state_processing(text_data_t* data, int* current_state) {
    char input[MAX_INPUT_LEN + 1];
    printf("Processing text...\n");
    
    char temp[MAX_INPUT_LEN + 1];
    if (!fgets(temp, sizeof(temp), stdin)) {
        TRANSITION(STATE_ERROR);
        return;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
    }
    
    strncpy(input, temp, sizeof(input) - 1);
    input[sizeof(input) - 1] = '\0';
    
    char word[MAX_WORD_LEN];
    int word_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= strlen(input); i++) {
        char c = input[i];
        
        if (IS_VALID_CHAR(c)) {
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = c;
            }
            in_word = 1;
        } else {
            if (in_word) {
                word[word_len] = '\0';
                process_word(word, data);
                word_len = 0;
                in_word = 0;
            }
        }
    }
    
    TRANSITION(STATE_WRITING);
}

static void state_writing(text_data_t* data, int* current_state) {
    printf("\nProcessed %d words (%d total characters):\n", data->word_count, data->char_count);
    
    for (int i = 0; i < data->word_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", data->words[i]);
    }
    printf("\n");
    
    TRANSITION(STATE_IDLE);
}

static void state_error(text_data_t* data, int* current_state) {
    printf("Error: Invalid input or processing error\n");
    TRANSITION(STATE_IDLE);
}

int main(void) {
    int current_state = STATE_IDLE;
    text_data_t data = {0};
    
    printf("Text Processor State Machine\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        switch (current_state) {
            case STATE_IDLE:
                state_idle(&data, &current_state);
                break;
            case STATE_READING:
                state_reading(&data, &current_state);
                break;
            case STATE_PROCESSING: