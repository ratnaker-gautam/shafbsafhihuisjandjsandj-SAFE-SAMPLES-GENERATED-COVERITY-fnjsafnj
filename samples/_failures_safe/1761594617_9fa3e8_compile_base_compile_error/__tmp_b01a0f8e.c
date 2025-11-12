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

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System started", "User login", "Database connection failed",
        "Memory allocation error", "Network timeout", "File not found",
        "Configuration loaded", "Backup completed", "Security violation",
        "Performance degradation"
    };
    
    for (int i = 0; i < count; i++) {
        snprintf(logs[i].timestamp, sizeof(logs[i].timestamp), 
                "2024-%02d-%02d %02d:%02d:%02d",
                (i % 12) + 1, (i % 28) + 1, 
                i % 24, i % 60, i % 60);
        
        int level_idx = rand() % 4;
        strcpy(logs[i].level, levels[level_idx]);
        
        int msg_idx = rand() % 10;
        strcpy(logs[i].message, messages[msg_idx]);
        
        logs[i].error_count = is_error_level(logs[i].level) ? 1 : 0;
    }
}

void analyze_logs(LogEntry* logs, int count) {
    int total_errors = 0;
    int total_warnings = 0;
    int total_info = 0;
    int total_debug = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "ERROR") == 0) total_errors++;
        else if (strcmp(logs[i].level, "WARN") == 0) total_warnings++;
        else if (strcmp(logs[i].level, "INFO") == 0) total_info++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) total_debug++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("INFO messages: %d\n", total_info);
    printf("WARN messages: %d\n", total_warnings);
    printf("ERROR messages: %d\n", total_errors);
    printf("DEBUG messages: %d\n", total_debug);
    printf("Error rate: %.2f%%\n", (total_errors * 100.0) / count);
    
    printf("\nRecent ERROR messages:\n");
    int error_shown = 0;
    for (int i = count - 1; i >= 0 && error_shown < 5; i--) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            printf("- %s: %s\n", logs[i].timestamp, logs[i].message);
            error_shown++;
        }
    }
    if (error_shown == 0) {
        printf("No ERROR messages found\n");
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    LogEntry logs[MAX_ENTRIES];
    int log_count = 5000;
    
    if (log_count > MAX_ENTRIES) {
        printf("Error: Too many log entries requested\n");
        return 1;
    }
    
    printf("Generating %d sample log entries...\n", log_count);
    generate_sample_logs(logs, log_count);
    
    printf("Analyzing logs...\n\n");
    analyze_logs(logs, log_count);
    
    printf("\nPerforming CPU-intensive pattern analysis...\n");
    
    clock_t start = clock();
    
    int pattern_matches = 0;
    for (int i = 0; i < log_count; i++) {
        for (int j = 0; logs[i].message[j] != '\0'; j++) {
            if (strstr