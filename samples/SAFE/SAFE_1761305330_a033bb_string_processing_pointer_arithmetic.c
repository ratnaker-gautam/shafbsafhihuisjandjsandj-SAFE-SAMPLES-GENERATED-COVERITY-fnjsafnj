//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main(void) {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN];
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *input_ptr = input;
    char *output_ptr = output;
    size_t input_len = strlen(input);
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t output_remaining = sizeof(output) - 1;
    
    while (*input_ptr != '\0' && output_remaining > 0) {
        if (isalpha((unsigned char)*input_ptr)) {
            if (islower((unsigned char)*input_ptr)) {
                *output_ptr = (char)toupper((unsigned char)*input_ptr);
            } else {
                *output_ptr = (char)tolower((unsigned char)*input_ptr);
            }
            output_ptr++;
            output_remaining--;
        } else if (isdigit((unsigned char)*input_ptr)) {
            *output_ptr = '#';
            output_ptr++;
            output_remaining--;
        } else if (!isspace((unsigned char)*input_ptr)) {
            *output_ptr = *input_ptr;
            output_ptr++;
            output_remaining--;
        }
        input_ptr++;
    }
    
    *output_ptr = '\0';
    
    if (output_remaining == 0) {
        fprintf(stderr, "Output buffer full\n");
        return 1;
    }
    
    printf("Processed string: %s\n", output);
    
    char *reverse_ptr = output_ptr - 1;
    printf("Reversed words: ");
    
    char *word_start = output;
    char *word_end = output;
    
    while (word_end < output_ptr) {
        while (word_end < output_ptr && !isspace((unsigned char)*word_end)) {
            word_end++;
        }
        
        char *rev = word_end - 1;
        while (rev >= word_start) {
            putchar(*rev);
            rev--;
        }
        
        if (word_end < output_ptr) {
            putchar(*word_end);
            word_end++;
            word_start = word_end;
        }
    }
    putchar('\n');
    
    return 0;
}