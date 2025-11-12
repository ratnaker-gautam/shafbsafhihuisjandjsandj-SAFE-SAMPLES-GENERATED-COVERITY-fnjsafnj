//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
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

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

void process_log_entry(const char* timestamp, enum LogLevel level, const char* message) {
    if (!timestamp || !message) return;
    
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
        default:
            printf("[UNKNOWN] %s: %s\n", timestamp, message);
            break;
    }
}

int is_valid_timestamp(const char* timestamp) {
    if (!timestamp || strlen(timestamp) != 19) return 0;
    
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
    char timestamp[64];
    char level_str[32];
    char message[256];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Enter 'quit' to exit\n\n");
    
    while (1) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(timestamp, sizeof(timestamp), stdin) == NULL) break;
        
        timestamp[strcspn(timestamp, "\n")] = 0;
        
        if (strcmp(timestamp, "quit") == 0) break;
        
        if (scanf("%31s", level_str) != 1) {
            printf("Error reading log level\n");
            while (getchar() != '\n');
            continue;
        }
        
        while (getchar() != ' ');
        
        if (fgets(message, sizeof(message), stdin) == NULL) {
            printf("Error reading message\n");
            continue;
        }
        
        message[strcspn(message, "\n")] = 0;
        
        if (!is_valid_timestamp(timestamp)) {
            printf("Invalid timestamp format. Use: YYYY-MM-DD HH:MM:SS\n");
            continue;
        }
        
        enum LogLevel level = parse_log_level(level_str);
        
        if (level == LOG_UNKNOWN) {
            printf("Unknown log level: %s. Using UNKNOWN.\n", level_str);
        }
        
        process_log_entry(timestamp, level, message);
        entry_count++;
    }
    
    printf("\nProcessed %d log entries\n", entry_count);
    
    printf("\nSummary by level:\n");
    printf("INFO entries processed\n");
    printf("WARNING entries processed\n");
    printf("ERROR entries processed\n");
    printf("DEBUG entries processed\n");
    printf("UNKNOWN entries processed\n");
    
    return 0;
}