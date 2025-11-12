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

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

void analyze_text(const char* text, struct TextStats* stats) {
    stats->line_count = 0;
    stats->word_count = 0;
    stats->char_count = 0;
    stats->longest_line = 0;
    
    if (text == NULL || strlen(text) == 0) {
        return;
    }
    
    int current_line_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; text[i] != '\0'; i++) {
        stats->char_count++;
        current_line_len++;
        
        if (text[i] == '\n') {
            stats->line_count++;
            if (current_line_len > stats->longest_line) {
                stats->longest_line = current_line_len;
            }
            current_line_len = 0;
        }
        
        if (is_word_char(text[i])) {
            if (!in_word) {
                stats->word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    if (current_line_len > 0) {
        stats->line_count++;
        if (current_line_len > stats->longest_line) {
            stats->longest_line = current_line_len;
        }
    }
}

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
        }
        
        if (strlen(buffer) == 0) {
            break;
        }
        
        if (strlen(buffer) >= MAX_LINE_LEN - 1) {
            printf("Line too long, truncated.\n");
        }
        
        strncpy(lines[count], buffer, MAX_LINE_LEN - 1);
        lines[count][MAX_LINE_LEN - 1] = '\0';
        count++;
    }
    
    return count;
}

void build_combined_text(char lines[][MAX_LINE_LEN], int line_count, char* combined, size_t combined_size) {
    combined[0] = '\0';
    
    for (int i = 0; i < line_count; i++) {
        size_t current_len = strlen(combined);
        size_t line_len = strlen(lines[i]);
        
        if (current_len + line_len + 2 < combined_size) {
            if (current_len > 0) {
                strncat(combined, "\n", combined_size - current_len - 1);
            }
            strncat(combined, lines[i], combined_size - strlen(combined) - 1);
        } else {
            break;
        }
    }
}

int main() {
    char lines[MAX_LINES][MAX_LINE_LEN];
    char combined_text[MAX_LINES * MAX_LINE_LEN];
    struct TextStats stats;
    
    int line_count = read_input_lines(lines);
    
    if (line_count == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    build_combined_text(lines, line_count, combined_text, sizeof(combined_text));
    
    analyze_text(combined_text, &stats);
    
    printf("\nText Analysis Results:\n");
    printf("Lines: %d\n", stats.line_count);
    printf("Words: %d\n", stats.word_count);
    printf("Characters: %d\n", stats.char_count);
    printf("Longest line length: %d\n", stats.longest_line);
    
    printf("\nProcessed text:\n");
    for (int i = 0; i < line_count; i++) {
        printf("%s\n", lines[i]);
    }
    
    return 0;
}