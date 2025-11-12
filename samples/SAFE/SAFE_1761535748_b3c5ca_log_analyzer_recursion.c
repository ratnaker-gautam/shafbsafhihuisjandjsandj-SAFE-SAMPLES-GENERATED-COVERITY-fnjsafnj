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
    
    if (start == end) {
        if (entries[start].level == current_level) {
            level_counts[current_level]++;
        }
        return;
    }
    
    int mid = start + (end - start) / 2;
    
    if (entries[mid].level == current_level) {
        level_counts[current_level]++;
    }
    
    analyze_log_recursive(entries, start, mid - 1, current_level, level_counts);
    analyze_log_recursive(entries, mid + 1, end, current_level, level_counts);
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    printf("Enter log entries (format: LEVEL MESSAGE). Enter 'END' to finish.\n");
    printf("Available levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < max_entries) {
        printf("Log entry %d: ", count + 1);
        
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
    int level_counts[5] = {0};
    
    int entry_count = read_log_file(entries, 1000);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 0;
    }
    
    printf("\nAnalyzing %d log entries...\n", entry_count);
    
    for (int level = 0; level < 5; level++) {
        analyze_log_recursive(entries, 0, entry_count - 1, level, level_counts);
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int total_entries = 0;
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries\n", level_to_string(i), level_counts[i]);
        total_entries += level_counts[i];
    }
    
    printf("====================\n");
    printf("Total    : %d entries\n", total_entries);
    
    if (total_entries > 0) {
        printf("\nDistribution:\n");
        for (int i = 0; i < 5; i++) {
            float percentage = (float)level_counts[i] / total_entries * 100.0f;
            printf("%-8s: %.1f%%\n", level_to_string(i), percentage);
        }
    }
    
    return 0;
}