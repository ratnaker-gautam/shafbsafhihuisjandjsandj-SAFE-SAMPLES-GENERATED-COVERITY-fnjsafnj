//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int* level_counts, int current_depth) {
    if (current_depth >= MAX_LEVELS) {
        return;
    }
    
    if (start > end) {
        return;
    }
    
    int mid = start + (end - start) / 2;
    if (mid < start || mid > end) {
        return;
    }
    
    int level = parse_log_level(entries[mid].level);
    if (level >= 0 && level < 5) {
        level_counts[level]++;
    }
    
    if (mid > start) {
        analyze_log_recursive(entries, start, mid - 1, level_counts, current_depth + 1);
    }
    
    if (mid < end) {
        analyze_log_recursive(entries, mid + 1, end, level_counts, current_depth + 1);
    }
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    printf("Enter log entries (format: LEVEL MESSAGE TIMESTAMP). Enter 'END' to finish.\n");
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < max_entries) {
        printf("Entry %d: ", count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        char level[16];
        char message[MAX_LINE_LENGTH];
        int timestamp;
        
        int parsed = sscanf(line, "%15s %1023s %d", level, message, &timestamp);
        if (parsed != 3) {
            printf("Invalid format. Use: LEVEL MESSAGE TIMESTAMP\n");
            continue;
        }
        
        if (parse_log_level(level) == -1) {
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        if (timestamp < 0) {
            printf("Timestamp must be non-negative\n");
            continue;
        }
        
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        entries[count].level[sizeof(entries[count].level) - 1] = '\0';
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        entries[count].timestamp = timestamp;
        
        count++;
    }
    
    return count;
}

int main() {
    struct LogEntry entries[100];
    int level_counts[5] = {0};
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    int entry_count = read_log_file(entries, 100);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    analyze_log_recursive(entries, 0, entry_count - 1, level_counts, 0);
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries\n", level_names[i], level_counts[i]);
    }
    
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += level_counts[i];
    }
    printf("Total   : %d entries\n", total);
    
    return 0;
}