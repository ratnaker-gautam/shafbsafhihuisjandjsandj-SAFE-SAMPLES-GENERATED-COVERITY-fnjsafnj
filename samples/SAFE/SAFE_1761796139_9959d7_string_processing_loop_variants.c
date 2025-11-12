//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
    int i, j, k;
    
    printf("Enter a string to process: ");
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
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
            processed[j++] = tolower((unsigned char)input[i]);
        } else if (isdigit((unsigned char)input[i])) {
            int digit = input[i] - '0';
            for (k = 0; k < digit && j < MAX_LEN * 2 - 1; k++) {
                processed[j++] = '*';
            }
        } else {
            processed[j++] = input[i];
        }
        
        if (j >= MAX_LEN * 2 - 1) {
            break;
        }
    }
    processed[j] = '\0';
    
    int count = 0;
    i = 0;
    while (processed[i] != '\0') {
        if (processed[i] == '*') {
            count++;
        }
        i++;
    }
    
    char reversed[MAX_LEN * 2];
    int rev_idx = 0;
    for (i = j - 1; i >= 0; i--) {
        reversed[rev_idx++] = processed[i];
    }
    reversed[rev_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Star count: %d\n", count);
    
    int vowel_count = 0;
    char *ptr = processed;
    do {
        char c = *ptr;
        if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' || 
            c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vowel_count++;
        }
        ptr++;
    } while (*ptr != '\0');
    
    printf("Vowel count: %d\n", vowel_count);
    
    return 0;
}