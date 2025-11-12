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
    
    printf("Enter text lines (empty line to finish, max %d lines):\n", MAX_LINES);
    
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
            printf("Line too long, truncated to %d characters\n", MAX_LINE_LEN - 1);
        }
        
        strncpy(lines[count], buffer, MAX_LINE_LEN - 1);
        lines[count][MAX_LINE_LEN - 1] = '\0';
        count++;
    }
    
    return count;
}

void process_string(char *str) {
    if (str == NULL) return;
    
    int len = strlen(str);
    if (len == 0) return;
    
    int write_pos = 0;
    int in_word = 0;
    
    for (int i = 0; i < len; i++) {
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
        } else {
            str[write_pos++] = str[i];
            in_word = 0;
        }
    }
    
    if (write_pos > 0 && isspace((unsigned char)str[write_pos - 1])) {
        write_pos--;
    }
    
    str[write_pos] = '\0';
}

int main() {
    char lines[MAX_LINES][MAX_LINE_LEN];
    int line_count;
    
    line_count = read_input_lines(lines);
    
    if (line_count == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    printf("\nProcessed text:\n");
    for (int i = 0; i < line_count; i++) {
        process_string(lines[i]);
        printf("%s\n", lines[i]);
    }
    
    printf("\nStatistics:\n");
    int total_chars = 0;
    int total_words = 0;
    
    for (int i = 0; i < line_count; i++) {
        total_chars += strlen(lines[i]);
        
        char temp[MAX_LINE_LEN];
        strncpy(temp, lines[i], MAX_LINE_LEN - 1);
        temp[MAX_LINE_LEN - 1] = '\0';
        
        char *token = strtok(temp, " ");
        while (token != NULL) {
            if (strlen(token) > 0) {
                total_words++;
            }
            token = strtok(NULL, " ");
        }
    }
    
    printf("Total lines: %d\n", line_count);
    printf("Total characters: %d\n", total_chars);
    printf("Total words: %d\n", total_words);
    
    return 0;
}