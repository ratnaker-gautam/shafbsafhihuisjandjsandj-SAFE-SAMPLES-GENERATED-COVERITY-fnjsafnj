//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3

int process_char(char c, int *count, int *word_len) {
    if (isalpha(c)) {
        if (*word_len == 0) {
            (*count)++;
        }
        (*word_len)++;
        return STATE_READING;
    } else if (isspace(c)) {
        *word_len = 0;
        return STATE_IDLE;
    } else if (c == '.') {
        return STATE_DONE;
    }
    return STATE_PROCESSING;
}

int main(void) {
    char input[MAX_INPUT];
    char *ptr = input;
    int state = STATE_IDLE;
    int word_count = 0;
    int current_word_len = 0;
    
    printf("Enter text (end with '.'): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    while (*ptr != '\0' && state != STATE_DONE) {
        if (ptr >= input + sizeof(input) - 1) {
            break;
        }
        
        switch (state) {
            case STATE_IDLE:
            case STATE_PROCESSING:
                state = process_char(*ptr, &word_count, &current_word_len);
                break;
            case STATE_READING:
                state = process_char(*ptr, &word_count, &current_word_len);
                break;
            case STATE_DONE:
                break;
        }
        
        ptr++;
    }
    
    printf("Word count: %d\n", word_count);
    return 0;
}