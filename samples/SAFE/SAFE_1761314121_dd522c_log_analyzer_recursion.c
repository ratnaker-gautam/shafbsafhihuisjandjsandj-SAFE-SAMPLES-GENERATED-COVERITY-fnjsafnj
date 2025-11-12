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

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) == 0) continue;
        
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        if (sscanf(line, "%31s %1023[^\n]", level_str, message) == 2) {
            int level = parse_log_level(level_str);
            if (level >= 0) {
                entries[count].level = level;
                strncpy(entries[count].message, message, MAX_LINE_LENGTH - 1);
                entries[count].message[MAX_LINE_LENGTH - 1] = '\0';
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

int main(void) {
    char filename[256];
    printf("Enter log file name: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    struct LogEntry entries[1000];
    int entry_count = read_log_file(filename, entries, 1000);
    
    if (entry_count < 0) {
        printf("Error opening file: %s\n", filename);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    int level_counts[5] = {0};
    
    for (int level = 0; level < 5; level++) {
        analyze_log_recursive(entries, 0, entry_count - 1, level, level_counts);
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    
    for (int level = 0; level < 5; level++) {
        printf("%-8s: %d entries\n", level_to_string(level), level_counts[level]);
    }
    
    int max_level = 0;
    for (int level = 1; level < 5; level++) {
        if (level_counts[level] > level_counts[max_level]) {
            max_level = level;
        }
    }
    
    printf("\nMost frequent level: %s\n", level_to_string(max_level));
    
    return 0;
}