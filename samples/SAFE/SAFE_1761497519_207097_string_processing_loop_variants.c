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
        printf("Empty string provided\n");
        return 0;
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
        printf("No alphabetic characters found\n");
        return 0;
    }
    
    printf("Processed string: %s\n", output);
    
    int processed_len = strlen(output);
    int is_palindrome = 1;
    
    k = processed_len - 1;
    i = 0;
    while (i < k) {
        if (output[i] != output[k]) {
            is_palindrome = 0;
            break;
        }
        i++;
        k--;
    }
    
    if (is_palindrome) {
        printf("The processed string is a palindrome\n");
    } else {
        printf("The processed string is not a palindrome\n");
    }
    
    int char_count[26] = {0};
    int total_chars = 0;
    
    for (i = 0; i < processed_len; i++) {
        if (output[i] >= 'a' && output[i] <= 'z') {
            char_count[output[i] - 'a']++;
            total_chars++;
        }
    }
    
    if (total_chars > 0) {
        printf("Character frequencies:\n");
        for (i = 0; i < 26; i++) {
            if (char_count[i] > 0) {
                printf("%c: %d (%.2f%%)\n", 'a' + i, char_count[i], 
                       (float)char_count[i] * 100.0f / total_chars);
            }
        }
    }
    
    return 0;
}