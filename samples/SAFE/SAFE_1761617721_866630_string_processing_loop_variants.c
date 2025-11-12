//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH];
    int i, j, len;
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LENGTH) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            processed[j++] = input[i];
        }
    }
    processed[j] = '\0';
    
    if (j == 0) {
        printf("No alphanumeric characters found\n");
        return 0;
    }
    
    char reversed[MAX_LENGTH];
    int k = 0;
    int processed_len = strlen(processed);
    
    i = processed_len - 1;
    while (i >= 0) {
        reversed[k++] = processed[i];
        i--;
    }
    reversed[k] = '\0';
    
    char vowels_removed[MAX_LENGTH];
    int m = 0;
    char *ptr = processed;
    
    do {
        char c = *ptr;
        if (c != '\0') {
            char lower_c = tolower((unsigned char)c);
            if (lower_c != 'a' && lower_c != 'e' && lower_c != 'i' && 
                lower_c != 'o' && lower_c != 'u') {
                vowels_removed[m++] = c;
            }
            ptr++;
        }
    } while (*ptr != '\0');
    vowels_removed[m] = '\0';
    
    char case_swapped[MAX_LENGTH];
    int n = 0;
    
    for (char *p = processed; *p != '\0'; p++) {
        if (isupper((unsigned char)*p)) {
            case_swapped[n++] = tolower((unsigned char)*p);
        } else if (islower((unsigned char)*p)) {
            case_swapped[n++] = toupper((unsigned char)*p);
        } else {
            case_swapped[n++] = *p;
        }
    }
    case_swapped[n] = '\0';
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Vowels removed: %s\n", vowels_removed);
    printf("Case swapped: %s\n", case_swapped);
    
    return 0;
}