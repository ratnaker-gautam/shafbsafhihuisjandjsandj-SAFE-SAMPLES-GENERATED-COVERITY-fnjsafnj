//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    int level;
    char message[MAX_LINE_LENGTH];
};

int parse_log_level(const char* line) {
    if (strncmp(line, "ERROR", 5) == 0) return 3;
    if (strncmp(line, "WARN", 4) == 0) return 2;
    if (strncmp(line, "INFO", 4) == 0) return 1;
    if (strncmp(line, "DEBUG", 5) == 0) return 0;
    return -1;
}

void process_log_recursive(struct LogEntry* entries, int start, int end, int current_level) {
    if (start >= end || current_level >= MAX_LEVELS) return;
    
    printf("Level %d logs:\n", current_level);
    
    for (int i = start; i < end; i++) {
        if (entries[i].level == current_level) {
            printf("  %s\n", entries[i].message);
        }
    }
    
    process_log_recursive(entries, start, end, current_level + 1);
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (len >= MAX_LINE_LENGTH - 1) {
            printf("Line too long, skipping.\n");
            continue;
        }
        
        int level = parse_log_level(line);
        if (level == -1) {
            printf("Invalid log format, skipping: %s\n", line);
            continue;
        }
        
        entries[count].level = level;
        strncpy(entries[count].message, line, MAX_LINE_LENGTH - 1);
        entries[count].message[MAX_LINE_LENGTH - 1] = '\0';
        count++;
    }
    
    return count;
}

int main() {
    struct LogEntry entries[1000];
    int entry_count;
    
    entry_count = read_log_file(entries, 1000);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nAnalyzing %d log entries by severity level:\n\n", entry_count);
    process_log_recursive(entries, 0, entry_count, 0);
    
    return 0;
}