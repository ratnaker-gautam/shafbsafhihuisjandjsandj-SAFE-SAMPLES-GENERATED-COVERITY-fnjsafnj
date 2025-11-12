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

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int* counts, int depth) {
    if (depth >= MAX_LEVELS || start > end) {
        return;
    }
    
    if (start == end) {
        int level = parse_log_level(entries[start].level);
        if (level >= 0 && level < 5) {
            counts[level]++;
        }
        return;
    }
    
    int mid = start + (end - start) / 2;
    if (mid < start || mid > end) {
        return;
    }
    
    analyze_log_recursive(entries, start, mid, counts, depth + 1);
    analyze_log_recursive(entries, mid + 1, end, counts, depth + 1);
}

int read_log_file(const char* filename, struct LogEntry** entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    
    int capacity = 100;
    int count = 0;
    struct LogEntry* logs = malloc(capacity * sizeof(struct LogEntry));
    if (!logs) {
        fclose(file);
        return -1;
    }
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (count >= capacity) {
            capacity *= 2;
            if (capacity > 100000) {
                free(logs);
                fclose(file);
                return -1;
            }
            struct LogEntry* new_logs = realloc(logs, capacity * sizeof(struct LogEntry));
            if (!new_logs) {
                free(logs);
                fclose(file);
                return -1;
            }
            logs = new_logs;
        }
        
        char level[16];
        char message[MAX_LINE_LENGTH];
        int timestamp;
        
        if (sscanf(line, "%d %15s %1023[^\n]", &timestamp, level, message) == 3) {
            if (timestamp < 0 || timestamp > 2147483647) {
                continue;
            }
            strncpy(logs[count].level, level, sizeof(logs[count].level) - 1);
            logs[count].level[sizeof(logs[count].level) - 1] = '\0';
            strncpy(logs[count].message, message, sizeof(logs[count].message) - 1);
            logs[count].message[sizeof(logs[count].message) - 1] = '\0';
            logs[count].timestamp = timestamp;
            count++;
        }
    }
    
    fclose(file);
    *entries = logs;
    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry* entries = NULL;
    int entry_count = read_log_file(argv[1], &entries);
    
    if (entry_count <= 0) {
        printf("Error: Could not read log file or file is empty\n");
        if (entries) free(entries);
        return 1;
    }
    
    int level_counts[5] = {0};
    analyze_log_recursive(entries, 0, entry_count - 1, level_counts, 0);
    
    printf("Log Analysis Results:\n");
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO: %d\n", level_counts[1]);
    printf("WARNING: %d\n", level_counts[2]);
    printf("ERROR: %d\n", level_counts[3]);
    printf("CRITICAL: %d\n", level_counts[4]);
    
    int total = level_counts[0] + level_counts[1] + level_counts[2] + level_counts[3] + level_counts[4];
    if (total > 0) {
        printf("Total entries analyzed: %d\n", total);
    } else {
        printf("No valid log entries found\n");
    }
    
    free(entries);
    return 0;
}