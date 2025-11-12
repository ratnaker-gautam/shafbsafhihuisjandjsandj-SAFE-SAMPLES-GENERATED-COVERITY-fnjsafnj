//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
        "User login attempt",
        "Database connection established",
        "Memory usage high",
        "Disk space low",
        "Network timeout",
        "Authentication failed",
        "Critical system error"
    };
    
    time_t current_time = time(NULL);
    
    for (int i = 0; i < 100 && *count < MAX_ENTRIES; i++) {
        struct tm* time_info = localtime(&current_time);
        if (time_info == NULL) continue;
        
        char timestamp[32];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);
        
        int level_idx = rand() % 3;
        int msg_idx = rand() % 8;
        
        strcpy(entries[*count].timestamp, timestamp);
        strcpy(entries[*count].level, levels[level_idx]);
        strcpy(entries[*count].message, messages[msg_idx]);
        entries[*count].error_count = 0;
        
        (*count)++;
        current_time -= rand() % 3600;
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    srand((unsigned int)time(NULL));
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    printf("Processing %d log entries...\n", entry_count);
    
    clock_t start_time = clock();
    
    for (int i = 0; i < entry_count; i++) {
        if (is_error_level(entries[i].level)) {
            for (int j = 0; j < entry_count; j++) {
                if (strstr(entries[j].message, "error") != NULL || 
                    strstr(entries[j].message, "fail") != NULL) {
                    entries[j].error_count++;
                }
            }
        }
    }
    
    analyze_log_patterns(entries, entry_count);
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("CPU time used: %.6f seconds\n", cpu_time);
    
    int total_errors = 0;
    for (int i = 0; i < entry_count; i++) {
        total_