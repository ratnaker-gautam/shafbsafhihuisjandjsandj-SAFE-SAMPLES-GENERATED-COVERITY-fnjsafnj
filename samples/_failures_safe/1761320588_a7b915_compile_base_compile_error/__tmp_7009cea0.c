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

int validate_log_entry(const LogEntry* entry) {
    if (entry == NULL) return 0;
    
    if (strlen(entry->timestamp) == 0 || strlen(entry->level) == 0 || 
        strlen(entry->message) == 0) return 0;
    
    for (int i = 0; entry->timestamp[i]; i++) {
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != '-' && 
            entry->timestamp[i] != ':' && entry->timestamp[i] != '.') return 0;
    }
    
    const char* valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int valid = 0;
    for (int i = 0; i < 4; i++) {
        if (strcmp(entry->level, valid_levels[i]) == 0) {
            valid = 1;
            break;
        }
    }
    if (!valid) return 0;
    
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    char most_common_level[16] = "";
    int level_counts[4] = {0};
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        if (!validate_log_entry(&entries[i])) continue;
        
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
        
        for (int j = 0; j < 4; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    int max_count = 0;
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            strcpy(most_common_level, levels[i]);
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    printf("Most common level: %s\n", most_common_level);
    printf("Error percentage: %.2f%%\n", count > 0 ? (error_count * 100.0) / count : 0.0);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - CPU Bound Simulation\n");
    printf("Generating sample log data...\n");
    
    const char* sample_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* sample_messages[] = {
        "System started successfully",
        "User login attempted",
        "Database connection established",
        "Memory usage high",
        "Network timeout occurred",
        "Configuration file loaded",
        "Backup completed",
        "Security alert detected"
    };
    
    srand((unsigned int)time(NULL));
    
    for (int i = 0; i < 5000 && entry_count < MAX_ENTRIES; i++) {
        LogEntry entry;
        
        int hour = rand() % 24;
        int min = rand() % 60;
        int sec = rand() % 60;
        snprintf(entry