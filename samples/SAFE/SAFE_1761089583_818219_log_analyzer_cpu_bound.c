//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    int part_count = 0;
    char* token = strtok(temp, "|");
    
    while (token && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
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
            if (error_count < 5) {
                printf("Error %d: %s\n", error_count + 1, entries[i].message);
            }
            error_count++;
        }
    }
    if (error_count > 5) {
        printf("... and %d more errors\n", error_count - 5);
    }
}

void generate_sample_logs(LogEntry* entries, int* count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System startup completed",
        "User login successful",
        "Database connection established",
        "Memory usage at 75%",
        "Network latency detected",
        "File not found",
        "Permission denied",
        "Configuration loaded",
        "Backup initiated",
        "Cache cleared"
    };
    
    time_t now = time(NULL);
    *count = 0;
    
    for (int i = 0; i < 500 && *count < MAX_ENTRIES; i++) {
        LogEntry entry;
        time_t timestamp = now - (rand() % 86400);
        struct tm* tm_info = localtime(&timestamp);
        
        strftime(entry.timestamp, 31, "%Y-%m-%d %H:%M:%S", tm_info);
        strncpy(entry.level, levels[rand() % 4], 15);
        strncpy(entry.message, messages[rand() % 10], 255);
        
        entries[(*count)++] = entry;
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - CPU Bound Analysis\n");
    printf("Generating sample log data...\n");
    
    srand(42);
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    printf("Analyzing %d log entries...\n", entry_count);
    
    clock_t start = clock();
    analyze_logs(entries, entry_count);
    clock_t end = clock();
    
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nAnalysis completed in %.3f seconds\n", cpu_time);
    
    return 0;
}