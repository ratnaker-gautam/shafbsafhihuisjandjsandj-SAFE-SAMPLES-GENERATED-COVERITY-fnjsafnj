//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    char input[1024];
    char buffer[1024];
    int buffer_pos = 0;
    enum State state = START;
    int number_count = 0;
    int word_count = 0;
    int i = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    while (i <= input_len) {
        char c = input[i];
        
        switch (state) {
            case START:
                if (isdigit(c)) {
                    buffer[buffer_pos++] = c;
                    state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer[buffer_pos++] = c;
                    state = READING_WORD;
                } else if (c == ' ' || c == '\t' || c == '\0') {
                } else if (c != '\0') {
                    state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    } else {
                        state = ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    number_count++;
                    printf("Found number: %s\n", buffer);
                    buffer_pos = 0;
                    state = START;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    } else {
                        state = ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    printf("Found word: %s\n", buffer);
                    buffer_pos = 0;
                    state = START;
                } else {
                    state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
        
        if (state == ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
        
        i++;
        
        if (c == '\0') {
            break;
        }
    }
    
    if (state == READING_NUMBER || state == READING_WORD) {
        buffer[buffer_pos] = '\0';
        if (state == READING_NUMBER) {
            number_count++;
            printf("Found number: %s\n", buffer);
        } else {
            word_count++;
            printf("Found word: %s\n", buffer);
        }
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}