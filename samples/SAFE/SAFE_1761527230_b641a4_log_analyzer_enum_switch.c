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
    LOG_CRITICAL,
    LOG_UNKNOWN
};

enum LogLevel parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return LOG_UNKNOWN;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
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
    char input[256];
    int log_counts[5] = {0};
    int total_logs = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Enter 'END' to finish input\n");
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) < 25) {
            printf("Invalid log entry: too short\n");
            continue;
        }
        
        char timestamp[20];
        char level_str[16];
        char message[200];
        
        if (sscanf(input, "%19s %15s %199[^\n]", timestamp, level_str, message) != 3) {
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
        
        log_counts[level]++;
        total_logs++;
        
        printf("Processed: %s %s %s\n", timestamp, level_str, message);
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total logs processed: %d\n", total_logs);
    
    for (enum LogLevel level = LOG_INFO; level <= LOG_CRITICAL; level++) {
        const char* level_name = level_to_string(level);
        int count = log_counts[level];
        double percentage = total_logs > 0 ? (count * 100.0) / total_logs : 0.0;
        printf("%-8s: %d (%.1f%%)\n", level_name, count, percentage);
    }
    
    if (total_logs > 0) {
        enum LogLevel most_common = LOG_INFO;
        for (enum LogLevel level = LOG_WARNING; level <= LOG_CRITICAL; level++) {
            if (log_counts[level] > log_counts[most_common]) {
                most_common = level;
            }
        }
        printf("Most common level: %s\n", level_to_string(most_common));
    }
    
    return 0;
}