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
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
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
        "System started successfully",
        "User login attempted",
        "Database connection established",
        "Memory usage high",
        "Network timeout occurred",
        "File not found",
        "Configuration updated",
        "Backup completed",
        "Security alert detected",
        "Service restarted"
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
        strcpy(logs[i].level, levels[level_idx]);
        
        int msg_idx = rand() % 10;
        strcpy(logs[i].message, messages[msg_idx]);
        
        logs[i].error_count = (level_idx == 2) ? 1 : 0;
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    LogEntry logs[MAX_ENTRIES];
    int log_count = 500 + (rand() % 500);
    
    if (log_count > MAX_ENTRIES) {
        log_count = MAX_ENTRIES;
    }
    
    generate_sample_logs(logs, log_count);
    
    printf("Processing %d log entries...\n", log_count);
    
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
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", log_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    
    printf("\nFirst 5 entries (sorted by timestamp):\n");
    for (int i = 0; i < 5 && i < log_count; i++) {
        printf("%s [%s] %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    printf("\nLast 5 entries (sorted by timestamp):\n");
    for (int i = log_count - 5; i < log_count && i >= 0; i++) {
        printf("%s [%s] %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    double error_rate = (double)error_count / log_count * 100.0;
    printf("\nError rate: %.2f%%\n", error_rate);
    
    if (error_rate > 10.0) {
        printf("Status