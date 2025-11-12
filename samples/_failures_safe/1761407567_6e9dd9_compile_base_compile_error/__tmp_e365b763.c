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
    
    for (size_t i = 0; i < strlen(entry->timestamp); i++) {
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != ':' && 
            entry->timestamp[i] != '-' && entry->timestamp[i] != '.') return 0;
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

void generate_sample_logs(LogEntry* logs, int* count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System started successfully",
        "User login attempt",
        "Database connection established",
        "Memory usage high",
        "Network timeout occurred",
        "File not found",
        "Configuration loaded",
        "Backup completed"
    };
    
    *count = 5000;
    if (*count > MAX_ENTRIES) *count = MAX_ENTRIES;
    
    for (int i = 0; i < *count; i++) {
        int level_idx = rand() % 4;
        int msg_idx = rand() % 8;
        
        snprintf(logs[i].timestamp, sizeof(logs[i].timestamp), 
                "2024-%02d-%02d %02d:%02d:%02d",
                (rand() % 12) + 1, (rand() % 28) + 1,
                rand() % 24, rand() % 60, rand() % 60);
        
        strcpy(logs[i].level, levels[level_idx]);
        strcpy(logs[i].message, messages[msg_idx]);
        logs[i].error_count = (strcmp(levels[level_idx], "ERROR") == 0) ? 1 : 0;
    }
}

void analyze_logs(const LogEntry* logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    int total_messages = 0;
    
    for (int i = 0; i < count; i++) {
        if (!validate_log_entry(&logs[i])) continue;
        
        total_messages++;
        
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total valid entries: %d\n", total_messages);
    printf("INFO messages: %d (%.1f%%)\n", info_count, total_messages > 0 ? (info_count * 100.0) / total_messages : 0);
    printf("WARN messages: %d (%.1f%%)\n", warn_count, total_messages > 0 ? (warn_count * 100.0) / total_messages : 0);
    printf("ERROR messages: %d (%.1f%%)\n", error_count, total_messages > 0 ? (error_count * 100.0) / total_messages : 0);
    printf("DEBUG messages: %d (