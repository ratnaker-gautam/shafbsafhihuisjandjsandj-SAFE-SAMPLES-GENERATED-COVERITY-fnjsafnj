//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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

int count_level_occurrences(LogEntry* entries, int count, const char* level) {
    int occurrences = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            occurrences++;
        }
    }
    return occurrences;
}

void generate_sample_logs(LogEntry* entries, int* count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System startup completed",
        "User login successful",
        "Database connection established",
        "Memory usage high",
        "Network timeout detected",
        "Backup process started",
        "Security scan completed",
        "Configuration updated"
    };
    
    time_t base_time = time(NULL);
    *count = 5000;
    
    for (int i = 0; i < *count; i++) {
        time_t log_time = base_time - (rand() % 86400);
        struct tm* tm_info = localtime(&log_time);
        strftime(entries[i].timestamp, 32, "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = rand() % 4;
        strcpy(entries[i].level, levels[level_idx]);
        
        int msg_idx = rand() % 8;
        strcpy(entries[i].message, messages[msg_idx]);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Processing synthetic log data\n");
    
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to process\n");
        return 1;
    }
    
    printf("Processed %d log entries\n", entry_count);
    
    int info_count = count_level_occurrences(entries, entry_count, "INFO");
    int warn_count = count_level_occurrences(entries, entry_count, "WARN");
    int error_count = count_level_occurrences(entries, entry_count, "ERROR");
    int debug_count = count_level_occurrences(entries, entry_count, "DEBUG");
    
    printf("Log Level Statistics:\n");
    printf("INFO:  %d entries (%.1f%%)\n", info_count, (info_count * 100.0) / entry_count);
    printf("WARN:  %d entries (%.1f%%)\n", warn_count, (warn_count * 100.0) / entry_count);
    printf("ERROR: %d entries (%.1f%%)\n", error_count, (error_count * 100.0) / entry_count);
    printf("DEBUG: %d entries (%.1f%%)\n", debug_count, (debug_count * 100.0) / entry_count);
    
    int error_entries = 0;
    printf("\nRecent ERROR entries:\n");
    for (int i = entry_count - 1; i >= 0 && error_entries < 5; i--) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            error_entries++;
        }
    }
    
    if (error_entries == 0) {
        printf("No ERROR entries found\n");
    }
    
    return 0;
}