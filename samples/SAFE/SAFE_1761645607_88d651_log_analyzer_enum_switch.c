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
    int counts[5] = {0};
    char timestamps[100][20];
    char messages[100][128];
    char levels[100][16];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' to finish input\n");
    printf("Maximum 100 entries\n");
    
    while (entry_count < 100) {
        printf("Entry %d: ", entry_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        char timestamp[20];
        char level_str[16];
        char message[128];
        
        int parsed = sscanf(input, "%19s %15s %127[^\n]", timestamp, level_str, message);
        
        if (parsed != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
            continue;
        }
        
        if (!is_valid_timestamp(timestamp)) {
            printf("Invalid timestamp format: %s\n", timestamp);
            continue;
        }
        
        enum LogLevel level = parse_log_level(level_str);
        if (level == LOG_UNKNOWN) {
            printf("Unknown log level: %s\n", level_str);
            continue;
        }
        
        strncpy(timestamps[entry_count], timestamp, sizeof(timestamps[0]) - 1);
        timestamps[entry_count][sizeof(timestamps[0]) - 1] = '\0';
        strncpy(levels[entry_count], level_str, sizeof(levels[0]) - 1);
        levels[entry_count][sizeof(levels[0]) - 1] = '\0';
        strncpy(messages[entry_count], message, sizeof(messages[0]) - 1);
        messages[entry_count][sizeof(messages[0]) - 1] = '\0';
        
        counts[level]++;
        entry_count++;
    }
    
    printf("\n=== Log Analysis Report ===\n");
    printf("Total entries: %d\n\n", entry_count);
    
    printf("Level distribution:\n");
    for (enum LogLevel level = LOG_INFO; level <= LOG_CRITICAL; level++) {
        printf("%-8s: %d entries\n", level_to_string(level), counts[level]);
    }
    
    printf("\n=== Critical and Error Entries ===\n");
    for (int i = 0; i < entry_count; i++) {
        enum LogLevel level = parse_log_level(levels[i]);
        switch (level) {
            case LOG_CRITICAL:
            case LOG_ERROR:
                printf("[%s] %s: %s\n", timestamps[i], levels[i], messages[i]);
                break;
            default:
                break;
        }
    }
    
    printf("\n=== All Entries Summary ===\n");
    for (int i = 0; i < entry_count; i++) {
        printf("%3d. [%s] %-8s: %s\n", i + 1, timestamps[i], levels[i], messages[i]);
    }
    
    return 0;
}