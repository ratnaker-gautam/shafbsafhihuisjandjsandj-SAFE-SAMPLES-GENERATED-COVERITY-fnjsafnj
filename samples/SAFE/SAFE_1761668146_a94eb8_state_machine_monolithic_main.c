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
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    while (i <= input_len && current_state != ERROR && current_state != DONE) {
        char c = (i < input_len) ? input[i] : ' ';
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)c)) {
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
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    current_state = START;
                } else if (c != '\0') {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    if (buffer_index > 0 && buffer_index < sizeof(buffer)) {
                        buffer[buffer_index] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                    }
                    buffer_index = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = tolower((unsigned char)c);
                    }
                    current_state = READING_WORD;
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    if (buffer_index > 0 && buffer_index < sizeof(buffer)) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                    }
                    buffer_index = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid character at position %d\n", i);
                break;
                
            case DONE:
                break;
        }
        
        i++;
        
        if (i > input_len && current_state != ERROR) {
            current_state = DONE;
        }
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Input parsing failed\n");
        return 1;
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}