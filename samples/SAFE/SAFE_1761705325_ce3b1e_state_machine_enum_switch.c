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
    int number_count = 0;
    int i = 0;
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (current_state != FINISHED && current_state != ERROR && i < 1024) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isdigit(c)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = tolower(c);
                    }
                    current_state = READING_WORD;
                } else if (isspace(c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0') {
                    if (buffer_index > 0 && buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        if (num > 0) {
                            number_sum += num;
                            number_count++;
                        }
                    }
                    current_state = FINISHED;
                } else if (isdigit(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                } else if (isspace(c)) {
                    if (buffer_index > 0 && buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        if (num > 0) {
                            number_sum += num;
                            number_count++;
                        }
                    }
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0') {
                    if (buffer_index > 0 && buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                    }
                    current_state = FINISHED;
                } else if (isalpha(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = tolower(c);
                    }
                } else if (isspace(c)) {
                    if (buffer_index > 0 && buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                    }
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isdigit(c)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = tolower(c);
                    }
                    current_state = READING_WORD;
                } else if (!isspace(c)) {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
            case ERROR:
                break;
        }
        
        i++;
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Words found: %d\n", word_count);
    printf("Numbers found: %d\n", number_count);
    if (number_count > 0) {
        printf("Sum of numbers: %d\n", number_sum);
        printf("Average of numbers: %.2f\n", (double)number_sum / number_count);
    } else {
        printf("No numbers to calculate average\n");
    }
    
    return 0;
}