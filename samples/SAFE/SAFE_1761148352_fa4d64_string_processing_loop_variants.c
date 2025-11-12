//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH * 2];
    int i, j, count;
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LENGTH - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    i = 0;
    while (input[i] != '\0') {
        if (isalpha((unsigned char)input[i])) {
            count = 1;
            int k = i + 1;
            for (; isalpha((unsigned char)input[k]) && input[k] == input[i]; k++) {
                count++;
            }
            
            if (j + 3 >= sizeof(processed)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            processed[j++] = input[i];
            if (count > 1) {
                char num_str[16];
                int num_len = snprintf(num_str, sizeof(num_str), "%d", count);
                if (num_len < 0 || num_len >= (int)sizeof(num_str)) {
                    fprintf(stderr, "Number formatting error\n");
                    return 1;
                }
                
                if (j + num_len >= sizeof(processed)) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                
                for (int m = 0; m < num_len; m++) {
                    processed[j++] = num_str[m];
                }
            }
            
            i += count;
        } else {
            if (j >= sizeof(processed)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[j++] = input[i++];
        }
    }
    
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    char reversed[MAX_LENGTH];
    size_t input_len = strlen(input);
    if (input_len >= sizeof(reversed)) {
        fprintf(stderr, "Reversal buffer overflow\n");
        return 1;
    }
    
    for (size_t idx = 0; idx < input_len; idx++) {
        reversed[input_len - 1 - idx] = input[idx];
    }
    reversed[input_len] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int vowel_count = 0;
    int consonant_count = 0;
    const char *ptr = input;
    while (*ptr) {
        char c = tolower((unsigned char)*ptr);
        if (isalpha((unsigned char)*ptr)) {
            switch (c) {
                case 'a': case 'e': case 'i': case 'o': case 'u':
                    vowel_count++;
                    break;
                default:
                    consonant_count++;
                    break;
            }
        }
        ptr++;
    }
    
    printf("Vowels: %d, Consonants: %d\n", vowel_count, consonant_count);
    
    return 0;
}