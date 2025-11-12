//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINES 100
#define MAX_LINE_LEN 256

struct TextStats {
    int line_count;
    int word_count;
    int char_count;
    int longest_line;
};

void trim_trailing_whitespace(char *str) {
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }
}

int count_words_in_line(const char *line) {
    int count = 0;
    int in_word = 0;
    
    for (int i = 0; line[i] != '\0'; i++) {
        if (isspace((unsigned char)line[i])) {
            in_word = 0;
        } else if (!in_word) {
            count++;
            in_word = 1;
        }
    }
    
    return count;
}

void analyze_text(char lines[][MAX_LINE_LEN], int line_count, struct TextStats *stats) {
    stats->line_count = line_count;
    stats->word_count = 0;
    stats->char_count = 0;
    stats->longest_line = 0;
    
    for (int i = 0; i < line_count; i++) {
        int line_len = strlen(lines[i]);
        stats->char_count += line_len;
        
        if (line_len > stats->longest_line) {
            stats->longest_line = line_len;
        }
        
        stats->word_count += count_words_in_line(lines[i]);
    }
}

int read_input_lines(char lines[][MAX_LINE_LEN]) {
    int count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter text lines (empty line to finish, max %d lines):\n", MAX_LINES);
    
    while (count < MAX_LINES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        trim_trailing_whitespace(buffer);
        
        if (buffer[0] == '\0') {
            break;
        }
        
        if (strlen(buffer) >= MAX_LINE_LEN - 1) {
            printf("Line too long, truncated to %d characters\n", MAX_LINE_LEN - 1);
        }
        
        strncpy(lines[count], buffer, MAX_LINE_LEN - 1);
        lines[count][MAX_LINE_LEN - 1] = '\0';
        count++;
    }
    
    return count;
}

void display_stats(const struct TextStats *stats) {
    printf("\nText Analysis Results:\n");
    printf("Lines: %d\n", stats->line_count);
    printf("Words: %d\n", stats->word_count);
    printf("Characters: %d\n", stats->char_count);
    printf("Longest line: %d characters\n", stats->longest_line);
}

void process_and_display_lines(char lines[][MAX_LINE_LEN], int line_count) {
    printf("\nProcessed text (trailing whitespace removed):\n");
    for (int i = 0; i < line_count; i++) {
        printf("%s\n", lines[i]);
    }
}

int main() {
    char lines[MAX_LINES][MAX_LINE_LEN];
    struct TextStats stats;
    
    int line_count = read_input_lines(lines);
    
    if (line_count == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    analyze_text(lines, line_count, &stats);
    process_and_display_lines(lines, line_count);
    display_stats(&stats);
    
    return 0;
}