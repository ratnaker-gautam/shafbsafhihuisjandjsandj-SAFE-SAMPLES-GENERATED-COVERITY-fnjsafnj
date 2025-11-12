//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR };

int main(void) {
    enum State current_state = IDLE;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int ch;
    
    while ((ch = getchar()) != EOF) {
        switch (current_state) {
            case IDLE:
                if (isalpha(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = READING;
                }
                break;
                
            case READING:
                if (isalpha(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                } else {
                    buffer[buffer_pos] = '\0';
                    current_state = PROCESSING;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0) {
                    word_count++;
                    for (int i = 0; i < buffer_pos; i++) {
                        buffer[i] = (char)toupper(buffer[i]);
                    }
                    current_state = WRITING;
                } else {
                    current_state = IDLE;
                }
                break;
                
            case WRITING:
                if (buffer_pos > 0) {
                    printf("%s\n", buffer);
                    buffer_pos = 0;
                    current_state = IDLE;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Error state encountered\n");
                return 1;
        }
        
        if (ch == '\n') {
            if (current_state == READING) {
                buffer[buffer_pos] = '\0';
                current_state = PROCESSING;
            }
        }
    }
    
    if (current_state == READING) {
        buffer[buffer_pos] = '\0';
        current_state = PROCESSING;
    }
    
    while (current_state == PROCESSING || current_state == WRITING) {
        switch (current_state) {
            case PROCESSING:
                if (buffer_pos > 0) {
                    word_count++;
                    for (int i = 0; i < buffer_pos; i++) {
                        buffer[i] = (char)toupper(buffer[i]);
                    }
                    current_state = WRITING;
                } else {
                    current_state = IDLE;
                }
                break;
                
            case WRITING:
                if (buffer_pos > 0) {
                    printf("%s\n", buffer);
                    buffer_pos = 0;
                    current_state = IDLE;
                }
                break;
                
            default:
                break;
        }
    }
    
    printf("Total words processed: %d\n", word_count);
    return 0;
}