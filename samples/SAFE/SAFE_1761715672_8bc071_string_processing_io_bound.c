//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINES 100
#define MAX_LINE_LEN 256

int main() {
    char lines[MAX_LINES][MAX_LINE_LEN];
    int line_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter text lines (empty line to finish, max %d lines):\n", MAX_LINES);
    
    while (line_count < MAX_LINES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        if (len >= MAX_LINE_LEN) {
            printf("Line too long, skipping.\n");
            continue;
        }
        
        strncpy(lines[line_count], buffer, MAX_LINE_LEN - 1);
        lines[line_count][MAX_LINE_LEN - 1] = '\0';
        line_count++;
    }
    
    if (line_count == 0) {
        printf("No lines entered.\n");
        return 0;
    }
    
    printf("\nProcessing lines...\n");
    
    for (int i = 0; i < line_count; i++) {
        char processed[MAX_LINE_LEN];
        int proc_idx = 0;
        size_t len = strlen(lines[i]);
        
        for (size_t j = 0; j < len && proc_idx < MAX_LINE_LEN - 1; j++) {
            char c = lines[i][j];
            
            if (isalnum((unsigned char)c) || isspace((unsigned char)c)) {
                if (proc_idx > 0 && isspace((unsigned char)processed[proc_idx - 1]) && isspace((unsigned char)c)) {
                    continue;
                }
                
                if (isalpha((unsigned char)c)) {
                    processed[proc_idx++] = tolower((unsigned char)c);
                } else {
                    processed[proc_idx++] = c;
                }
            }
        }
        
        if (proc_idx > 0 && isspace((unsigned char)processed[proc_idx - 1])) {
            proc_idx--;
        }
        
        processed[proc_idx] = '\0';
        
        if (proc_idx > 0) {
            printf("Line %d: %s\n", i + 1, processed);
        } else {
            printf("Line %d: (empty after processing)\n", i + 1);
        }
    }
    
    printf("\nStatistics:\n");
    printf("Total lines processed: %d\n", line_count);
    
    int total_chars = 0;
    int total_words = 0;
    
    for (int i = 0; i < line_count; i++) {
        total_chars += strlen(lines[i]);
        
        char temp[MAX_LINE_LEN];
        strncpy(temp, lines[i], sizeof(temp) - 1);
        temp[sizeof(temp) - 1] = '\0';
        
        char *token = strtok(temp, " \t");
        while (token != NULL) {
            int valid_word = 0;
            for (size_t j = 0; token[j] != '\0'; j++) {
                if (isalnum((unsigned char)token[j])) {
                    valid_word = 1;
                    break;
                }
            }
            if (valid_word) {
                total_words++;
            }
            token = strtok(NULL, " \t");
        }
    }
    
    printf("Total characters: %d\n", total_chars);
    printf("Total words: %d\n", total_words);
    
    return 0;
}