//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_index = 0;
    enum State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
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
    while (i <= input_len) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = DONE;
                } else if (isdigit((unsigned char)c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = tolower((unsigned char)c);
                    }
                    current_state = READING_WORD;
                } else if (!isspace((unsigned char)c)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (buffer_index > 0 && buffer_index < sizeof(buffer)) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        if (num > 0) {
                            if (number_sum <= INT_MAX - num) {
                                number_sum += num;
                            }
                        }
                    }
                    current_state = (c == '\0') ? DONE : START;
                } else if (isdigit((unsigned char)c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (buffer_index > 0 && buffer_index < sizeof(buffer)) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                    }
                    current_state = (c == '\0') ? DONE : START;
                } else if (isalpha((unsigned char)c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = tolower((unsigned char)c);
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format\n");
                return 1;
                
            case DONE:
                goto process_results;
        }
        
        i++;
    }

process_results:
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}