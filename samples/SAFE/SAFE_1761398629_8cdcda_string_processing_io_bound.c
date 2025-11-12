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
        if (isalnum((unsigned char)str[read_pos])) {
            if (!in_word) {
                if (write_pos > 0 && write_pos < MAX_LINE_LEN - 1) {
                    str[write_pos++] = ' ';
                }
                in_word = 1;
            }
            if (write_pos < MAX_LINE_LEN - 1) {
                str[write_pos++] = tolower((unsigned char)str[read_pos]);
            }
        } else {
            in_word = 0;
        }
        read_pos++;
    }
    
    str[write_pos] = '\0';
    
    if (write_pos > 0 && str[write_pos - 1] == ' ') {
        str[write_pos - 1] = '\0';
    }
}

int main(void) {
    char lines[MAX_LINES][MAX_LINE_LEN];
    int line_count;
    
    line_count = read_input_lines(lines, MAX_LINES);
    if (line_count < 0) {
        return 1;
    }
    
    if (line_count == 0) {
        fprintf(stderr, "Error: No input provided\n");
        return 1;
    }
    
    for (int i = 0; i < line_count; i++) {
        process_string(lines[i]);
        if (strlen(lines[i]) > 0) {
            printf("%s\n", lines[i]);
        }
    }
    
    return 0;
}