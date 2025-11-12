//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
    char level[16];
    char message[256];
};

int parse_timestamp(const char* ts_str, struct tm* tm) {
    return strptime(ts_str, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int validate_log_level(const char* level) {
    const char* valid_levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int i = 0; i < 5; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE). Enter 'END' to finish.\n");
    printf("Example: 2024-01-15 10:30:00 INFO User logged in\n");
    
    while (log_count < MAX_ENTRIES) {
        printf("Log %d: ", log_count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        char timestamp[64], level[16], message[256];
        int parsed = sscanf(line, "%63s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed != 3) {
            printf("Invalid format. Use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        struct tm time_struct = {0};
        if (!parse_timestamp(timestamp, &time_struct)) {
            printf("Invalid timestamp format. Use: YYYY-MM-DD HH:MM:SS\n");
            continue;
        }
        
        if (!validate_log_level(level)) {
            printf("Invalid log level. Valid: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Message cannot be empty\n");
            continue;
        }
        
        strncpy(logs[log_count].timestamp, timestamp, sizeof(logs[log_count].timestamp) - 1);
        strncpy(logs[log_count].level, level, sizeof(logs[log_count].level) - 1);
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        
        logs[log_count].timestamp[sizeof(logs[log_count].timestamp) - 1] = '\0';
        logs[log_count].level[sizeof(logs[log_count].level) - 1] = '\0';
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\0';
        
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No logs to analyze.\n");
        return 0;
    }
    
    printf("\n=== LOG ANALYSIS REPORT ===\n");
    printf("Total entries: %d\n", log_count);
    
    int level_counts[5] = {0};
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < log_count; i++) {
        for (int j = 0; j < 5; j++) {
            if (strcmp(logs[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("\nLevel distribution:\n");
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d (%.1f%%)\n", levels[i], level_counts[i], 
               (float)level_counts[i] / log_count * 100);
    }
    
    printf("\nRecent entries (last 5):\n");
    int start = (log_count > 5) ? log_count - 5 : 0;
    for (int i = start; i < log_count; i++) {
        printf("[%s] %s: %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    int error_critical_count = level_counts[3] + level_counts[4];
    if (error_critical_count > 0) {
        printf("\nERROR/CRITICAL entries found:\n");
        for (int i = 0; i < log_count; i++) {
            if (strcmp(logs[i].level, "ERROR") == 0 || strcmp(logs[i].level, "CRITICAL") == 0) {
                printf("[%s] %s: %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
            }
        }
    }
    
    return 0;
}