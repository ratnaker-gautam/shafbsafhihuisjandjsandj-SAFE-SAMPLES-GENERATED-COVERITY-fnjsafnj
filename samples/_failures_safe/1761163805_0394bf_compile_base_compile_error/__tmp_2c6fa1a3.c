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
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp_line, "|");
    int part_count = 0;
    
    while (token && part_count < 3) {
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
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->error_count = 0;
    if (strstr(entry->message, "error") || strstr(entry->level, "ERROR")) {
        entry->error_count = 1;
    }
    
    return 1;
}

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System started successfully",
        "User login attempted",
        "Database connection established",
        "File not found",
        "Memory allocation failed",
        "Network timeout occurred",
        "Configuration loaded",
        "Backup completed",
        "Security violation detected",
        "Service unavailable"
    };
    
    time_t base_time = time(NULL);
    
    for (int i = 0; i < count; i++) {
        int level_idx = rand() % 4;
        int msg_idx = rand() % 10;
        
        time_t log_time = base_time - (count - i) * 60;
        struct tm* tm_info = localtime(&log_time);
        
        snprintf(logs[i].timestamp, sizeof(logs[i].timestamp),
                "%04d-%02d-%02d %02d:%02d:%02d",
                tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
                tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
        
        strncpy(logs[i].level, levels[level_idx], sizeof(logs[i].level) - 1);
        logs[i].level[sizeof(logs[i].level) - 1] = '\0';
        
        strncpy(logs[i].message, messages[msg_idx], sizeof(logs[i].message) - 1);
        logs[i].message[sizeof(logs[i].message) - 1] = '\0';
        
        logs[i].error_count = (level_idx == 2) ? 1 : 0;
    }
}

void analyze_logs(LogEntry* logs, int count) {
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    int total_errors = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
        
        total_errors += logs[i].error_count;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d (%.1f%%)\n", info_count, (float)info_count / count * 100);
    printf("WARNING: %d (%.1f%%)\n", warning_count, (float)warning_count / count * 100);
    printf("ERROR: %d (%.1f%%)\n", error_count, (float)error_count / count * 100);
    printf("DEBUG: %d (%.1f%%)\n", debug_count, (float)debug_count / count * 100);
    printf("Total error occurrences: %d\n", total_errors);
    
    printf("\nRecent ERROR entries:\n");
    int printed = 0;
    for (int i = count -