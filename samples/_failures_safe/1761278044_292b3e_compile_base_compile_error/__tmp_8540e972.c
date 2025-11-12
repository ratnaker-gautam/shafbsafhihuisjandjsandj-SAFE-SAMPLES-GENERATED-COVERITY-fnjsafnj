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
            printf("Informational message - normal priority\n");
            break;
        case LOG_WARNING:
            printf("Warning detected - requires attention\n");
            break;
        case LOG_ERROR:
            printf("Error occurred - immediate action needed\n");
            break;
        case LOG_CRITICAL:
            printf("CRITICAL FAILURE - SYSTEM MAY BE UNSTABLE\n");
            break;
        default:
            printf("Unknown log level\n");
            break;
    }
    
    char time_buf[64];
    struct tm time_info;
    localtime_r(&entry->timestamp, &time_info);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &time_info);
    printf("Time: %s\n", time_buf);
    
    printf("Message: %s\n", entry->message);
    printf("---\n");
}

int main(void) {
    struct LogEntry logs[10];
    int log_count = 0;
    char input[512];
    
    printf("Log Analyzer - Enter up to 10 log entries\n");
    printf("Format: TIMESTAMP LEVEL MESSAGE\n");
    printf("Example: 1609459200 ERROR Database connection failed\n");
    printf("Levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'done' to finish input\n\n");
    
    while (log_count < 10) {
        printf("Log entry %d: ", log_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "done") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        char* timestamp_str = strtok(input, " ");
        char* level_str = strtok(NULL, " ");
        char* message = strtok(NULL, "");
        
        if (timestamp_str == NULL || level_str == NULL || message == NULL) {
            printf("Invalid format. Use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        if (strlen(message) >= 256) {
            printf("Message too long (max 255 characters)\n");
            continue;
        }
        
        char* endptr;
        long timestamp = strtol(timestamp_str, &endptr, 10);
        if (*endptr != '\0' || timestamp < 0) {
            printf("Invalid timestamp\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level\n");
            continue;
        }
        
        logs[log_count].timestamp = (time_t)timestamp;
        logs[log_count].level = (enum LogLevel)level;
        strncpy(logs[log_count].message, message, 255);
        logs[log_count].message[255] = '\0';
        
        log_count++;
    }
    
    printf("\n=== LOG ANALYSIS RESULTS ===\n\n");
    
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
    
    printf("=== SUMMARY ===\n");
    printf("Total entries: %d\n", log_count);
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO: %d\n", level_counts[1]);
    printf("WARNING: %