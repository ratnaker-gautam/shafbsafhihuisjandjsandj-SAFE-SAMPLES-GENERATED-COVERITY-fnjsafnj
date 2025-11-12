//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR };

int main(void) {
    enum State current_state = IDLE;
    char buffer[256];
    size_t buffer_pos = 0;
    int input_char;
    int processed_count = 0;
    const int max_processed = 10;
    
    while (processed_count < max_processed) {
        switch (current_state) {
            case IDLE:
                printf("Enter data (press Enter to process, Ctrl+D to exit): ");
                buffer_pos = 0;
                current_state = READING;
                break;
                
            case READING:
                input_char = getchar();
                if (input_char == EOF) {
                    current_state = ERROR;
                } else if (input_char == '\n') {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        current_state = PROCESSING;
                    } else {
                        current_state = IDLE;
                    }
                } else if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)input_char;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                {
                    size_t i = 0;
                    while (buffer[i] != '\0') {
                        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                            buffer[i] = buffer[i] - 'a' + 'A';
                        }
                        i++;
                    }
                    current_state = WRITING;
                }
                break;
                
            case WRITING:
                printf("Processed: %s\n", buffer);
                processed_count++;
                current_state = IDLE;
                break;
                
            case ERROR:
                printf("Error occurred. Resetting.\n");
                current_state = IDLE;
                break;
        }
        
        if (current_state == IDLE && processed_count >= max_processed) {
            break;
        }
    }
    
    printf("Processing complete. %d items processed.\n", processed_count);
    return 0;
}