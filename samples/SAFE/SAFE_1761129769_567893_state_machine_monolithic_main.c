//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    END
};

int main(void) {
    char input[1024];
    char buffer[1024];
    int buf_index = 0;
    enum State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int number_count = 0;
    int i = 0;
    int input_len = 0;

    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    while (i <= input_len) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    buf_index = 0;
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    buf_index = 0;
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = tolower(c);
                    }
                } else if (c == ' ' || c == '\t') {
                    current_state = SKIPPING_SPACE;
                } else if (c == '\0') {
                    current_state = END;
                }
                i++;
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    i++;
                } else {
                    if (buf_index < sizeof(buffer)) {
                        buffer[buf_index] = '\0';
                    }
                    int num = atoi(buffer);
                    number_sum += num;
                    number_count++;
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = tolower(c);
                    }
                    i++;
                } else {
                    if (buf_index < sizeof(buffer)) {
                        buffer[buf_index] = '\0';
                    }
                    if (buf_index > 0) {
                        word_count++;
                    }
                    current_state = START;
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == ' ' || c == '\t') {
                    i++;
                } else {
                    current_state = START;
                }
                break;
                
            case END:
                i++;
                break;
        }
    }

    if (current_state == READING_NUMBER) {
        if (buf_index < sizeof(buffer)) {
            buffer[buf_index] = '\0';
        }
        int num = atoi(buffer);
        number_sum += num;
        number_count++;
    } else if (current_state == READING_WORD) {
        if (buf_index < sizeof(buffer)) {
            buffer[buf_index] = '\0';
        }
        if (buf_index > 0) {
            word_count++;
        }
    }

    printf("Words found: %d\n", word_count);
    printf("Numbers found: %d\n", number_count);
    if (number_count > 0) {
        printf("Sum of numbers: %d\n", number_sum);
        printf("Average of numbers: %.2f\n", (double)number_sum / number_count);
    } else {
        printf("No numbers to sum or average\n");
    }

    return 0;
}