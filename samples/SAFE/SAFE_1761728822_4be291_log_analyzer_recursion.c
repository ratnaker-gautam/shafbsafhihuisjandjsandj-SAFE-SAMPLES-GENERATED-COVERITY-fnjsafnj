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
            int next_start = i + 1;
            int next_end = end;
            for (int j = i + 1; j <= end; j++) {
                if (entries[j].level <= current_level) {
                    next_end = j - 1;
                    break;
                }
            }
            if (next_start <= next_end) {
                analyze_log_recursive(entries, next_start, next_end, current_level + 1, counts);
            }
            i = next_end;
        }
    }
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
            printf("Line too long, skipping.\n");
            continue;
        }
        
        strncpy(entries[count].line, buffer, MAX_LINE_LENGTH - 1);
        entries[count].line[MAX_LINE_LENGTH - 1] = '\0';
        entries[count].level = parse_log_level(buffer);
        count++;
    }
    
    return count;
}

int main() {
    struct LogEntry entries[1000];
    int level_counts[MAX_LEVELS] = {0};
    int total_entries;
    
    total_entries = read_log_file(entries, 1000);
    
    if (total_entries == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    analyze_log_recursive(entries, 0, total_entries - 1, 0, level_counts);
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", total_entries);
    printf("DEBUG level: %d\n", level_counts[0]);
    printf("INFO level: %d\n", level_counts[1]);
    printf("WARN level: %d\n", level_counts[2]);
    printf("ERROR level: %d\n", level_counts[3]);
    
    int unknown = total_entries;
    for (int i = 0; i < 4; i++) {
        unknown -= level_counts[i];
    }
    printf("Unknown level: %d\n", unknown);
    
    return 0;
}