//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINES 100
#define MAX_LINE_LEN 256

int read_input_lines(char lines[][MAX_LINE_LEN]) {
    int count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter text lines (empty line to finish):\n");
    while (count < MAX_LINES) {
        if (fgets(buffer, MAX_LINE_LEN, stdin) == NULL) {
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
        
        if (len >= MAX_LINE_LEN - 1) {
            fprintf(stderr, "Line too long, skipping\n");
            continue;
        }
        
        strncpy(lines[count], buffer, MAX_LINE_LEN - 1);
        lines[count][MAX_LINE_LEN - 1] = '\0';
        count++;
    }
    
    return count;
}

void process_string(char *str) {
    if (str == NULL) return;
    
    size_t len = strlen(str);
    if (len == 0) return;
    
    int write_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)str[i])) {
            if (!in_word) {
                str[write_pos++] = toupper((unsigned char)str[i]);
                in_word = 1;
            } else {
                str[write_pos++] = tolower((unsigned char)str[i]);
            }
        } else if (isspace((unsigned char)str[i])) {
            if (in_word) {
                str[write_pos++] = ' ';
                in_word = 0;
            }
        }
    }
    
    if (write_pos > 0 && str[write_pos - 1] == ' ') {
        write_pos--;
    }
    
    str[write_pos] = '\0';
}

int main(void) {
    char lines[MAX_LINES][MAX_LINE_LEN];
    int line_count;
    
    line_count = read_input_lines(lines);
    
    if (line_count == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    printf("\nProcessed output:\n");
    for (int i = 0; i < line_count; i++) {
        process_string(lines[i]);
        printf("%s\n", lines[i]);
    }
    
    return 0;
}