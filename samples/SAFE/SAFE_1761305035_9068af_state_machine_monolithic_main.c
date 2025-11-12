//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_pos = 0;
    enum State state = START;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars): ");
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
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        switch (state) {
            case START:
                if (c == '\0') {
                    state = DONE;
                } else if (isdigit((unsigned char)c)) {
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = tolower((unsigned char)c);
                    }
                    state = READING_WORD;
                } else if (c != ' ' && c != '\t') {
                    state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0' || c == ' ' || c == '\t') {
                    if (buffer_pos > 0 && buffer_pos < sizeof(buffer)) {
                        buffer[buffer_pos] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                    }
                    buffer_pos = 0;
                    state = (c == '\0') ? DONE : START;
                } else if (isdigit((unsigned char)c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    } else {
                        state = ERROR;
                    }
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0' || c == ' ' || c == '\t') {
                    if (buffer_pos > 0 && buffer_pos < sizeof(buffer)) {
                        buffer[buffer_pos] = '\0';
                        word_count++;
                    }
                    buffer_pos = 0;
                    state = (c == '\0') ? DONE : START;
                } else if (isalpha((unsigned char)c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = tolower((unsigned char)c);
                    } else {
                        state = ERROR;
                    }
                } else {
                    state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
        
        if (state == ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}