//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
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
    int char_count = 0;
    int c;
    
    while (1) {
        switch (current_state) {
            case IDLE:
                printf("Enter text (empty line to quit): ");
                buffer_pos = 0;
                current_state = READING;
                break;
                
            case READING:
                c = getchar();
                if (c == EOF) {
                    current_state = ERROR;
                    break;
                }
                
                if (c == '\n') {
                    if (buffer_pos == 0) {
                        current_state = WRITING;
                        break;
                    }
                    buffer[buffer_pos] = '\0';
                    current_state = PROCESSING;
                    break;
                }
                
                if (buffer_pos < 255) {
                    buffer[buffer_pos++] = (char)c;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0) {
                    int in_word = 0;
                    char_count += buffer_pos;
                    
                    for (int i = 0; i < buffer_pos; i++) {
                        if (isalpha((unsigned char)buffer[i])) {
                            if (!in_word) {
                                word_count++;
                                in_word = 1;
                            }
                        } else {
                            in_word = 0;
                        }
                    }
                }
                current_state = IDLE;
                break;
                
            case WRITING:
                if (word_count > 0 || char_count > 0) {
                    printf("Results: %d words, %d characters\n", word_count, char_count);
                } else {
                    printf("No data processed.\n");
                }
                return 0;
                
            case ERROR:
                printf("Error occurred during processing.\n");
                return 1;
        }
    }
}