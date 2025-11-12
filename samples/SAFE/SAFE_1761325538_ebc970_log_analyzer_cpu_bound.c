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
    return (strcmp(level, "ERROR") == 0 || strcmp(level, "FATAL") == 0);
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
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nError entries found:\n");
        for (int i = 0; i < count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
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
        "Database connection failed",
        "Critical system error",
        "Backup completed",
        "Security alert triggered"
    };
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    for (int i = 0; i < 50 && *count < MAX_ENTRIES; i++) {
        char timestamp[32];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = rand() % 3;
        int msg_idx = rand() % 9;
        
        strcpy(entries[*count].timestamp, timestamp);
        strcpy(entries[*count].level, levels[level_idx]);
        strcpy(entries[*count].message, messages[msg_idx]);
        entries[*count].error_count = 0;
        
        (*count)++;
        
        tm_info->tm_sec += (rand() % 60) + 1;
        mktime(tm_info);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    srand((unsigned int)time(NULL));
    
    printf("Log Analyzer - CPU Bound Analysis\n");
    printf("Generating sample log data...\n");
    
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    printf("Analyzing %d log entries...\n", entry_count);
    
    clock_t start = clock();
    
    for (int i = 0; i < 1000; i++) {
        analyze_log_patterns(entries, entry_count);
    }
    
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("\nCPU-bound analysis completed.\n");
    printf("Processing time: %.3f seconds\n", cpu_time);
    
    return 0;
}