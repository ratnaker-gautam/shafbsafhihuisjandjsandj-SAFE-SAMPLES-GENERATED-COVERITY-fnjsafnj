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
    int level_counts[5] = {0};
    char input[512];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (log_count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "END") == 0) break;
        
        char timestamp[20];
        char level_str[20];
        char message[256];
        
        int parsed = sscanf(input, "%19s %19s %255[^\n]", timestamp, level_str, message);
        
        if (parsed != 3) {
            printf("Invalid format. Skipping entry.\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format. Skipping entry.\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Skipping entry.\n");
            continue;
        }
        
        strncpy(logs[log_count].timestamp, timestamp, sizeof(logs[log_count].timestamp) - 1);
        logs[log_count].timestamp[sizeof(logs[log_count].timestamp) - 1] = '\0';
        logs[log_count].level = level;
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\0';
        
        level_counts[level]++;
        log_count++;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", log_count);
    
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", level_to_string(i), level_counts[i]);
    }
    
    printf("\nFilter by level (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    
    input[strcspn(input, "\n")] = 0;
    int filter_level = parse_log_level(input);
    
    if (filter_level != -1) {
        printf("\nEntries with level %s:\n", level_to_string(filter_level));
        int found = 0;
        for (int i = 0; i < log_count; i++) {
            if (logs[i].level == filter_level) {
                printf("%s [%s] %s\n", logs[i].timestamp, level_to_string(logs[i].level), logs[i].message);
                found++;
            }
        }
        if (found == 0) {
            printf("No entries found.\n");
        }
    } else {
        printf("Invalid level specified.\n");
    }
    
    return 0;
}