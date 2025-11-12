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
    char line[512];
    int stats[5] = {0};
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < 100 && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        char timestamp[20];
        char level_str[16];
        char message[256];
        
        int result = sscanf(line, "%19s %15s %255[^\n]", timestamp, level_str, message);
        if (result != 3) {
            printf("Invalid format. Skipping line.\n");
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
        
        if (strlen(message) == 0) {
            printf("Empty message. Skipping line.\n");
            continue;
        }
        
        strcpy(entries[entry_count].timestamp, timestamp);
        entries[entry_count].level = level;
        strcpy(entries[entry_count].message, message);
        stats[level]++;
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("\nLevel distribution:\n");
    for (int i = 0; i < 5; i++) {
        printf("%s: %d (%.1f%%)\n", level_to_string(i), stats[i], 
               (float)stats[i] / entry_count * 100);
    }
    
    printf("\nEntries by severity level:\n");
    for (int level = 0; level < 5; level++) {
        printf("\n%s entries:\n", level_to_string(level));
        int found = 0;
        for (int i = 0; i < entry_count; i++) {
            if (entries[i].level == level) {
                printf("  %s: %s\n", entries[i].timestamp, entries[i].message);
                found = 1;
            }
        }
        if (!found) {
            printf("  (none)\n");
        }
    }
    
    enum LogLevel min_level;
    printf("\nEnter minimum level to filter (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
    if (fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = 0;
        min_level = parse_log_level(line);
        if (min_level == -1) {
            printf("Invalid level. Showing all entries.\n");
            min_level = LOG_DEBUG;
        }
    } else {
        min_level = LOG_DEBUG;
    }
    
    printf("\nFiltered entries (level >= %s):\n", level_to_string(min_level));
    int filtered_count = 0;
    for (int i = 0