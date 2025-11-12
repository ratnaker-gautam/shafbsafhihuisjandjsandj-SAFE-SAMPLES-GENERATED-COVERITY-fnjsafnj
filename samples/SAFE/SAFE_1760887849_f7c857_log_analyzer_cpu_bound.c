//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* parts[3];
    char* token = strtok(temp, "|");
    int count = 0;
    
    while (token && count < 3) {
        parts[count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp)) return 0;
    if (strlen(parts[1]) >= sizeof(entry->level)) return 0;
    if (strlen(parts[2]) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, parts[0]);
    strcpy(entry->level, parts[1]);
    strcpy(entry->message, parts[2]);
    
    return 1;
}

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

void analyze_logs(LogEntry* entries, int count) {
    int level_counts[4] = {0};
    const char* level_names[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(entries[i].level, level_names[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("=====================\n");
    for (int i = 0; i < 4; i++) {
        printf("%s: %d entries\n", level_names[i], level_counts[i]);
    }
    
    int error_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
            if (error_count <= 5) {
                printf("Error %d: %s\n", error_count, entries[i].message);
            }
        }
    }
    
    if (error_count > 5) {
        printf("... and %d more errors\n", error_count - 5);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log lines (format: timestamp|level|message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count++] = entry;
            } else {
                printf("Invalid log level: %s\n", entry.level);
            }
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    clock_t start = clock();
    analyze_logs(entries, entry_count);
    clock_t end = clock();
    
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Analysis completed in %.6f seconds\n", cpu_time);
    printf("Processed %d log entries\n", entry_count);
    
    return 0;
}