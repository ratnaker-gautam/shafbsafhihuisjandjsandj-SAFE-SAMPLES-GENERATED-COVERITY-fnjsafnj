//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN];
    int counts[26] = {0};
    
    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
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
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            processed[j] = tolower((unsigned char)input[i]);
            j++;
        }
    }
    processed[j] = '\0';
    
    if (j == 0) {
        printf("No alphabetic characters found\n");
        return 0;
    }
    
    size_t proc_len = j;
    size_t k = 0;
    while (k < proc_len) {
        int idx = processed[k] - 'a';
        if (idx >= 0 && idx < 26) {
            counts[idx]++;
        }
        k++;
    }
    
    printf("Processed string: %s\n", processed);
    printf("Character frequencies:\n");
    
    int m = 0;
    do {
        if (counts[m] > 0) {
            printf("%c: %d\n", 'a' + m, counts[m]);
        }
        m++;
    } while (m < 26);
    
    char reversed[MAX_LEN];
    size_t n = proc_len;
    size_t p = 0;
    while (n > 0) {
        reversed[p] = processed[n - 1];
        p++;
        n--;
    }
    reversed[p] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int is_palindrome = 1;
    size_t left = 0;
    size_t right = proc_len - 1;
    while (left < right) {
        if (processed[left] != processed[right]) {
            is_palindrome = 0;
            break;
        }
        left++;
        right--;
    }
    
    printf("Palindrome: %s\n", is_palindrome ? "yes" : "no");
    
    return 0;
}