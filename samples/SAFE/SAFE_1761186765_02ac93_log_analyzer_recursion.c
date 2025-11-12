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
    if (strstr(line, "[ERROR]")) return 3;
    if (strstr(line, "[WARN]")) return 2;
    if (strstr(line, "[INFO]")) return 1;
    if (strstr(line, "[DEBUG]")) return 0;
    return -1;
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int current_level, int* counts) {
    if (start > end || current_level >= MAX_LEVELS) return;
    
    for (int i = start; i <= end; i++) {
        if (entries[i].level == current_level) {
            counts[current_level]++;
            analyze_log_recursive(entries, start, i - 1, current_level + 1, counts);
            analyze_log_recursive(entries, i + 1, end, current_level + 1, counts);
            break;
        }
    }
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL && count < max_entries) {
        if (strlen(buffer) > 0) {
            buffer[strcspn(buffer, "\n")] = '\0';
            strncpy(entries[count].line, buffer, MAX_LINE_LENGTH - 1);
            entries[count].line[MAX_LINE_LENGTH - 1] = '\0';
            entries[count].level = parse_log_level(buffer);
            count++;
        }
    }
    
    return count;
}

int main(void) {
    struct LogEntry entries[1000];
    int level_counts[MAX_LEVELS] = {0};
    int entry_count;
    
    entry_count = read_log_file(entries, 1000);
    
    if (entry_count == 0) {
        printf("No log entries found.\n");
        return 1;
    }
    
    analyze_log_recursive(entries, 0, entry_count - 1, 0, level_counts);
    
    printf("Log Level Analysis:\n");
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO: %d\n", level_counts[1]);
    printf("WARN: %d\n", level_counts[2]);
    printf("ERROR: %d\n", level_counts[3]);
    
    int total = level_counts[0] + level_counts[1] + level_counts[2] + level_counts[3];
    printf("Total entries processed: %d\n", total);
    
    return 0;
}