//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024
#define MAX_WORDS 256

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT) return 0;
    return 1;
}

void reverse_word(char *start, char *end) {
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

int process_string(char *input, char *output, size_t output_size) {
    if (!validate_input(input) || output == NULL || output_size < MAX_INPUT) {
        return 0;
    }

    char *words[MAX_WORDS];
    int word_count = 0;
    char *ptr = input;
    
    while (*ptr != '\0' && word_count < MAX_WORDS) {
        while (*ptr != '\0' && isspace(*ptr)) {
            ptr++;
        }
        if (*ptr == '\0') break;
        
        words[word_count] = ptr;
        word_count++;
        
        while (*ptr != '\0' && !isspace(*ptr)) {
            ptr++;
        }
        if (*ptr != '\0') {
            *ptr = '\0';
            ptr++;
        }
    }

    char *out_ptr = output;
    for (int i = word_count - 1; i >= 0; i--) {
        char *word = words[i];
        size_t word_len = strlen(word);
        
        if (out_ptr + word_len + 1 >= output + output_size) {
            return 0;
        }
        
        reverse_word(word, word + word_len - 1);
        
        char *word_ptr = word;
        while (*word_ptr != '\0') {
            *out_ptr = *word_ptr;
            out_ptr++;
            word_ptr++;
        }
        
        if (i > 0) {
            *out_ptr = ' ';
            out_ptr++;
        }
    }
    *out_ptr = '\0';
    
    return 1;
}

int main(void) {
    char input[MAX_INPUT];
    char output[MAX_INPUT];
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!process_string(input, output, sizeof(output))) {
        fprintf(stderr, "Error processing string\n");
        return 1;
    }
    
    printf("Processed: %s\n", output);
    return 0;
}