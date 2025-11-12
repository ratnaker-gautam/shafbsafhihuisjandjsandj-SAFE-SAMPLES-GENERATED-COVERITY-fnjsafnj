//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char input[1024];
    char buffer[1024];
    int buf_pos = 0;
    enum state current_state = START;
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
    
    if (input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    while (i <= input_len && current_state != ERROR && current_state != DONE) {
        char c = (i < input_len) ? input[i] : ' ';
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    buffer[buf_pos++] = c;
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer[buf_pos++] = c;
                    current_state = READING_WORD;
                } else if (c == ' ' || c == '\t') {
                } else if (c == '\0') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buf_pos < sizeof(buffer) - 1) {
                        buffer[buf_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    buffer[buf_pos] = '\0';
                    number_count++;
                    printf("Found number: %s\n", buffer);
                    buf_pos = 0;
                    current_state = (c == '\0') ? DONE : START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buf_pos < sizeof(buffer) - 1) {
                        buffer[buf_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    buffer[buf_pos] = '\0';
                    word_count++;
                    printf("Found word: %s\n", buffer);
                    buf_pos = 0;
                    current_state = (c == '\0') ? DONE : START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid input format\n");
                break;
                
            case DONE:
                break;
        }
        
        i++;
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Processing failed due to invalid input\n");
        return 1;
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}