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
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* token = strtok(temp_line, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    return 1;
}

int count_log_level(const LogEntry* logs, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void generate_sample_logs(LogEntry* logs, int* count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "User login successful",
        "Database connection established",
        "File not found",
        "Memory allocation failed",
        "Network timeout",
        "Configuration loaded",
        "Invalid input detected",
        "Backup completed"
    };
    
    *count = 5000;
    if (*count > MAX_ENTRIES) *count = MAX_ENTRIES;
    
    for (int i = 0; i < *count; i++) {
        snprintf(logs[i].timestamp, sizeof(logs[i].timestamp), 
                 "2024-%02d-%02dT%02d:%02d:%02d",
                 (i % 12) + 1, (i % 28) + 1, 
                 i % 24, (i * 2) % 60, (i * 3) % 60);
        
        int level_idx = i % 4;
        strcpy(logs[i].level, levels[level_idx]);
        
        int msg_idx = i % 8;
        strcpy(logs[i].message, messages[msg_idx]);
    }
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    generate_sample_logs(logs, &log_count);
    
    if (log_count == 0) {
        printf("No logs to analyze\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total log entries: %d\n", log_count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int count = count_log_level(logs, log_count, levels[i]);
        printf("%s: %d (%.1f%%)\n", levels[i], count, 
               (count * 100.0) / log_count);
    }
    
    int error_count = count_log_level(logs, log_count, "ERROR");
    if (error_count > 0) {
        printf("\nError messages found:\n");
        for (int i = 0; i < log_count; i++) {
            if (strcmp(logs[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", logs[i].timestamp, logs[i].message);
            }
        }
    }
    
    return 0;
}