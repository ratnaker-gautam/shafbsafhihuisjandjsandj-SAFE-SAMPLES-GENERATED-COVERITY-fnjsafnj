//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    char input[1024];
    char buffer[256];
    int buffer_index = 0;
    enum state current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int i = 0;
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    while (i <= input_len && current_state != FINISHED && current_state != ERROR) {
        char c = (i < input_len) ? input[i] : '\0';
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = tolower(c);
                    }
                    current_state = READING_WORD;
                } else if (c == ' ' || c == '\t') {
                    current_state = SKIPPING_SPACE;
                } else if (c == '\0') {
                    current_state = FINISHED;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    buffer[buffer_index] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = tolower(c);
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    buffer[buffer_index] = '\0';
                    if (strlen(buffer) > 0) {
                        word_count++;
                    }
                    current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == ' ' || c == '\t') {
                    // Continue skipping
                } else if (isdigit(c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = tolower(c);
                    }
                    current_state = READING_WORD;
                } else if (c == '\0') {
                    current_state = FINISHED;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid input format\n");
                break;
        }
        
        i++;
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Processing error at position %d\n", i - 1);
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}