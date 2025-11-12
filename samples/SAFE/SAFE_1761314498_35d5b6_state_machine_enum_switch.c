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
    int buffer_pos = 0;
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
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = tolower(c);
                    }
                    current_state = READING_WORD;
                } else if (isspace(c)) {
                    current_state = SKIPPING_SPACE;
                } else if (c == '\0') {
                    current_state = FINISHED;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = tolower(c);
                    }
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    if (strlen(buffer) > 0) {
                        word_count++;
                    }
                    current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (isspace(c)) {
                    break;
                } else if (isdigit(c)) {
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = tolower(c);
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
                fprintf(stderr, "Invalid character at position %d\n", i);
                return 1;
        }
        
        i++;
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}