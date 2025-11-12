//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int is_valid_log_level(const char* level) {
    return (strcmp(level, "INFO") == 0 || 
            strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0 ||
            strcmp(level, "DEBUG") == 0);
}

void analyze_logs(LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    int total_messages = 0;
    
    for (int i = 0; i < count; i++) {
        if (!is_valid_log_level(entries[i].level)) continue;
        
        total_messages++;
        
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total messages: %d\n", total_messages);
    printf("INFO: %d (%.1f%%)\n", info_count, total_messages > 0 ? (info_count * 100.0) / total_messages : 0.0);
    printf("WARN: %d (%.1f%%)\n", warn_count, total_messages > 0 ? (warn_count * 100.0) / total_messages : 0.0);
    printf("ERROR: %d (%.1f%%)\n", error_count, total_messages > 0 ? (error_count * 100.0) / total_messages : 0.0);
    printf("DEBUG: %d (%.1f%%)\n", debug_count, total_messages > 0 ? (debug_count * 100.0) / total_messages : 0.0);
}

void generate_sample_logs(LogEntry* entries, int* count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "User login successful",
        "Database connection established",
        "Failed to connect to service",
        "Memory usage high",
        "Configuration loaded",
        "Invalid input detected",
        "Backup completed",
        "Network timeout",
        "Cache cleared",
        "Security alert"
    };
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    for (int i = 0; i < 50; i++) {
        if (*count >= MAX_ENTRIES) break;
        
        LogEntry* entry = &entries[*count];
        
        int level_idx = rand() % 4;
        int msg_idx = rand() % 10;
        
        tm_info->tm_sec = (tm_info->tm_sec + i) % 60;
        strftime(entry->timestamp, 31, "%Y-%m-%d %H:%M:%S", tm_info);
        strncpy(entry->level, levels[level_idx], 15);
        entry->level[15] = '\0';
        strncpy(entry->message, messages[msg_idx], 255);
        entry->message[255] = '\0';
        entry->error_count = (level_idx == 2) ? 1 : 0;
        
        (*count)++;
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    printf("Processing %d log entries...\n", entry_count);
    
    clock_t start = clock();
    
    analyze_log