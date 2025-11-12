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

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }

    while (i <= len) {
        char c = input[i];
        
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
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    if (buffer_index > 0 && buffer_index < sizeof(buffer)) {
                        buffer[buffer_index] = '\0';
                        printf("Found number: %s\n", buffer);
                        number_count++;
                        valid_input = 1;
                    }
                    buffer_index = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = tolower(c);
                    }
                    current_state = READING_WORD;
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    if (buffer_index > 0 && buffer_index < sizeof(buffer)) {
                        buffer[buffer_index] = '\0';
                        printf("Found word: %s\n", buffer);
                        word_count++;
                        valid_input = 1;
                    }
                    buffer_index = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid character encountered at position %d\n", i);
                buffer_index = 0;
                if (c == ' ' || c == '\t' || c == '\0') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case DONE:
                break;
        }
        
        if (c == '\0') {
            current_state = DONE;
            break;
        }
        
        i++;
        if (i >= sizeof(input)) {
            break;
        }
    }

    if (current_state == READING_NUMBER || current_state == READING_WORD) {
        if (buffer_index > 0 && buffer_index < sizeof(buffer)) {
            buffer[buffer_index] = '\0';
            if (current_state == READING_NUMBER) {
                printf("Found number: %s\n", buffer);
                number_count++;
            } else {
                printf("Found word: %s\n", buffer);
                word_count++;
            }
            valid_input = 1;
        }
    }

    if (valid_input) {
        printf("Summary: %d numbers, %d words\n", number_count, word_count);
    } else {
        printf("No valid numbers or words found\n");
    }

    return 0;
}