//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINES 100
#define MAX_LINE_LEN 256

int read_input_lines(char lines[][MAX_LINE_LEN], int max_lines) {
    int count = 0;
    char buffer[MAX_LINE_LEN];
    
    while (count < max_lines && fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len >= MAX_LINE_LEN - 1) {
            fprintf(stderr, "Error: Line too long\n");
            return -1;
        }
        
        if (len > 0) {
            strncpy(lines[count], buffer, MAX_LINE_LEN - 1);
            lines[count][MAX_LINE_LEN - 1] = '\0';
            count++;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return -1;
    }
    
    return count;
}

void process_string(char *str) {
    if (str == NULL) return;
    
    int write_pos = 0;
    int read_pos = 0;
    int in_word = 0;
    
    while (str[read_pos] != '\0') {
        if (isalpha((unsigned char)str[read_pos])) {
            if (!in_word) {
                str[write_pos++] = toupper((unsigned char)str[read_pos]);
                in_word = 1;
            } else {
                str[write_pos++] = tolower((unsigned char)str[read_pos]);
            }
        } else if (isspace((unsigned char)str[read_pos])) {
            if (in_word && write_pos > 0) {
                str[write_pos++] = ' ';
                in_word = 0;
            }
        } else {
            if (write_pos > 0 && !isspace((unsigned char)str[write_pos - 1])) {
                str[write_pos++] = ' ';
            }
        }
        read_pos++;
    }
    
    if (write_pos > 0 && isspace((unsigned char)str[write_pos - 1])) {
        write_pos--;
    }
    
    str[write_pos] = '\0';
}

int main(void) {
    char lines[MAX_LINES][MAX_LINE_LEN];
    
    printf("Enter text lines (empty line or Ctrl+D to finish):\n");
    
    int line_count = read_input_lines(lines, MAX_LINES);
    if (line_count < 0) {
        return 1;
    }
    
    if (line_count == 0) {
        printf("No input provided.\n");
        return 0;
    }
    
    printf("\nProcessed output:\n");
    for (int i = 0; i < line_count; i++) {
        process_string(lines[i]);
        printf("%s\n", lines[i]);
    }
    
    return 0;
}