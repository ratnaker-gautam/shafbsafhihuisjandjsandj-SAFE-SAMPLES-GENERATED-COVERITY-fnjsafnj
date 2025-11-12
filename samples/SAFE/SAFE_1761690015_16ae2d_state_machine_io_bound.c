//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state { IDLE, READING, PROCESSING, WRITING, ERROR };

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > 1024) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

void process_data(char* data, size_t len) {
    if (data == NULL || len == 0) return;
    for (size_t i = 0; i < len; i++) {
        if (islower(data[i])) data[i] = toupper(data[i]);
        else if (isupper(data[i])) data[i] = tolower(data[i]);
    }
}

int main(void) {
    enum state current_state = IDLE;
    char buffer[1025];
    size_t input_len;
    int result;
    
    printf("Enter text to process (max 1024 chars): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        current_state = ERROR;
    } else {
        input_len = strlen(buffer);
        if (input_len > 0 && buffer[input_len - 1] == '\n') {
            buffer[input_len - 1] = '\0';
            input_len--;
        }
        current_state = READING;
    }
    
    if (current_state == READING) {
        if (!validate_input(buffer, input_len)) {
            current_state = ERROR;
        } else {
            current_state = PROCESSING;
        }
    }
    
    if (current_state == PROCESSING) {
        process_data(buffer, input_len);
        current_state = WRITING;
    }
    
    if (current_state == WRITING) {
        printf("Processed result: %s\n", buffer);
        result = EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Error: Invalid input or processing error\n");
        result = EXIT_FAILURE;
    }
    
    return result;
}