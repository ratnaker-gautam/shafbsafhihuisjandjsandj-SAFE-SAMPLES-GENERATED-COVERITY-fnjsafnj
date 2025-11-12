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
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Count vowels\n");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    switch (choice) {
        case 1: {
            size_t i = 0;
            while (i < len) {
                processed[i] = toupper((unsigned char)input[i]);
                i++;
            }
            processed[len] = '\0';
            printf("Uppercase: %s\n", processed);
            break;
        }
        
        case 2: {
            for (size_t i = 0; i < len; i++) {
                processed[i] = tolower((unsigned char)input[i]);
            }
            processed[len] = '\0';
            printf("Lowercase: %s\n", processed);
            break;
        }
        
        case 3: {
            size_t i = len;
            size_t j = 0;
            do {
                if (i > 0) {
                    processed[j] = input[i - 1];
                    j++;
                    i--;
                }
            } while (i > 0);
            processed[len] = '\0';
            printf("Reversed: %s\n", processed);
            break;
        }
        
        case 4: {
            int vowel_count = 0;
            const char *vowels = "aeiouAEIOU";
            char *ptr = input;
            
            while (*ptr != '\0') {
                const char *v = vowels;
                int found = 0;
                while (*v != '\0') {
                    if (*ptr == *v) {
                        found = 1;
                        break;
                    }
                    v++;
                }
                if (found) {
                    vowel_count++;
                }
                ptr++;
            }
            printf("Vowel count: %d\n", vowel_count);
            break;
        }
        
        default:
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }
    
    return 0;
}