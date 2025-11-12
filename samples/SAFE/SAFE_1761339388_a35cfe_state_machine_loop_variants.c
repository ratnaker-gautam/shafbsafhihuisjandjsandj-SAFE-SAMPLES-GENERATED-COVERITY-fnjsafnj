//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
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
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case IDLE:
                printf("Enter text (empty line to finish):\n");
                current_state = READING;
                break;
                
            case READING:
                buffer_pos = 0;
                memset(buffer, 0, sizeof(buffer));
                
                while ((c = getchar()) != EOF && c != '\n') {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = (char)c;
                    }
                }
                
                if (c == EOF) {
                    current_state = ERROR;
                } else if (buffer_pos == 0) {
                    current_state = DONE;
                } else {
                    buffer[buffer_pos] = '\0';
                    current_state = PROCESSING;
                }
                break;
                
            case PROCESSING:
                word_count = 0;
                char_count = 0;
                int in_word = 0;
                
                for (int i = 0; i < buffer_pos; i++) {
                    if (isalpha((unsigned char)buffer[i])) {
                        char_count++;
                        if (!in_word) {
                            word_count++;
                            in_word = 1;
                        }
                    } else {
                        in_word = 0;
                    }
                }
                
                current_state = WRITING;
                break;
                
            case WRITING:
                printf("Line: %s\n", buffer);
                printf("Words: %d, Characters: %d\n\n", word_count, char_count);
                current_state = READING;
                break;
                
            case ERROR:
                printf("Error reading input\n");
                break;
                
            case DONE:
                printf("Processing complete\n");
                break;
        }
    }
    
    return current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}