//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READ_CHAR,
    PROCESS_CHAR,
    OUTPUT_RESULT,
    ERROR,
    DONE
};

int main(void) {
    enum state current_state = START;
    char input_buffer[256];
    char current_char;
    int char_count = 0;
    int vowel_count = 0;
    int consonant_count = 0;
    int digit_count = 0;
    int space_count = 0;
    int index = 0;
    int valid_input = 1;

    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case START:
                printf("Enter text (max 255 characters): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    size_t len = strlen(input_buffer);
                    if (len > 0 && input_buffer[len - 1] == '\n') {
                        input_buffer[len - 1] = '\0';
                    }
                    if (strlen(input_buffer) > 0) {
                        current_state = READ_CHAR;
                    } else {
                        printf("Error: Empty input\n");
                        current_state = ERROR;
                    }
                } else {
                    printf("Error: Failed to read input\n");
                    current_state = ERROR;
                }
                break;

            case READ_CHAR:
                if (index < (int)strlen(input_buffer)) {
                    current_char = input_buffer[index];
                    index++;
                    current_state = PROCESS_CHAR;
                } else {
                    current_state = OUTPUT_RESULT;
                }
                break;

            case PROCESS_CHAR:
                char_count++;
                if (isalpha((unsigned char)current_char)) {
                    char lower_char = tolower((unsigned char)current_char);
                    if (lower_char == 'a' || lower_char == 'e' || lower_char == 'i' || 
                        lower_char == 'o' || lower_char == 'u') {
                        vowel_count++;
                    } else {
                        consonant_count++;
                    }
                } else if (isdigit((unsigned char)current_char)) {
                    digit_count++;
                } else if (isspace((unsigned char)current_char)) {
                    space_count++;
                }
                current_state = READ_CHAR;
                break;

            case OUTPUT_RESULT:
                printf("\nCharacter Analysis Results:\n");
                printf("Total characters processed: %d\n", char_count);
                printf("Vowels: %d\n", vowel_count);
                printf("Consonants: %d\n", consonant_count);
                printf("Digits: %d\n", digit_count);
                printf("Whitespace characters: %d\n", space_count);
                printf("Other characters: %d\n", 
                       char_count - (vowel_count + consonant_count + digit_count + space_count));
                current_state = DONE;
                break;

            case ERROR:
                printf("Program terminated due to error\n");
                current_state = DONE;
                break;

            case DONE:
                break;
        }
    }

    return valid_input ? EXIT_SUCCESS : EXIT_FAILURE;
}