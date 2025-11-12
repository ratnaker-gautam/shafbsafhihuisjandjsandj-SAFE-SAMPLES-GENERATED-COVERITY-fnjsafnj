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
    int ch;
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case IDLE:
                printf("Enter text (empty line to finish): ");
                buffer_pos = 0;
                current_state = READING;
                break;
                
            case READING:
                ch = getchar();
                if (ch == EOF) {
                    current_state = DONE;
                } else if (ch == '\n') {
                    if (buffer_pos == 0) {
                        current_state = DONE;
                    } else {
                        buffer[buffer_pos] = '\0';
                        current_state = PROCESSING;
                    }
                } else if (buffer_pos < 255) {
                    buffer[buffer_pos++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                {
                    int i = 0;
                    int in_word = 0;
                    while (buffer[i] != '\0') {
                        if (isalpha((unsigned char)buffer[i])) {
                            if (!in_word) {
                                word_count++;
                                in_word = 1;
                            }
                        } else {
                            in_word = 0;
                        }
                        i++;
                    }
                    current_state = WRITING;
                }
                break;
                
            case WRITING:
                printf("Line: %s\n", buffer);
                printf("Words in line: %d\n", word_count);
                word_count = 0;
                current_state = IDLE;
                break;
                
            case ERROR:
                printf("Error: Buffer overflow\n");
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Program completed\n");
    return 0;
}