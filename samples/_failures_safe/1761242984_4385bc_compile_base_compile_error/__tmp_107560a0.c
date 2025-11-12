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
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
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
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (log_count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "END") == 0) break;
        
        if (strlen(input) < 30) {
            printf("Invalid log entry format\n");
            continue;
        }
        
        struct LogEntry entry;
        
        if (sscanf(input, "%19s %s", entry.timestamp, input + 20) != 2) {
            printf("Failed to parse timestamp and level\n");
            continue;
        }
        
        if (!validate_timestamp(entry.timestamp)) {
            printf("Invalid timestamp format\n");
            continue;
        }
        
        char level_str[20];
        char* message_start = input + 20;
        char* space_pos = strchr(message_start, ' ');
        
        if (space_pos == NULL) {
            printf("Missing log message\n");
            continue;
        }
        
        *space_pos = 0;
        strncpy(level_str, message_start, sizeof(level_str) - 1);
        level_str[sizeof(level_str) - 1] = 0;
        strncpy(entry.message, space_pos + 1, sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = 0;
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level: %s\n", level_str);
            continue;
        }
        entry.level = level;
        
        logs[log_count] = entry;
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No valid log entries provided\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("=====================\n");
    
    int level_counts[5] = {0};
    for (int i = 0; i < log_count; i++) {
        if (logs[i].level >= LOG_DEBUG && logs[i].level <= LOG_CRITICAL) {
            level_counts[logs[i].level]++;
        }
    }
    
    printf("Total entries: %d\n", log_count);
    printf("\nBreakdown by level:\n");
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        printf("%-8s: %d entries\n", level_to_string(level), level_counts[level]);
    }
    
    printf("\nFilter logs by level (enter level name or 'ALL'): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    input[strcspn(input, "\n")] = 0;
    
    printf("\nMatching log entries:\n");
    printf("=====================\n");
    
    for (int i = 0; i < log_count; i++) {
        int should_print = 0;
        
        if (strcmp(input, "ALL") == 0) {
            should_print = 1;
        } else {
            int filter_level = parse_log_level(input);
            if (filter_level != -