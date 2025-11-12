//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 100
#define MAX_LINE_LEN 256

int read_input_lines(char lines[][MAX_LINE_LEN], int max_lines) {
    int count = 0;
    char buffer[MAX_LINE_LEN];
    
    while (count < max_lines && fgets(buffer, MAX_LINE_LEN, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len >= MAX_LINE_LEN - 1) {
            fprintf(stderr, "Error: Input line too long\n");
            return -1;
        }
        
        if (len > 0) {
            strncpy(lines[count], buffer, MAX_LINE_LEN - 1);
            lines[count][MAX_LINE_LEN - 1] = '\0';
            count++;
        }
    }
    
    return count;
}

void process_string(char *str) {
    if (str == NULL) return;
    
    int len = strlen(str);
    if (len == 0) return;
    
    int word_start = 1;
    for (int i = 0; i < len; i++) {
        if (isspace((unsigned char)str[i])) {
            word_start = 1;
        } else if (word_start) {
            str[i] = toupper((unsigned char)str[i]);
            word_start = 0;
        } else {
            str[i] = tolower((unsigned char)str[i]);
        }
    }
}

int main(void) {
    char lines[MAX_LINES][MAX_LINE_LEN];
    
    printf("Enter text lines (empty line or EOF to finish):\n");
    
    int line_count = read_input_lines(lines, MAX_LINES);
    if (line_count < 0) {
        return EXIT_FAILURE;
    }
    
    if (line_count == 0) {
        printf("No input provided.\n");
        return EXIT_SUCCESS;
    }
    
    printf("\nProcessed text (title case):\n");
    for (int i = 0; i < line_count; i++) {
        char processed[MAX_LINE_LEN];
        strncpy(processed, lines[i], MAX_LINE_LEN - 1);
        processed[MAX_LINE_LEN - 1] = '\0';
        
        process_string(processed);
        printf("%s\n", processed);
    }
    
    return EXIT_SUCCESS;
}