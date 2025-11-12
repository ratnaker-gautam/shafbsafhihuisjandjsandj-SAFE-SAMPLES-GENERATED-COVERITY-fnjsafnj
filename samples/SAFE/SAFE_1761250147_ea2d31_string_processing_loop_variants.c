//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char output[MAX_LEN];
    int i, j, k;
    int len;
    
    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    if (len >= MAX_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            output[j++] = tolower((unsigned char)input[i]);
        }
    }
    output[j] = '\0';
    
    if (j == 0) {
        fprintf(stderr, "No alphabetic characters found\n");
        return 1;
    }
    
    printf("Processed string: %s\n", output);
    
    int freq[26] = {0};
    int total_chars = j;
    
    i = 0;
    while (i < total_chars) {
        if (output[i] >= 'a' && output[i] <= 'z') {
            freq[output[i] - 'a']++;
        }
        i++;
    }
    
    printf("Character frequencies:\n");
    for (k = 0; k < 26; k++) {
        if (freq[k] > 0) {
            printf("%c: %d\n", 'a' + k, freq[k]);
        }
    }
    
    char reversed[MAX_LEN];
    int rev_idx = 0;
    
    for (i = total_chars - 1; i >= 0; i--) {
        reversed[rev_idx++] = output[i];
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int is_palindrome = 1;
    i = 0;
    int half_len = total_chars / 2;
    
    while (i < half_len) {
        if (output[i] != output[total_chars - 1 - i]) {
            is_palindrome = 0;
            break;
        }
        i++;
    }
    
    if (is_palindrome) {
        printf("The processed string is a palindrome\n");
    } else {
        printf("The processed string is not a palindrome\n");
    }
    
    return 0;
}