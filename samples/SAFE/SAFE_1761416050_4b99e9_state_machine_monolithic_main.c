//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    char input[1024];
    char buffer[256];
    int buffer_index = 0;
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
                if (isdigit(input[i])) {
                    state = READING_NUMBER;
                    buffer_index = 0;
                    buffer[buffer_index++] = input[i];
                } else if (isalpha(input[i])) {
                    state = READING_WORD;
                    buffer_index = 0;
                    buffer[buffer_index++] = input[i];
                } else if (!isspace(input[i])) {
                    state = ERROR;
                }
                i++;
                break;
                
            case READING_NUMBER:
                if (isdigit(input[i])) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = input[i];
                    } else {
                        state = ERROR;
                    }
                    i++;
                } else if (isspace(input[i]) || input[i] == '\0') {
                    buffer[buffer_index] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    state = START;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(input[i])) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = input[i];
                    } else {
                        state = ERROR;
                    }
                    i++;
                } else if (isspace(input[i]) || input[i] == '\0') {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    state = START;
                } else {
                    state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;
                
            default:
                state = ERROR;
                break;
        }
    }
    
    if (state == READING_NUMBER) {
        buffer[buffer_index] = '\0';
        int num = atoi(buffer);
        number_sum += num;
    } else if (state == READING_WORD) {
        word_count++;
    } else if (state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}