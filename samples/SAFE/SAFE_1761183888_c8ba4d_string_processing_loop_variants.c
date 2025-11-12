//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH * 2];
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
    
    if (len >= MAX_LENGTH - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
            processed[j++] = tolower((unsigned char)input[i]);
        } else if (isdigit((unsigned char)input[i])) {
            processed[j++] = input[i];
            processed[j++] = '#';
        } else {
            processed[j++] = '_';
        }
        
        if (j >= sizeof(processed) - 2) {
            processed[sizeof(processed) - 1] = '\0';
            break;
        }
    }
    processed[j] = '\0';
    
    printf("Processed string: ");
    k = 0;
    while (processed[k] != '\0') {
        putchar(processed[k]);
        k++;
    }
    putchar('\n');
    
    int vowel_count = 0;
    int digit_count = 0;
    char *ptr = processed;
    do {
        char c = *ptr;
        if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            vowel_count++;
        } else if (isdigit((unsigned char)c)) {
            digit_count++;
        }
        ptr++;
    } while (*ptr != '\0');
    
    printf("Vowels in processed string: %d\n", vowel_count);
    printf("Digits in processed string: %d\n", digit_count);
    
    char reversed[MAX_LENGTH * 2];
    int rev_idx = 0;
    int pos = j - 1;
    while (pos >= 0) {
        reversed[rev_idx++] = processed[pos];
        pos--;
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed processed string: ");
    for (int idx = 0; idx < rev_idx; idx++) {
        putchar(reversed[idx]);
    }
    putchar('\n');
    
    return 0;
}