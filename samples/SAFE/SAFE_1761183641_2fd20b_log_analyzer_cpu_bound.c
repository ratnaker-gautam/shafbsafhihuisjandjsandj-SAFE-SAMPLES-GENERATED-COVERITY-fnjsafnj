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
        "Network latency detected",
        "Critical error in module A",
        "Backup completed",
        "Security alert triggered"
    };
    
    time_t current_time = time(NULL);
    
    for (int i = 0; i < count; i++) {
        int level_idx = rand() % 3;
        int msg_idx = rand() % 8;
        
        time_t log_time = current_time - (rand() % 86400);
        struct tm* timeinfo = localtime(&log_time);
        
        snprintf(logs[i].timestamp, sizeof(logs[i].timestamp),
                "%04d-%02d-%02d %02d:%02d:%02d",
                timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
                timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        
        strcpy(logs[i].level, levels[level_idx]);
        strcpy(logs[i].message, messages[msg_idx]);
        
        logs[i].error_count = (strcmp(levels[level_idx], "ERROR") == 0) ? 1 : 0;
    }
}

void analyze_logs(LogEntry* logs, int count) {
    int total_entries = 0;
    int info_count = 0;
    int warning_count = 0;
    int error_count = 0;
    
    for (int i = 0; i < count; i++) {
        total_entries++;
        
        if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(logs[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", total_entries);
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    
    if (total_entries > 0) {
        printf("Error rate: %.2f%%\n", (error_count * 100.0) / total_entries);
    }
    
    printf("\nRecent ERROR entries:\n");
    int error_shown = 0;
    for (int i = count - 1; i >= 0 && error_shown < 5; i--) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            printf("- %s: %s\n", logs[i].timestamp, logs[i].message);
            error_shown++;
        }
    }
    
    if (error_shown == 0) {
        printf("No ERROR entries found.\n");
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
    
    printf("\nProcessing complete.\n");
    
    return 0;
}