//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int is_valid_level(const char* level) {
    return (strcmp(level, "INFO") == 0 || 
            strcmp(level, "WARNING") == 0 || 
            strcmp(level, "ERROR") == 0 ||
            strcmp(level, "DEBUG") == 0);
}

void analyze_logs(LogEntry* entries, int count) {
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    char most_common_level[16] = "INFO";
    int max_count = 0;
    int level_counts[4] = {0};
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
            level_counts[0]++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
            level_counts[1]++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
            level_counts[2]++;
        } else if (strcmp(entries[i].level, "DEBUG") == 0) {
            debug_count++;
            level_counts[3]++;
        }
    }
    
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            strcpy(most_common_level, levels[i]);
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    printf("Most common level: %s (%d occurrences)\n", most_common_level, max_count);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (one per line, format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
            } else {
                printf("Invalid log level in line: %s\n", line);
            }
        } else {
            printf("Failed to parse line: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    clock_t start = clock();
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = 0; j < 1000; j++) {
            entries[i].error_count = (strcmp(entries[i].level, "ERROR") == 0) ? 1 : 0;
        }
    }
    
    analyze_logs(entries, entry_count);
    
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU time used: %.6f seconds\n", cpu_time);
    
    return 0;
}