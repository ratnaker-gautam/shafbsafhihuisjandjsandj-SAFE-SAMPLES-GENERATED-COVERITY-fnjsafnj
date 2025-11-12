//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    char input[1024];
    char buffer[256];
    int buffer_pos = 0;
    enum State state = START;
    int number_sum = 0;
    int word_count = 0;
    int i = 0;
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (state != DONE && state != ERROR && input[i] != '\0') {
        switch (state) {
            case START:
                if (isspace((unsigned char)input[i])) {
                    i++;
                } else if (isdigit((unsigned char)input[i])) {
                    state = READING_NUMBER;
                    buffer_pos = 0;
                } else if (isalpha((unsigned char)input[i])) {
                    state = READING_WORD;
                    buffer_pos = 0;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)input[i])) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = input[i++];
                    } else {
                        state = ERROR;
                    }
                } else if (isspace((unsigned char)input[i]) || input[i] == '\0') {
                    buffer[buffer_pos] = '\0';
                    long num = strtol(buffer, NULL, 10);
                    if (num > 0 && num <= 1000000) {
                        number_sum += (int)num;
                    }
                    state = START;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)input[i])) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = input[i++];
                    } else {
                        state = ERROR;
                    }
                } else if (isspace((unsigned char)input[i]) || input[i] == '\0') {
                    buffer[buffer_pos] = '\0';
                    if (buffer_pos > 0) {
                        word_count++;
                    }
                    state = START;
                } else {
                    state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;
                
            default:
                break;
        }
    }
    
    if (state == READING_NUMBER) {
        buffer[buffer_pos] = '\0';
        long num = strtol(buffer, NULL, 10);
        if (num > 0 && num <= 1000000) {
            number_sum += (int)num;
        }
    } else if (state == READING_WORD) {
        buffer[buffer_pos] = '\0';
        if (buffer_pos > 0) {
            word_count++;
        }
    } else if (state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}