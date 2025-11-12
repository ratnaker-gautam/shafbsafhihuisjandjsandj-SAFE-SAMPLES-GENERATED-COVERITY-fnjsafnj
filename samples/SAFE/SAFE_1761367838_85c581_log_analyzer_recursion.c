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

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

const char* level_to_string(int level) {
    switch (level) {
        case 0: return "DEBUG";
        case 1: return "INFO";
        case 2: return "WARNING";
        case 3: return "ERROR";
        case 4: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int current_level, int* level_counts) {
    if (start > end || current_level >= MAX_LEVELS) {
        return;
    }
    
    if (level_counts[current_level] < 0) {
        level_counts[current_level] = 0;
    }
    
    for (int i = start; i <= end; i++) {
        if (entries[i].level == current_level) {
            level_counts[current_level]++;
        }
    }
    
    analyze_log_recursive(entries, start, end, current_level + 1, level_counts);
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: LEVEL MESSAGE). Enter 'END' to finish:\n");
    
    while (count < max_entries) {
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
        
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        if (sscanf(line, "%31s %1023[^\n]", level_str, message) != 2) {
            printf("Invalid format. Use: LEVEL MESSAGE\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Unknown log level: %s\n", level_str);
            continue;
        }
        
        if (strlen(message) >= MAX_LINE_LENGTH) {
            printf("Message too long\n");
            continue;
        }
        
        entries[count].level = level;
        strncpy(entries[count].message, message, MAX_LINE_LENGTH - 1);
        entries[count].message[MAX_LINE_LENGTH - 1] = '\0';
        count++;
    }
    
    return count;
}

int main(void) {
    struct LogEntry entries[1000];
    int level_counts[MAX_LEVELS] = {0};
    
    for (int i = 0; i < MAX_LEVELS; i++) {
        level_counts[i] = -1;
    }
    
    int entry_count = read_log_file(entries, 1000);
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_log_recursive(entries, 0, entry_count - 1, 0, level_counts);
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int total_entries = 0;
    for (int i = 0; i < 5; i++) {
        if (level_counts[i] >= 0) {
            printf("%-10s: %d entries\n", level_to_string(i), level_counts[i]);
            total_entries += level_counts[i];
        }
    }
    
    printf("Total entries: %d\n", total_entries);
    
    if (total_entries > 0) {
        printf("\nDistribution:\n");
        for (int i = 0; i < 5; i++) {
            if (level_counts[i] >= 0) {
                double percentage = (level_counts[i] * 100.0) / total_entries;
                printf("%-10s: %.1f%%\n", level_to_string(i), percentage);
            }
        }
    }
    
    return 0;
}