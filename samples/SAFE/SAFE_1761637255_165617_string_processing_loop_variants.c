//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH];
    int choice;
    
    printf("Enter a string (max %d chars): ", MAX_LENGTH - 1);
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
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Reverse string\n");
    printf("2. Convert to uppercase\n");
    printf("3. Remove vowels\n");
    printf("4. Count words\n");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    switch (choice) {
        case 1: {
            size_t i = 0;
            size_t j = len;
            while (j > 0) {
                j--;
                processed[i] = input[j];
                i++;
            }
            processed[i] = '\0';
            printf("Reversed: %s\n", processed);
            break;
        }
        
        case 2: {
            size_t pos = 0;
            do {
                if (pos < len) {
                    processed[pos] = toupper((unsigned char)input[pos]);
                    pos++;
                }
            } while (pos < len);
            processed[len] = '\0';
            printf("Uppercase: %s\n", processed);
            break;
        }
        
        case 3: {
            size_t src = 0;
            size_t dst = 0;
            for (src = 0; src < len; src++) {
                char c = tolower((unsigned char)input[src]);
                if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
                    processed[dst] = input[src];
                    dst++;
                }
            }
            processed[dst] = '\0';
            printf("Without vowels: %s\n", processed);
            break;
        }
        
        case 4: {
            int word_count = 0;
            int in_word = 0;
            size_t idx = 0;
            while (idx < len) {
                if (isspace((unsigned char)input[idx])) {
                    in_word = 0;
                } else {
                    if (!in_word) {
                        word_count++;
                        in_word = 1;
                    }
                }
                idx++;
            }
            printf("Word count: %d\n", word_count);
            break;
        }
        
        default:
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }
    
    return 0;
}