//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state { IDLE, READING, PROCESSING, OUTPUT, ERROR, DONE };

int main(void) {
    enum state current_state = IDLE;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int char_count = 0;
    int c;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while (current_state != DONE && current_state != ERROR) {
        c = getchar();
        
        switch (current_state) {
            case IDLE:
                if (c == EOF) {
                    current_state = DONE;
                } else if (!isspace(c) && c != EOF) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)c;
                        char_count++;
                        current_state = READING;
                    } else {
                        current_state = ERROR;
                    }
                }
                break;
                
            case READING:
                if (c == EOF || isspace(c)) {
                    if (buffer_pos > 0 && buffer_pos < 256) {
                        buffer[buffer_pos] = '\0';
                        current_state = PROCESSING;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)c;
                        char_count++;
                    } else {
                        current_state = ERROR;
                    }
                }
                break;
                
            case PROCESSING:
                word_count++;
                current_state = OUTPUT;
                break;
                
            case OUTPUT:
                printf("Word %d: %s\n", word_count, buffer);
                buffer_pos = 0;
                if (c == EOF) {
                    current_state = DONE;
                } else {
                    current_state = IDLE;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Error: Buffer overflow or invalid input\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == DONE) {
        printf("\nSummary: %d words, %d characters\n", word_count, char_count);
    }
    
    return 0;
}