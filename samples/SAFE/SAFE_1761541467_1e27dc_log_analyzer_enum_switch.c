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
    int debug_count = 0, info_count = 0, warning_count = 0, error_count = 0, critical_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < 100 && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        char timestamp[20], level_str[10], message[256];
        int parsed = sscanf(line, "%19s %9s %255[^\n]", timestamp, level_str, message);
        
        if (parsed != 3) {
            printf("Invalid log format. Skipping line.\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format. Skipping line.\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Skipping line.\n");
            continue;
        }
        
        strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp) - 1);
        entries[entry_count].timestamp[sizeof(entries[entry_count].timestamp) - 1] = '\0';
        entries[entry_count].level = level;
        strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
        entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
        entry_count++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    
    for (int i = 0; i < entry_count; i++) {
        switch (entries[i].level) {
            case LOG_DEBUG: debug_count++; break;
            case LOG_INFO: info_count++; break;
            case LOG_WARNING: warning_count++; break;
            case LOG_ERROR: error_count++; break;
            case LOG_CRITICAL: critical_count++; break;
        }
    }
    
    printf("DEBUG: %d\n", debug_count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("CRITICAL: %d\n", critical_count);
    
    printf("\nEntries by severity level:\n");
    
    enum LogLevel current_level = LOG_DEBUG;
    while (current_level <= LOG_CRITICAL) {
        printf("\n%s entries:\n", level_to_string(current_level));
        int found = 0;
        
        for (int i = 0; i < entry_count; i++) {
            if (entries[i].level == current_level) {
                printf("  %s: %s\n", entries[i].timestamp, entries[i].message);
                found = 1;
            }
        }
        
        if (!found) {
            printf("  No entries found.\n");
        }
        
        current_level++;
    }
    
    return 0;
}