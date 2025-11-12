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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* token = strtok(temp, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    return 1;
}

int count_log_level(const LogEntry* logs, int count, const char* level) {
    if (logs == NULL || level == NULL || count <= 0 || count > MAX_ENTRIES) return 0;
    
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void generate_sample_logs(LogEntry* logs, int* count) {
    if (logs == NULL || count == NULL) return;
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "User login successful",
        "Database connection established",
        "File not found",
        "Memory allocation failed",
        "Network timeout",
        "Configuration loaded",
        "Invalid input detected",
        "Backup completed",
        "Security alert triggered",
        "Service started"
    };
    
    *count = 5000;
    if (*count > MAX_ENTRIES) {
        *count = MAX_ENTRIES;
    }
    
    for (int i = 0; i < *count; i++) {
        int month = (i % 12) + 1;
        int day = (i % 28) + 1;
        int hour = i % 24;
        int minute = i % 60;
        int second = i % 60;
        
        int written = snprintf(logs[i].timestamp, sizeof(logs[i].timestamp), 
                 "2024-%02d-%02dT%02d:%02d:%02d",
                 month, day, hour, minute, second);
        if (written < 0 || written >= (int)sizeof(logs[i].timestamp)) {
            strcpy(logs[i].timestamp, "2024-01-01T00:00:00");
        }
        
        const char* level = levels[i % 4];
        if (strlen(level) < sizeof(logs[i].level)) {
            strcpy(logs[i].level, level);
        } else {
            strcpy(logs[i].level, "UNKNOWN");
        }
        
        const char* message = messages[i % 10];
        if (strlen(message) < sizeof(logs[i].message)) {
            strcpy(logs[i].message, message);
        } else {
            strcpy(logs[i].message, "Unknown message");
        }
    }
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    printf("Generating sample log data...\n");
    generate_sample_logs(logs, &log_count);
    
    if (log_count == 0) {
        printf("No logs generated.\n");
        return 1;
    }
    
    printf("Analyzing %d log entries...\n", log_count);
    
    int info_count = count_log_level(logs, log_count, "INFO");
    int warn_count = count_log_level(logs, log_count, "WARN");
    int error_count = count_log_level(logs, log_count, "ERROR");
    int debug_count = count_log_level(logs, log_count, "DEBUG");
    
    printf("Log Level Summary:\n");
    printf("INFO:  %d entries\n", info_count);
    printf("WARN:  %d entries\n", warn_count);
    printf("ERROR: %d entries\n", error_count);
    printf("DEBUG: %d entries\n", debug_count);
    
    int total_counted = info_count + warn_count + error_count + debug_count;
    if (total_counted != log_count) {
        printf("Warning: Level counts don't match total entries.\n");
    }
    
    printf("Analysis complete.\n");
    return 0;
}