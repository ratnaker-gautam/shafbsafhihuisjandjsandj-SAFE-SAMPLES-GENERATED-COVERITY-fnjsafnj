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
    
    printf("Enter a string: ");
    if (fgets(input, MAX_LEN, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
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
        if (isalpha(input[i])) {
            output[j] = toupper(input[i]);
            j++;
        }
    }
    output[j] = '\0';
    
    printf("Uppercase letters only: %s\n", output);
    
    int letter_count = 0;
    k = 0;
    while (output[k] != '\0') {
        letter_count++;
        k++;
    }
    printf("Letter count: %d\n", letter_count);
    
    char reversed[MAX_LEN];
    int m = letter_count - 1;
    int n = 0;
    do {
        if (m >= 0) {
            reversed[n] = output[m];
            n++;
            m--;
        }
    } while (m >= 0);
    reversed[n] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int palindrome = 1;
    int left = 0;
    int right = letter_count - 1;
    while (left < right) {
        if (output[left] != output[right]) {
            palindrome = 0;
            break;
        }
        left++;
        right--;
    }
    
    printf("Is palindrome: %s\n", palindrome ? "Yes" : "No");
    
    int freq[26] = {0};
    int idx;
    for (i = 0; i < letter_count; i++) {
        idx = output[i] - 'A';
        if (idx >= 0 && idx < 26) {
            freq[idx]++;
        }
    }
    
    printf("Letter frequencies:\n");
    char ch = 'A';
    while (ch <= 'Z') {
        if (freq[ch - 'A'] > 0) {
            printf("%c: %d\n", ch, freq[ch - 'A']);
        }
        ch++;
    }
    
    return 0;
}