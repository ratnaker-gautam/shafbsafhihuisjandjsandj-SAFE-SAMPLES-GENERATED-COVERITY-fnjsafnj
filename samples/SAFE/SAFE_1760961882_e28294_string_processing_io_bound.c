//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
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

int count_words(const char *str) {
    int count = 0;
    int in_word = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (isspace((unsigned char)str[i])) {
            in_word = 0;
        } else if (!in_word) {
            count++;
            in_word = 1;
        }
    }
    return count;
}

int process_text(struct TextStats *stats, char lines[][MAX_LINE_LEN]) {
    stats->line_count = 0;
    stats->word_count = 0;
    stats->char_count = 0;
    stats->longest_line = 0;
    
    printf("Enter text (empty line to finish, max %d lines):\n", MAX_LINES);
    
    for (int i = 0; i < MAX_LINES; i++) {
        if (fgets(lines[i], MAX_LINE_LEN, stdin) == NULL) {
            break;
        }
        
        trim_trailing_whitespace(lines[i]);
        
        if (lines[i][0] == '\0') {
            break;
        }
        
        int line_len = strlen(lines[i]);
        if (line_len > stats->longest_line) {
            stats->longest_line = line_len;
        }
        
        stats->char_count += line_len;
        stats->word_count += count_words(lines[i]);
        stats->line_count++;
        
        if (stats->line_count >= MAX_LINES) {
            break;
        }
    }
    
    return stats->line_count > 0;
}

void display_stats(const struct TextStats *stats) {
    printf("\nText Statistics:\n");
    printf("Lines: %d\n", stats->line_count);
    printf("Words: %d\n", stats->word_count);
    printf("Characters: %d\n", stats->char_count);
    printf("Longest line: %d characters\n", stats->longest_line);
}

void display_processed_lines(char lines[][MAX_LINE_LEN], int line_count) {
    printf("\nProcessed lines (trimmed):\n");
    for (int i = 0; i < line_count; i++) {
        printf("[%d] %s\n", i + 1, lines[i]);
    }
}

int main() {
    struct TextStats stats;
    char lines[MAX_LINES][MAX_LINE_LEN];
    
    if (!process_text(&stats, lines)) {
        printf("No text entered.\n");
        return 1;
    }
    
    display_stats(&stats);
    display_processed_lines(lines, stats.line_count);
    
    return 0;
}