//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    time_t timestamp;
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

void analyze_log_entry(const struct LogEntry* entry) {
    printf("Analyzing: [%s] ", level_to_string(entry->level));
    
    switch (entry->level) {
        case LOG_DEBUG:
            printf("Debug information - typically verbose\n");
            break;
        case LOG_INFO:
            printf("Normal operational message\n");
            break;
        case LOG_WARNING:
            printf("Warning condition - may need attention\n");
            break;
        case LOG_ERROR:
            printf("Error condition - requires investigation\n");
            break;
        case LOG_CRITICAL:
            printf("Critical condition - immediate action required\n");
            break;
        default:
            printf("Unknown log level\n");
            break;
    }
    
    printf("Message: %s\n", entry->message);
}

int main(void) {
    struct LogEntry logs[10];
    int log_count = 0;
    char input[512];
    char level_str[32];
    char message[256];
    
    printf("Log Analyzer - Enter up to 10 log entries\n");
    printf("Format: LEVEL MESSAGE (e.g., ERROR Database connection failed)\n");
    printf("Available levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'DONE' to finish input\n\n");
    
    while (log_count < 10) {
        printf("Log entry %d: ", log_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "DONE") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        int parsed = sscanf(input, "%31s %255[^\n]", level_str, message);
        if (parsed < 2) {
            printf("Invalid format. Use: LEVEL MESSAGE\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level: %s\n", level_str);
            continue;
        }
        
        if (strlen(message) >= sizeof(logs[log_count].message)) {
            printf("Message too long. Truncating.\n");
            message[sizeof(logs[log_count].message) - 1] = 0;
        }
        
        logs[log_count].timestamp = time(NULL);
        logs[log_count].level = level;
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = 0;
        
        log_count++;
    }
    
    printf("\n=== LOG ANALYSIS REPORT ===\n");
    printf("Total entries: %d\n\n", log_count);
    
    int level_counts[5] = {0};
    
    for (int i = 0; i < log_count; i++) {
        analyze_log_entry(&logs[i]);
        printf("Timestamp: %ld\n", (long)logs[i].timestamp);
        printf("---\n");
        
        if (logs[i].level >= LOG_DEBUG && logs[i].level <= LOG_CRITICAL) {
            level_counts[logs[i].level]++;
        }
    }
    
    printf("\n=== STATISTICS ===\n");
    for (enum LogLevel l = LOG_DEBUG; l <= LOG_CRITICAL; l++) {
        printf("%s: %d entries\n", level_to_string(l), level_counts[l]);
    }
    
    int error_count = level_counts[LOG_ERROR] + level_counts[LOG_CRITICAL];
    if (error_count > 0) {
        printf("\nALERT: %d error/critical entries found!\n", error_count);
    } else {
        printf("\nNo critical errors detected.\n");
    }
    
    return 0;
}