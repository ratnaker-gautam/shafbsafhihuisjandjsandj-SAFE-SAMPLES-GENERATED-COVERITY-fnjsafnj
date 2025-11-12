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
    char line[300];
    int min_level = LOG_INFO;
    
    printf("Enter minimum log level to display (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    line[strcspn(line, "\n")] = 0;
    
    int level_input = parse_log_level(line);
    if (level_input != -1) {
        min_level = level_input;
    }
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < 100) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) >= 300) {
            printf("Line too long, skipping\n");
            continue;
        }
        
        char timestamp[20];
        char level_str[10];
        char message[256];
        
        if (sscanf(line, "%19s %9s %255[^\n]", timestamp, level_str, message) != 3) {
            printf("Invalid format, skipping\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format, skipping\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Unknown log level, skipping\n");
            continue;
        }
        
        struct LogEntry* entry = &entries[entry_count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        entry->level = level;
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        entry_count++;
    }
    
    printf("\nFiltered log entries (level >= %s):\n", level_to_string(min_level));
    printf("=========================================\n");
    
    int displayed_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level >= min_level) {
            printf("%s [%s] %s\n", entries[i].timestamp, 
                   level_to_string(entries[i].level), entries[i].message);
            displayed_count++;
        }
    }
    
    printf("\nTotal entries: %d, Displayed: %d\n", entry_count, displayed_count);
    
    int level_counts[5] = {0};
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level >= 0 && entries[i].level <= 4) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("\nLevel statistics:\n");
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", level_to_string(i), level_counts[i]);
    }
    
    return 0;
}