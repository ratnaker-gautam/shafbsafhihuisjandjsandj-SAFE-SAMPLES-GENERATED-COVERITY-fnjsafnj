//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

int main(void) {
    enum state current_state = IDLE;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int ch;
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case IDLE:
                printf("Enter text (empty line to finish): ");
                current_state = READING;
                break;
                
            case READING:
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (buffer[0] == '\n') {
                    current_state = DONE;
                } else {
                    buffer_pos = 0;
                    current_state = PROCESSING;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos >= (int)sizeof(buffer) - 1) {
                    current_state = WRITING;
                    break;
                }
                
                ch = buffer[buffer_pos];
                if (ch == '\0' || ch == '\n') {
                    current_state = WRITING;
                    break;
                }
                
                if (isalpha((unsigned char)ch)) {
                    if (buffer_pos == 0 || !isalpha((unsigned char)buffer[buffer_pos - 1])) {
                        word_count++;
                    }
                }
                buffer_pos++;
                break;
                
            case WRITING:
                printf("Processed line: ");
                for (int i = 0; i < buffer_pos && buffer[i] != '\n' && buffer[i] != '\0'; i++) {
                    putchar(buffer[i]);
                }
                printf("\nWord count so far: %d\n", word_count);
                current_state = READING;
                break;
                
            case ERROR:
                fprintf(stderr, "Error reading input\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    printf("Total words: %d\n", word_count);
    return 0;
}