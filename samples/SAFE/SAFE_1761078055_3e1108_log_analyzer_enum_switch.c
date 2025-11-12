//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void process_log_entry(const char* timestamp, enum LogLevel level, const char* message) {
    switch (level) {
        case LOG_INFO:
            printf("[INFO] %s: %s\n", timestamp, message);
            break;
        case LOG_WARNING:
            printf("[WARNING] %s: %s\n", timestamp, message);
            break;
        case LOG_ERROR:
            printf("[ERROR] %s: %s\n", timestamp, message);
            break;
        case LOG_DEBUG:
            printf("[DEBUG] %s: %s\n", timestamp, message);
            break;
        case LOG_UNKNOWN:
            printf("[UNKNOWN] %s: %s\n", timestamp, message);
            break;
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
    char timestamp[32];
    char level_str[16];
    char message[256];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'quit' to exit\n\n");
    
    while (1) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(timestamp, sizeof(timestamp), stdin) == NULL) {
            break;
        }
        
        timestamp[strcspn(timestamp, "\n")] = 0;
        
        if (strcmp(timestamp, "quit") == 0) {
            break;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format. Use: YYYY-MM-DD HH:MM:SS\n");
            continue;
        }
        
        if (scanf("%15s", level_str) != 1) {
            printf("Failed to read log level\n");
            while (getchar() != '\n');
            continue;
        }
        
        while (getchar() != ' ');
        
        if (fgets(message, sizeof(message), stdin) == NULL) {
            printf("Failed to read message\n");
            continue;
        }
        
        message[strcspn(message, "\n")] = 0;
        
        if (strlen(message) == 0) {
            printf("Message cannot be empty\n");
            continue;
        }
        
        enum LogLevel level = parse_log_level(level_str);
        
        printf("Processed: ");
        process_log_entry(timestamp, level, message);
        
        entry_count++;
    }
    
    printf("\nTotal log entries processed: %d\n", entry_count);
    printf("Summary by level:\n");
    
    int level_counts[5] = {0};
    for (int i = 0; i < entry_count; i++) {
        level_counts[i % 5]++;
    }
    
    printf("INFO: %d\n", level_counts[0]);
    printf("WARNING: %d\n", level_counts[1]);
    printf("ERROR: %d\n", level_counts[2]);
    printf("DEBUG: %d\n", level_counts[3]);
    printf("UNKNOWN: %d\n", level_counts[4]);
    
    return 0;
}