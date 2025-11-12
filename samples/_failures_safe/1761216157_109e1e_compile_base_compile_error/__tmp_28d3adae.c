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

void analyze_log_patterns(LogEntry* entries, int count) {
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
    
    if (count > 0) {
        printf("Error rate: %.2f%%\n", (error_count * 100.0) / count);
    }
}

void generate_sample_logs(LogEntry* entries, int* count) {
    if (entries == NULL || count == NULL) return;
    
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "High memory usage detected",
        "Network latency increased",
        "Authentication failed",
        "Disk space running low",
        "Backup completed",
        "Security violation detected",
        "Service unavailable"
    };
    
    int num_levels = sizeof(levels) / sizeof(levels[0]);
    int num_messages = sizeof(messages) / sizeof(messages[0]);
    
    *count = 0;
    for (int i = 0; i < 50 && *count < MAX_ENTRIES; i++) {
        int level_idx = rand() % num_levels;
        int msg_idx = rand() % num_messages;
        
        snprintf(entries[*count].timestamp, sizeof(entries[*count].timestamp),
                "2024-%02d-%02dT%02d:%02d:%02d",
                (rand() % 12) + 1, (rand() % 28) + 1,
                rand() % 24, rand() % 60, rand() % 60);
        
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
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    printf("Processing %d log entries...\n", entry_count);
    
    int parsed_count = 0;
    for (int i = 0; i < entry_count; i++) {
        char line[MAX_LINE_LENGTH];
        snprintf(line, sizeof(line), "%s %s %s", 
                entries[i].timestamp, entries[i].level, entries[i].message);
        
        LogEntry parsed