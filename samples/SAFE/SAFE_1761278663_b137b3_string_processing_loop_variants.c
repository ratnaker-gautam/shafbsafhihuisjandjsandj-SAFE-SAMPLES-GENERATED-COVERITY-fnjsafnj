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
        fprintf(stderr, "Empty string\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Reverse string\n");
    printf("2. Count vowels\n");
    printf("3. Remove spaces\n");
    printf("4. Convert to uppercase\n");
    printf("Choice: ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
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
            int count = 0;
            const char *ptr = input;
            do {
                char c = tolower(*ptr);
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    count++;
                }
                ptr++;
            } while (*ptr != '\0');
            printf("Vowel count: %d\n", count);
            break;
        }
        
        case 3: {
            size_t i = 0;
            size_t j = 0;
            for (; input[i] != '\0'; i++) {
                if (!isspace((unsigned char)input[i])) {
                    processed[j] = input[i];
                    j++;
                }
            }
            processed[j] = '\0';
            printf("Without spaces: %s\n", processed);
            break;
        }
        
        case 4: {
            size_t i = 0;
            while (input[i] != '\0') {
                processed[i] = toupper((unsigned char)input[i]);
                i++;
            }
            processed[i] = '\0';
            printf("Uppercase: %s\n", processed);
            break;
        }
        
        default:
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }
    
    return 0;
}