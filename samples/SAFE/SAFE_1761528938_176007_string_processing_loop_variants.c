//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN];
    int choice;
    
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
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Reverse string\n");
    printf("2. Convert to uppercase\n");
    printf("3. Remove vowels\n");
    printf("4. Count words\n");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    switch (choice) {
        case 1: {
            size_t i = 0;
            size_t j = len;
            processed[j] = '\0';
            while (j > 0) {
                j--;
                processed[i] = input[j];
                i++;
            }
            printf("Reversed: %s\n", processed);
            break;
        }
        
        case 2: {
            size_t i = 0;
            do {
                if (i >= len) break;
                processed[i] = toupper((unsigned char)input[i]);
                i++;
            } while (i < len);
            processed[len] = '\0';
            printf("Uppercase: %s\n", processed);
            break;
        }
        
        case 3: {
            size_t i = 0;
            size_t j = 0;
            for (; i < len; i++) {
                char c = tolower((unsigned char)input[i]);
                if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
                    if (j < MAX_LEN - 1) {
                        processed[j] = input[i];
                        j++;
                    }
                }
            }
            processed[j] = '\0';
            printf("Without vowels: %s\n", processed);
            break;
        }
        
        case 4: {
            int count = 0;
            int in_word = 0;
            size_t i = 0;
            while (i < len) {
                if (isspace((unsigned char)input[i])) {
                    in_word = 0;
                } else {
                    if (!in_word) {
                        count++;
                        in_word = 1;
                    }
                }
                i++;
            }
            printf("Word count: %d\n", count);
            break;
        }
        
        default:
            fprintf(stderr, "Invalid operation choice\n");
            return 1;
    }
    
    return 0;
}