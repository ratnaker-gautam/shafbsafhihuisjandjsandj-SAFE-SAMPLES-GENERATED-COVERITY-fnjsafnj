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
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* token = strtok(temp_line, " ");
    if (token == NULL) return 0;
    
    strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    
    strncpy(entry->level, token, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    
    strncpy(entry->message, token, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->error_count = 0;
    if (strstr(entry->level, "ERROR") != NULL) {
        entry->error_count = 1;
    }
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started", "User login", "File processed", 
        "Database connection lost", "Memory allocation failed",
        "Network timeout", "Configuration updated", "Backup completed"
    };
    
    time_t base_time = time(NULL);
    
    for (int i = 0; i < count; i++) {
        time_t log_time = base_time - (count - i) * 60;
        struct tm* timeinfo = localtime(&log_time);
        
        snprintf(logs[i].timestamp, sizeof(logs[i].timestamp),
                "%04d-%02d-%02d %02d:%02d:%02d",
                timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
                timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        
        int level_idx = rand() % 3;
        strncpy(logs[i].level, levels[level_idx], sizeof(logs[i].level) - 1);
        logs[i].level[sizeof(logs[i].level) - 1] = '\0';
        
        int msg_idx = rand() % 8;
        strncpy(logs[i].message, messages[msg_idx], sizeof(logs[i].message) - 1);
        logs[i].message[sizeof(logs[i].message) - 1] = '\0';
        
        logs[i].error_count = (level_idx == 2) ? 1 : 0;
    }
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 5000;
    
    if (log_count > MAX_ENTRIES) {
        fprintf(stderr, "Error: Too many log entries requested\n");
        return 1;
    }
    
    srand((unsigned int)time(NULL));
    generate_sample_logs(logs, log_count);
    
    qsort(logs, log_count, sizeof(LogEntry), compare_entries);
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < log_count; i++) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(logs[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", log_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    printf("Error rate: %.2f%%\n", (error_count * 100.0) / log_count);
    
    printf("\nFirst 5 log entries:\n");
    for (int i = 0; i < 5 && i < log_count; i++) {
        printf("%s [%s] %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    printf("\nLast 5 log entries:\n");