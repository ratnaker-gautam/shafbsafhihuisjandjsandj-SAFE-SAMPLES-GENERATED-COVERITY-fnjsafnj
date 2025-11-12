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
                    if (buffer_pos > 0) {
                        current_state = PROCESSING;
                    } else {
                        current_state = DONE;
                    }
                    break;
                }
                
                if (buffer_pos < 255) {
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
                
                for (int i = 0; i < buffer_pos; i++) {
                    if (buffer[i] == '\n') {
                        buffer[i] = ' ';
                    }
                }
                
                current_state = OUTPUT;
                break;
                
            case OUTPUT:
                printf("Processed text: ");
                for (int i = 0; i < buffer_pos && i < 255; i++) {
                    putchar(buffer[i]);
                }
                printf("\nWords: %d\nCharacters: %d\n", word_count, char_count);
                
                current_state = IDLE;
                break;
                
            case ERROR:
                printf("Error: Buffer overflow detected\n");
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Processing complete.\n");
    return 0;
}