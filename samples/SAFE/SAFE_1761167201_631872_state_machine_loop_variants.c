//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
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
    
    for (i = 0; i <= (int)input_len; i++) {
        switch (current_state) {
            case START:
                if (i == (int)input_len) {
                    current_state = DONE;
                } else if (isdigit((unsigned char)input[i])) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = input[i];
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)input[i])) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = input[i];
                    }
                    current_state = READING_WORD;
                } else if (!isspace((unsigned char)input[i])) {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (i == (int)input_len || !isdigit((unsigned char)input[i])) {
                    if (buffer_index > 0 && buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                    }
                    
                    if (i == (int)input_len) {
                        current_state = DONE;
                    } else if (isalpha((unsigned char)input[i])) {
                        buffer_index = 0;
                        if (buffer_index < 255) {
                            buffer[buffer_index++] = input[i];
                        }
                        current_state = READING_WORD;
                    } else if (isspace((unsigned char)input[i])) {
                        current_state = START;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = input[i];
                    }
                }
                break;
                
            case READING_WORD:
                if (i == (int)input_len || !isalpha((unsigned char)input[i])) {
                    if (buffer_index > 0 && buffer_index < 256) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                    }
                    
                    if (i == (int)input_len) {
                        current_state = DONE;
                    } else if (isdigit((unsigned char)input[i])) {
                        buffer_index = 0;
                        if (buffer_index < 255) {
                            buffer[buffer_index++] = input[i];
                        }
                        current_state = READING_NUMBER;
                    } else if (isspace((unsigned char)input[i])) {
                        current_state = START;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = input[i];
                    }
                }
                break;
                
            case ERROR:
                if (i == (int)input_len) {
                    current_state = DONE;
                } else if (isspace((unsigned char)input[i])) {
                    current_state = START;
                }
                break;
                
            case DONE:
                break;
        }
        
        if (current_state == ERROR && i < (int)input_len) {
            fprintf(stderr, "Invalid character at position %d: '%c'\n", i, input[i]);
        }
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}