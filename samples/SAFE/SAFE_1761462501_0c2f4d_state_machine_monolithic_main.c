//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBERS,
    PROCESSING_TEXT,
    FINISHED,
    ERROR
};

int main(void) {
    enum State current_state = INIT;
    char buffer[256];
    int number_count = 0;
    int total_sum = 0;
    int valid_input = 1;
    
    printf("Enter data (type 'done' to finish):\n");
    
    while (current_state != FINISHED && current_state != ERROR) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            current_state = ERROR;
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len >= sizeof(buffer) - 1) {
            current_state = ERROR;
            break;
        }
        
        switch (current_state) {
            case INIT:
                if (strcmp(buffer, "numbers") == 0) {
                    current_state = READING_NUMBERS;
                    printf("Enter numbers (one per line):\n");
                } else if (strcmp(buffer, "text") == 0) {
                    current_state = PROCESSING_TEXT;
                    printf("Enter text lines:\n");
                } else if (strcmp(buffer, "done") == 0) {
                    current_state = FINISHED;
                } else {
                    printf("Unknown command. Use 'numbers', 'text', or 'done'.\n");
                }
                break;
                
            case READING_NUMBERS:
                if (strcmp(buffer, "done") == 0) {
                    current_state = FINISHED;
                } else {
                    char *endptr;
                    long num = strtol(buffer, &endptr, 10);
                    if (endptr != buffer && *endptr == '\0' && 
                        num >= -1000000 && num <= 1000000) {
                        total_sum += (int)num;
                        number_count++;
                        printf("Added number: %ld\n", num);
                    } else {
                        printf("Invalid number. Enter a valid integer or 'done'.\n");
                    }
                }
                break;
                
            case PROCESSING_TEXT:
                if (strcmp(buffer, "done") == 0) {
                    current_state = FINISHED;
                } else {
                    int char_count = 0;
                    int digit_count = 0;
                    for (size_t i = 0; i < strlen(buffer); i++) {
                        if (isalnum((unsigned char)buffer[i])) {
                            char_count++;
                        }
                        if (isdigit((unsigned char)buffer[i])) {
                            digit_count++;
                        }
                    }
                    printf("Line: %s (chars: %d, digits: %d)\n", 
                           buffer, char_count, digit_count);
                }
                break;
                
            case FINISHED:
            case ERROR:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input or system error.\n");
        return 1;
    }
    
    if (current_state == READING_NUMBERS) {
        printf("Processed %d numbers. Total sum: %d\n", number_count, total_sum);
    } else if (current_state == PROCESSING_TEXT) {
        printf("Text processing completed.\n");
    } else {
        printf("No data processed.\n");
    }
    
    return 0;
}