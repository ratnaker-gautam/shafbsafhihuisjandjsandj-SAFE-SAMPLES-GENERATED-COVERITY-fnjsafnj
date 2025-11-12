//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char line[MAX_LINE_LENGTH];
    int level;
};

int parse_log_level(const char* line) {
    if (strstr(line, "ERROR")) return 3;
    if (strstr(line, "WARN")) return 2;
    if (strstr(line, "INFO")) return 1;
    if (strstr(line, "DEBUG")) return 0;
    return -1;
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int* counts) {
    if (start > end) return;
    
    int level = entries[start].level;
    if (level >= 0 && level <= 3) {
        counts[level]++;
    }
    
    analyze_log_recursive(entries, start + 1, end, counts);
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log lines (empty line to finish):\n");
    
    while (count < max_entries) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (buffer[0] == '\0') break;
        
        if (len >= MAX_LINE_LENGTH - 1) {
            fprintf(stderr, "Line too long, skipping\n");
            continue;
        }
        
        strncpy(entries[count].line, buffer, MAX_LINE_LENGTH - 1);
        entries[count].line[MAX_LINE_LENGTH - 1] = '\0';
        entries[count].level = parse_log_level(buffer);
        count++;
    }
    
    return count;
}

int main(void) {
    struct LogEntry entries[MAX_LEVELS];
    int counts[4] = {0};
    const char* level_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    
    int entry_count = read_log_file(entries, MAX_LEVELS);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    analyze_log_recursive(entries, 0, entry_count - 1, counts);
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", level_names[i], counts[i]);
    }
    
    int total = counts[0] + counts[1] + counts[2] + counts[3];
    printf("Total entries: %d\n", total);
    
    return 0;
}