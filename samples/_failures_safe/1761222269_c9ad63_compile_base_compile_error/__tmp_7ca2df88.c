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
    struct LogEntry entries[100];
    int entry_count = 0;
    int level_counts[5] = {0};
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' to finish input\n");
    
    char input[512];
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
        char level_str[20];
        char message[256];
        
        int parsed = sscanf(input, "%19s %19s %255[^\n]", timestamp, level_str, message);
        
        if (parsed != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format. Use: YYYY-MM-DD HH:MM:SS\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Message cannot be empty\n");
            continue;
        }
        
        strncpy(entries[entry_count].timestamp, timestamp, 19);
        entries[entry_count].timestamp[19] = '\0';
        entries[entry_count].level = level;
        strncpy(entries[entry_count].message, message, 255);
        entries[entry_count].message[255] = '\0';
        
        level_counts[level]++;
        entry_count++;
    }
    
    printf("\n=== Log Analysis Report ===\n");
    printf("Total entries: %d\n", entry_count);
    printf("\nLevel distribution:\n");
    
    for (int i = 0; i < 5; i++) {
        const char* level_name = level_to_string(i);
        int count = level_counts[i];
        float percentage = entry_count > 0 ? (count * 100.0f) / entry_count : 0.0f;
        printf("%-8s: %d (%.1f%%)\n", level_name, count, percentage);
    }
    
    printf("\nRecent entries:\n");
    int display_count = entry_count < 5 ? entry_count : 5;
    for (int i = entry_count - display_count; i < entry_count; i++) {
        const char* level_name = level_to_string(entries[i].level);
        printf("[%s] %s: %s\n", entries[i].timestamp, level_name, entries[i].message);
    }
    
    enum LogLevel most_common = LOG_INFO;
    int max_count = 0;
    for (int i = 0; i < 5; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            most_common = i;
        }
    }
    
    printf("\nMost common log level: %s