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
        
        if (sscanf(input, "%19s %255s", entry.timestamp, input + 20) != 2) {
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
        
        size_t level_len = space_pos - message_start;
        if (level_len >= sizeof(level_str)) {
            printf("Log level too long\n");
            continue;
        }
        
        strncpy(level_str, message_start, level_len);
        level_str[level_len] = '\0';
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level: %s\n", level_str);
            continue;
        }
        entry.level = level;
        
        char* message = space_pos + 1;
        if (strlen(message) == 0) {
            printf("Empty log message\n");
            continue;
        }
        
        if (strlen(message) >= sizeof(entry.message)) {
            printf("Log message too long\n");
            continue;
        }
        strcpy(entry.message, message);
        
        logs[log_count] = entry;
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No valid log entries provided\n");
        return 1;
    }
    
    int level_counts[5] = {0};
    for (int i = 0; i < log_count; i++) {
        switch (logs[i].level) {
            case LOG_DEBUG: level_counts[0]++; break;
            case LOG_INFO: level_counts[1]++; break;
            case LOG_WARNING: level_counts[2]++; break;
            case LOG_ERROR: level_counts[3]++; break;
            case LOG_CRITICAL: level_counts[4]++; break;
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", log_count);
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO: %d\n", level_counts[1]);
    printf("WARNING: %d\n", level_counts[2]);
    printf("ERROR: %d\n", level_counts[3]);
    printf("CRITICAL: %d\n", level_counts[4]);
    
    enum LogLevel filter_level;
    printf("\nEnter log level to filter (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
    char filter_input[20];