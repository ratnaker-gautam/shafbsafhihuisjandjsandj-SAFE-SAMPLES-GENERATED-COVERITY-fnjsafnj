//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

int main(void) {
    enum State current_state = IDLE;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int char_count = 0;
    int c;
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case IDLE:
                buffer_pos = 0;
                word_count = 0;
                char_count = 0;
                memset(buffer, 0, sizeof(buffer));
                current_state = READING;
                break;
                
            case READING:
                c = getchar();
                if (c == EOF) {
                    current_state = PROCESSING;
                } else if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)c;
                    char_count++;
                    if (isspace(c) && buffer_pos > 1 && !isspace(buffer[buffer_pos-2])) {
                        word_count++;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0 && !isspace(buffer[buffer_pos-1])) {
                    word_count++;
                }
                current_state = WRITING;
                break;
                
            case WRITING:
                printf("\nText analysis results:\n");
                printf("Characters: %d\n", char_count);
                printf("Words: %d\n", word_count);
                printf("Buffer usage: %d/%zu\n", buffer_pos, sizeof(buffer));
                current_state = DONE;
                break;
                
            case ERROR:
                printf("Error: Buffer overflow detected\n");
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    return current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}