//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_ALPHA, ERROR, DONE };

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_pos = 0;
    enum State current_state = START;
    int num_count = 0;
    int alpha_count = 0;
    
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
    
    int i = 0;
    while (current_state != ERROR && current_state != DONE) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = DONE;
                } else if (isdigit((unsigned char)c)) {
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    current_state = READING_NUM;
                } else if (isalpha((unsigned char)c)) {
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    current_state = READING_ALPHA;
                } else if (c != ' ' && c != '\t') {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit((unsigned char)c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    if (buffer_pos > 0 && buffer_pos < sizeof(buffer)) {
                        buffer[buffer_pos] = '\0';
                        num_count++;
                        printf("Found number: %s\n", buffer);
                    }
                    buffer_pos = 0;
                    if (c == '\0') {
                        current_state = DONE;
                    } else {
                        current_state = START;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_ALPHA:
                if (isalpha((unsigned char)c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    if (buffer_pos > 0 && buffer_pos < sizeof(buffer)) {
                        buffer[buffer_pos] = '\0';
                        alpha_count++;
                        printf("Found word: %s\n", buffer);
                    }
                    buffer_pos = 0;
                    if (c == '\0') {
                        current_state = DONE;
                    } else {
                        current_state = START;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
            case DONE:
                break;
        }
        
        i++;
        if (i >= sizeof(input)) {
            current_state = ERROR;
        }
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Summary: %d numbers, %d words found\n", num_count, alpha_count);
    return 0;
}