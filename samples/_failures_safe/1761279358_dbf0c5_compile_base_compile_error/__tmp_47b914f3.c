//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return -1;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int main(void) {
    struct LogEntry logs[100];
    int log_count = 0;
    char input[512];
    int choice;
    
    printf("Log Analyzer\n");
    printf("Enter log entries in format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE\n");
    printf("Enter 'done' to finish input\n");
    
    while (log_count < 100) {
        printf("Log %d: ", log_count + 1);
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "done") == 0) break;
        
        char timestamp[20];
        char level_str[16];
        char message[256];
        
        if (sscanf(input, "%19s %15s %255[^\n]", timestamp, level_str, message) != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format. Use: YYYY-MM-DD HH:MM:SS\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        strncpy(logs[log_count].timestamp, timestamp, sizeof(logs[log_count].timestamp) - 1);
        logs[log_count].timestamp[sizeof(logs[log_count].timestamp) - 1] = '\0';
        logs[log_count].level = level;
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\0';
        
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No logs to analyze.\n");
        return 0;
    }
    
    while (1) {
        printf("\nAnalysis Options:\n");
        printf("1. Show all logs\n");
        printf("2. Filter by level\n");
        printf("3. Statistics\n");
        printf("4. Exit\n");
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        
        while (getchar() != '\n');
        
        switch (choice) {
            case 1: {
                printf("\nAll Logs:\n");
                for (int i = 0; i < log_count; i++) {
                    printf("%s [%s] %s\n", logs[i].timestamp, level_to_string(logs[i].level), logs[i].message);
                }
                break;
            }
            case 2: {
                printf("Enter level to filter (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
                char filter_level[16];
                if (fgets(filter_level, sizeof(filter_level), stdin) == NULL) break;
                filter_level[strcspn(filter_level, "\n")] = 0;
                
                int target_level = parse_log_level(filter_level);
                if (target_level == -1) {
                    printf("Invalid level.\n");
                    break;
                }
                
                printf("\nFiltered Logs (%s):\n", filter_level);
                int found = 0;
                for (int