//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp, "|");
    int part_count = 0;
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) ||
        strlen(parts[1]) >= sizeof(entry->level) ||
        strlen(parts[2]) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "User login successful",
        "Database connection established",
        "File upload completed",
        "Memory usage high",
        "Network timeout occurred",
        "Backup process started",
        "Cache cleared",
        "Security scan finished"
    };
    
    time_t base_time = time(NULL);
    
    for (int i = 0; i < count; i++) {
        time_t log_time = base_time - (count - i) * 60;
        struct tm* tm_info = localtime(&log_time);
        
        if (tm_info != NULL) {
            strftime(logs[i].timestamp, sizeof(logs[i].timestamp), 
                    "%Y-%m-%d %H:%M:%S", tm_info);
        } else {
            snprintf(logs[i].timestamp, sizeof(logs[i].timestamp), 
                    "2024-01-01 00:00:00");
        }
        
        strncpy(logs[i].level, levels[i % 4], sizeof(logs[i].level) - 1);
        strncpy(logs[i].message, messages[i % 8], sizeof(logs[i].message) - 1);
        
        logs[i].level[sizeof(logs[i].level) - 1] = '\0';
        logs[i].message[sizeof(logs[i].message) - 1] = '\0';
    }
}

void analyze_logs(const LogEntry* logs, int count) {
    int level_counts[4] = {0};
    const char* level_names[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(logs[i].level, level_names[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("=====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < 4; i++) {
        double percentage = (count > 0) ? (level_counts[i] * 100.0) / count : 0.0;
        printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i], percentage);
    }
    
    int error_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            if (error_count < 5) {
                printf("Error example: %s - %s\n", logs[i].timestamp, logs[i].message);
            }
            error_count++;
        }
    }
    
    if (error_count > 5) {
        printf("... and %d more errors\n", error_count - 5);
    }
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 500;
    
    if (log_count > MAX_ENTRIES) {
        printf("Error: Too many log entries requested\n");
        return 1;
    }
    
    printf("Generating %d sample log entries...\n", log_count);
    generate_sample_logs(logs, log_count);
    
    printf("Processing logs...\n");
    
    LogEntry parsed_logs[MAX_ENTRIES];
    int valid_count = 0;
    
    for