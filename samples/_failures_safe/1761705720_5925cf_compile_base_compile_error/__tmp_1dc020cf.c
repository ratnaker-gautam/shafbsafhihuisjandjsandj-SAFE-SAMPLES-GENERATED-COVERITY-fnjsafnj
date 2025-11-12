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
            printf("Debug message - low priority\n");
            break;
        case LOG_INFO:
            printf("Informational - normal operation\n");
            break;
        case LOG_WARNING:
            printf("Warning detected - potential issue\n");
            break;
        case LOG_ERROR:
            printf("Error occurred - requires attention\n");
            break;
        case LOG_CRITICAL:
            printf("CRITICAL ALERT - immediate action required\n");
            break;
        default:
            printf("Unknown log level\n");
            break;
    }
    
    char time_buf[64];
    struct tm time_info;
    time_t timestamp = entry->timestamp;
    if (localtime_r(&timestamp, &time_info) != NULL) {
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &time_info);
        printf("Time: %s | Message: %s\n", time_buf, entry->message);
    } else {
        printf("Time: Invalid | Message: %s\n", entry->message);
    }
}

int main(void) {
    struct LogEntry logs[10];
    int log_count = 0;
    char input[512];
    
    printf("Log Analyzer - Enter up to 10 log entries (format: LEVEL MESSAGE)\n");
    printf("Available levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'DONE' to finish input\n\n");
    
    while (log_count < 10) {
        printf("Log entry %d: ", log_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "DONE") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        char level_str[32];
        char message[256];
        
        if (sscanf(input, "%31s %255[^\n]", level_str, message) != 2) {
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
            message[sizeof(logs[log_count].message) - 1] = '\0';
        }
        
        logs[log_count].timestamp = time(NULL);
        logs[log_count].level = level;
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\0';
        
        log_count++;
    }
    
    printf("\n=== LOG ANALYSIS REPORT ===\n");
    printf("Total entries: %d\n\n", log_count);
    
    int level_counts[5] = {0};
    
    for (int i = 0; i < log_count; i++) {
        analyze_log_entry(&logs[i]);
        
        switch (logs[i].level) {
            case LOG_DEBUG: level_counts[0]++; break;
            case LOG_INFO: level_counts[1]++; break;
            case LOG_WARNING: level_counts[2]++; break;
            case LOG_ERROR: level_counts[3]++; break;
            case LOG_CRITICAL: level_counts[4]++; break;
            default: break;
        }
    }
    
    printf("\n=== SUMMARY BY LEVEL ===\n");
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO: %d\n", level_counts[1]);
    printf("WARNING: %d\n", level_counts[2]);
    printf("ERROR: %d\n", level_counts[3]);