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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->error_count = 0;
    
    return 1;
}

int is_error_level(const char* level) {
    if (level == NULL) return 0;
    return strcmp(level, "ERROR") == 0 || strcmp(level, "FATAL") == 0;
}

void analyze_logs(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    printf("Error percentage: %.2f%%\n", count > 0 ? (error_count * 100.0) / count : 0.0);
}

void generate_sample_logs(LogEntry* entries, int* count) {
    if (entries == NULL || count == NULL) return;
    
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login attempt",
        "Database connection established",
        "Memory usage high",
        "Disk space running low",
        "Authentication failed",
        "Network timeout",
        "Service unavailable"
    };
    
    int num_levels = sizeof(levels) / sizeof(levels[0]);
    int num_messages = sizeof(messages) / sizeof(messages[0]);
    
    *count = 0;
    for (int i = 0; i < 500 && *count < MAX_ENTRIES; i++) {
        int level_idx = rand() % num_levels;
        int msg_idx = rand() % num_messages;
        
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        
        snprintf(entries[*count].timestamp, sizeof(entries[*count].timestamp),
                "%04d-%02d-%02d %02d:%02d:%02d",
                tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
                tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
        
        strncpy(entries[*count].level, levels[level_idx], sizeof(entries[*count].level) - 1);
        entries[*count].level[sizeof(entries[*count].level) - 1] = '\0';
        
        strncpy(entries[*count].message, messages[msg_idx], sizeof(entries[*count].message) - 1);
        entries[*count].message[sizeof(entries[*count].message) - 1] = '\0';
        
        entries[*count].error_count = 0;
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
        printf("No log entries generated.\n");
        return 1;
    }
    
    printf("Processing %d log entries...\n", entry_count);
    
    clock_t start_time = clock();
    
    analyze_logs(entries, entry_count);
    
    int error_entries = 0;
    for (int i = 0; i < entry_count; i++) {
        if (is_error_level(entries[i].level)) {
            error_entries