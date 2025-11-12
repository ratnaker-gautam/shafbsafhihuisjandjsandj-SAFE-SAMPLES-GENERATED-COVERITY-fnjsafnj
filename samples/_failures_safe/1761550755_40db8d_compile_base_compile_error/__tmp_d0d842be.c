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
    int choice;
    
    printf("Log Analyzer\n");
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'done' on a separate line to finish input.\n");
    
    while (entry_count < 100) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        if (strncmp(line, "done", 4) == 0 && (line[4] == '\n' || line[4] == '\0')) {
            break;
        }
        
        char timestamp[20];
        char level_str[10];
        char message[256];
        
        int parsed = sscanf(line, "%19s %9s %255[^\n]", timestamp, level_str, message);
        
        if (parsed != 3) {
            printf("Invalid format. Please use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
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
        
        strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp) - 1);
        entries[entry_count].timestamp[sizeof(entries[entry_count].timestamp) - 1] = '\0';
        entries[entry_count].level = level;
        strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
        entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
        
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 0;
    }
    
    while (1) {
        printf("\nAnalysis Options:\n");
        printf("1. Show all entries\n");
        printf("2. Show entries by level\n");
        printf("3. Count entries by level\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        
        while (getchar() != '\n');
        
        switch (choice) {
            case 1: {
                printf("\nAll Log Entries:\n");
                for (int i = 0; i < entry_count; i++) {
                    printf("%s [%s] %s\n", entries[i].timestamp, 
                           level_to_string(entries[i].level), entries[i].message);
                }
                break;
            }
            case 2: {
                printf("Enter level to filter (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
                char filter_level[10];
                if (fgets(filter_level, sizeof(filter_level), stdin) == NULL) break;
                
                size_t len = strlen(filter_level);
                if (len > 0 && filter_level[len - 1] == '\n') {
                    filter_level[len - 1] = '\0';