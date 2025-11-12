//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

enum State process_input(char* data, int length, int pos, enum State current) {
    if (pos >= length) {
        return FINISHED;
    }
    
    if (current == START) {
        if (data[pos] == 'B') {
            return process_input(data, length, pos + 1, READING);
        } else {
            return ERROR;
        }
    } else if (current == READING) {
        if (data[pos] >= '0' && data[pos] <= '9') {
            printf("Digit: %c\n", data[pos]);
            return process_input(data, length, pos + 1, PROCESSING);
        } else if (data[pos] == 'E') {
            return process_input(data, length, pos + 1, FINISHED);
        } else {
            return ERROR;
        }
    } else if (current == PROCESSING) {
        if (data[pos] >= '0' && data[pos] <= '9') {
            printf("Digit: %c\n", data[pos]);
            return process_input(data, length, pos + 1, PROCESSING);
        } else if (data[pos] == 'E') {
            return process_input(data, length, pos + 1, FINISHED);
        } else {
            return ERROR;
        }
    }
    
    return current;
}

int validate_input(char* input, int max_len) {
    if (input == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len > max_len || len == 0) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    char buffer[256];
    printf("Enter sequence (format: B followed by digits, ending with E): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(buffer, 255)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Processing sequence: %s\n", buffer);
    
    enum State result = process_input(buffer, len, 0, START);
    
    if (result == FINISHED) {
        printf("Sequence processed successfully\n");
        return 0;
    } else {
        fprintf(stderr, "Invalid sequence format\n");
        return 1;
    }
}