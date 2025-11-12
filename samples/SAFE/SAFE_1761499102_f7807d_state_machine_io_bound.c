//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
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
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (current_state != ERROR) {
        switch (current_state) {
            case IDLE:
                buffer_pos = 0;
                word_count = 0;
                memset(buffer, 0, sizeof(buffer));
                current_state = READING;
                break;
                
            case READING:
                ch = getchar();
                if (ch == EOF) {
                    current_state = WRITING;
                } else if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)ch;
                    if (isspace(ch)) {
                        current_state = PROCESSING;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0 && !isspace(buffer[buffer_pos - 1])) {
                    word_count++;
                }
                current_state = READING;
                break;
                
            case WRITING:
                if (buffer_pos > 0) {
                    if (!isspace(buffer[buffer_pos - 1])) {
                        word_count++;
                    }
                    buffer[buffer_pos] = '\0';
                    printf("Text: %s\n", buffer);
                    printf("Word count: %d\n", word_count);
                }
                current_state = IDLE;
                break;
                
            case ERROR:
                fprintf(stderr, "Error: Buffer overflow\n");
                return 1;
        }
    }
    
    return 0;
}