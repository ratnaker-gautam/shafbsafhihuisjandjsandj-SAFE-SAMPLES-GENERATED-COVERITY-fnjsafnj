//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_index = 0;
    enum State current_state = START;
    int number_count = 0;
    int word_count = 0;
    int i = 0;
    int valid_input = 0;

    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }

    while (i < 256 && input[i] != '\0') {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (isspace(c)) {
                    i++;
                } else if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (isspace(c)) {
                    if (buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                    }
                    number_count++;
                    printf("Found number: %s\n", buffer);
                    current_state = START;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (isspace(c)) {
                    if (buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                    }
                    word_count++;
                    printf("Found word: %s\n", buffer);
                    current_state = START;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid character at position %d: '%c'\n", i, c);
                return 1;
                
            case DONE:
                break;
        }
    }

    if (current_state == READING_NUMBER) {
        if (buffer_index < 256) {
            buffer[buffer_index] = '\0';
        }
        number_count++;
        printf("Found number: %s\n", buffer);
    } else if (current_state == READING_WORD) {
        if (buffer_index < 256) {
            buffer[buffer_index] = '\0';
        }
        word_count++;
        printf("Found word: %s\n", buffer);
    } else if (current_state == ERROR) {
        fprintf(stderr, "Input ended in error state\n");
        return 1;
    }

    printf("\nSummary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}