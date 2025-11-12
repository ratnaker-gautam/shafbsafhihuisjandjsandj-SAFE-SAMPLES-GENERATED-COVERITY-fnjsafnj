//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char input[1024];
    char buffer[1024];
    int buffer_index = 0;
    enum State state = START;
    int number_sum = 0;
    int word_count = 0;
    int number_count = 0;
    int i = 0;
    int c;

    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    while (input[i] != '\0') {
        c = input[i];

        switch (state) {
            case START:
                if (isspace(c)) {
                    i++;
                } else if (isdigit(c)) {
                    state = READING_NUMBER;
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (isalpha(c)) {
                    state = READING_WORD;
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else {
                    state = ERROR;
                }
                break;

            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_index] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    number_count++;
                    state = START;
                    i++;
                } else {
                    state = ERROR;
                }
                break;

            case READING_WORD:
                if (isalpha(c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    state = START;
                    i++;
                } else {
                    state = ERROR;
                }
                break;

            case ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;

            case DONE:
                break;
        }

        if (state == ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
    }

    if (state == READING_NUMBER) {
        buffer[buffer_index] = '\0';
        int num = atoi(buffer);
        number_sum += num;
        number_count++;
    } else if (state == READING_WORD) {
        word_count++;
    }

    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    if (number_count > 0) {
        printf("Number sum: %d\n", number_sum);
    }

    return 0;
}