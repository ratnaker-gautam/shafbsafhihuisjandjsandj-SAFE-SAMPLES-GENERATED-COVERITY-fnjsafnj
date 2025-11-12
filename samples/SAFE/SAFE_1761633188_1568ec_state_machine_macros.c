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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int count;
} WordList;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

int process_words(const char* input, WordList* word_list) {
    if (input == NULL || word_list == NULL) return 0;
    
    word_list->count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    size_t word_len = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (c == ' ' || c == '\0') {
            if (word_len > 0 && word_len < MAX_WORD_LEN) {
                if (word_list->count >= MAX_WORDS) return 0;
                
                strncpy(word_list->words[word_list->count], &input[word_start], word_len);
                word_list->words[word_list->count][word_len] = '\0';
                word_list->count++;
            }
            word_start = i + 1;
            word_len = 0;
        } else {
            word_len++;
        }
    }
    
    return word_list->count > 0;
}

void print_words(const WordList* word_list) {
    if (word_list == NULL || word_list->count == 0) return;
    
    printf("Processed words (%d):\n", word_list->count);
    for (int i = 0; i < word_list->count; i++) {
        printf("%d: %s\n", i + 1, word_list->words[i]);
    }
}

int main() {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    WordList word_list;
    int result;
    
    printf("Word Processor State Machine\n");
    printf("Enter text (alphanumeric and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    size_t len = strlen(input_buffer);
                    if (len > 0 && input_buffer[len - 1] == '\n') {
                        input_buffer[len - 1] = '\0';
                    }
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (validate_input(input_buffer)) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    printf("Invalid input. Only alphanumeric characters and spaces allowed.\n");
                    TRANSITION(STATE_IDLE);
                    printf("Enter text: ");
                }
                break;
                
            case STATE_PROCESSING:
                result = process_words(input_buffer, &word_list);
                if (result) {
                    TRANSITION(STATE_WRITING);
                } else {
                    printf("Error processing words. Input may be too long or contain no valid words.\n");
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                print_words(&word_list);
                TRANSITION(STATE_IDLE);
                printf("\nEnter text: ");
                break;
                
            case STATE_ERROR:
                printf("Fatal error occurred. Exiting.\n");
                return 1;
        }
    }
    
    return 0;
}