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
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0;
}

void analyze_log_patterns(LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    int error_indices[MAX_ENTRIES];
    int error_count_total = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
            if (error_count_total < MAX_ENTRIES) {
                error_indices[error_count_total++] = i;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    if (error_count_total > 0) {
        printf("\nError messages:\n");
        for (int i = 0; i < error_count_total; i++) {
            int idx = error_indices[i];
            printf("%d. [%s] %s\n", i + 1, entries[idx].timestamp, entries[idx].message);
        }
    }
}

void generate_sample_logs(LogEntry* entries, int* count) {
    const char* levels[] = {"INFO", "WARN", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "High memory usage detected",
        "Network latency above threshold",
        "Critical system failure",
        "Backup completed",
        "Security alert triggered"
    };
    
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    
    for (int i = 0; i < 50 && *count < MAX_ENTRIES; i++) {
        LogEntry entry;
        
        int level_idx = rand() % 3;
        int msg_idx = rand() % 8;
        
        tm_info.tm_sec += rand() % 60;
        mktime(&tm_info);
        
        strftime(entry.timestamp, sizeof(entry.timestamp), 
                "%Y-%m-%d %H:%M:%S", &tm_info);
        strcpy(entry.level, levels[level_idx]);
        strcpy(entry.message, messages[msg_idx]);
        
        entries[*count] = entry;
        (*count)++;
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    srand((unsigned int)time(NULL));
    
    printf("Log Analyzer - CPU Bound Analysis\n");
    printf("Generating sample log data...\n");
    
    generate_sample_logs(entries, &entry_count);
    
    printf("Analyzing %d log entries...\n", entry_count);
    
    clock_t start = clock();
    
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < entry_count; j++) {
            char temp_line[MAX_LINE_LENGTH];
            snprintf(temp_line, sizeof(temp_line), "%s %s %s", 
                    entries[j].timestamp, entries[j].level, entries[j].message);
            
            LogEntry parsed_entry;
            if (parse_log_line(temp_line, &parsed_entry)) {
                if (!is_valid_level(parsed_entry.level)) {
                    continue;
                }
            }
        }
    }
    
    analyze_log_patterns(entries, entry_count);
    
    clock_t end = clock();
    double cpu_time = ((double)(