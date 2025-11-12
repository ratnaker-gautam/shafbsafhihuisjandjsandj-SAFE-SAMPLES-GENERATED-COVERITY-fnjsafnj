//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_UNKNOWN
};

enum LogLevel parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    return LOG_UNKNOWN;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

int is_valid_timestamp(const char* timestamp) {
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
    char buffer[256];
    int log_counts[5] = {0};
    char timestamps[100][20];
    char messages[100][128];
    enum LogLevel levels[100];
    int log_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' to finish input\n");
    
    while (log_count < 100) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) < 25) {
            printf("Invalid log entry: too short\n");
            continue;
        }
        
        char timestamp[20];
        char level_str[16];
        char message[128];
        
        if (sscanf(buffer, "%19s %15s %127[^\n]", timestamp, level_str, message) != 3) {
            printf("Invalid log format\n");
            continue;
        }
        
        if (!is_valid_timestamp(timestamp)) {
            printf("Invalid timestamp format\n");
            continue;
        }
        
        enum LogLevel level = parse_log_level(level_str);
        if (level == LOG_UNKNOWN) {
            printf("Unknown log level: %s\n", level_str);
            continue;
        }
        
        strncpy(timestamps[log_count], timestamp, sizeof(timestamps[0]) - 1);
        timestamps[log_count][sizeof(timestamps[0]) - 1] = '\0';
        strncpy(messages[log_count], message, sizeof(messages[0]) - 1);
        messages[log_count][sizeof(messages[0]) - 1] = '\0';
        levels[log_count] = level;
        log_counts[level]++;
        log_count++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total logs processed: %d\n", log_count);
    printf("\nBreakdown by level:\n");
    
    for (enum LogLevel level = LOG_INFO; level <= LOG_UNKNOWN; level++) {
        const char* level_name = level_to_string(level);
        printf("%-8s: %d entries\n", level_name, log_counts[level]);
    }
    
    printf("\nRecent log entries:\n");
    int start_idx = (log_count > 5) ? log_count - 5 : 0;
    for (int i = start_idx; i < log_count; i++) {
        printf("%s [%s] %s\n", timestamps[i], level_to_string(levels[i]), messages[i]);
    }
    
    enum LogLevel most_common = LOG_INFO;
    for (enum LogLevel level = LOG_WARNING; level <= LOG_UNKNOWN; level++) {
        if (log_counts[level] > log_counts[most_common]) {
            most_common = level;
        }
    }
    
    printf("\nMost common log level: %s\n", level_to_string(most_common));
    
    return 0;
}