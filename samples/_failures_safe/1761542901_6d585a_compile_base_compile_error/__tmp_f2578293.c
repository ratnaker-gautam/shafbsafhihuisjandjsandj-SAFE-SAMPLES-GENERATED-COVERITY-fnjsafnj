//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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
    
    entry->error_count = 0;
    if (strcmp(level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
    return 1;
}

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "Memory usage high",
        "Network timeout occurred",
        "File not found",
        "Permission denied",
        "Configuration updated"
    };
    
    time_t base_time = time(NULL);
    
    for (int i = 0; i < count; i++) {
        int level_idx = rand() % 3;
        int msg_idx = rand() % 8;
        
        time_t log_time = base_time - (count - i) * 60;
        struct tm* timeinfo = localtime(&log_time);
        
        snprintf(logs[i].timestamp, 32, "%04d-%02d-%02d %02d:%02d:%02d",
                 timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
                 timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        
        strncpy(logs[i].level, levels[level_idx], 15);
        logs[i].level[15] = '\0';
        
        strncpy(logs[i].message, messages[msg_idx], 255);
        logs[i].message[255] = '\0';
        
        logs[i].error_count = (strcmp(levels[level_idx], "ERROR") == 0) ? 1 : 0;
    }
}

void analyze_logs(LogEntry* logs, int count) {
    int info_count = 0;
    int warning_count = 0;
    int error_count = 0;
    int total_messages = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(logs[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
        }
        total_messages++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", total_messages);
    printf("INFO messages: %d\n", info_count);
    printf("WARNING messages: %d\n", warning_count);
    printf("ERROR messages: %d\n", error_count);
    
    if (total_messages > 0) {
        printf("Error rate: %.2f%%\n", (error_count * 100.0) / total_messages);
    }
    
    printf("\nRecent ERROR messages:\n");
    int errors_shown = 0;
    for (int i = count - 1; i >= 0 && errors_shown < 5; i--) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            printf("- %s: %s\n", logs[i].timestamp, logs[i].message);
            errors_shown++;
        }
    }
    
    if (errors_shown == 0) {
        printf("No ERROR messages found.\n");
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    LogEntry logs[MAX_ENTRIES];
    int log_count = 500 + (rand() % 500);
    
    if (log_count > MAX_ENTRIES) {
        log_count = MAX_ENTRIES;
    }
    
    printf("Generating %d sample log entries...\n", log_count);
    generate_sample_logs(logs, log_count);
    
    printf("Analyzing logs...\n\n");
    analyze_logs(logs, log_count);
    
    int valid_entries = 0;
    LogEntry test_entry;
    char test_line[] = "2024-01-15 10:30:00 INFO Test message";
    if (parse_log_line(test_line