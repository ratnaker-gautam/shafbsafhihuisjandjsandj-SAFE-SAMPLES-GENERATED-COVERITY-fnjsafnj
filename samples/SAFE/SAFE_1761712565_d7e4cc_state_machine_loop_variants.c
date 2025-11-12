//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    FINISHED,
    ERROR
};

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_pos = 0;
    enum state current_state = START;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars): ");
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
    
    int i = 0;
    while (i < input_len && current_state != FINISHED && current_state != ERROR) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (isspace((unsigned char)c)) {
                    i++;
                } else if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    i++;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    i++;
                } else if (isspace((unsigned char)c) || i == input_len) {
                    if (buffer_pos < sizeof(buffer)) {
                        buffer[buffer_pos] = '\0';
                    }
                    number_count++;
                    printf("Found number: %s\n", buffer);
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    i++;
                } else if (isspace((unsigned char)c) || i == input_len) {
                    if (buffer_pos < sizeof(buffer)) {
                        buffer[buffer_pos] = '\0';
                    }
                    word_count++;
                    printf("Found word: %s\n", buffer);
                    current_state = START;
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
    }
    
    if (current_state == READING_NUMBER || current_state == READING_WORD) {
        if (buffer_pos > 0 && buffer_pos < sizeof(buffer)) {
            buffer[buffer_pos] = '\0';
            if (current_state == READING_NUMBER) {
                number_count++;
                printf("Found number: %s\n", buffer);
            } else {
                word_count++;
                printf("Found word: %s\n", buffer);
            }
        }
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Processing failed due to invalid input\n");
        return 1;
    }
    
    printf("\nSummary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}