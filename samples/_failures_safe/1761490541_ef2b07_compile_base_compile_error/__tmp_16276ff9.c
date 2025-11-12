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
    char line[400];
    int stats[5] = {0};
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < 100 && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        char timestamp[20];
        char level_str[20];
        char message[256];
        
        int parsed = sscanf(line, "%19s %19s %255[^\n]", timestamp, level_str, message);
        if (parsed != 3) {
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
        
        strncpy(entries[entry_count].timestamp, timestamp, 19);
        entries[entry_count].timestamp[19] = '\0';
        entries[entry_count].level = level;
        strncpy(entries[entry_count].message, message, 255);
        entries[entry_count].message[255] = '\0';
        
        if (level >= 0 && level < 5) {
            stats[level]++;
        }
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("\nLevel Statistics:\n");
    
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        const char* level_name = level_to_string(level);
        int count = (level >= 0 && level < 5) ? stats[level] : 0;
        float percentage = (entry_count > 0) ? (count * 100.0f) / entry_count : 0.0f;
        printf("%-8s: %d (%.1f%%)\n", level_name, count, percentage);
    }
    
    enum LogLevel filter_level;
    printf("\nEnter level to filter by (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
    char filter_input[20];
    if (fgets(filter_input, sizeof(filter_input), stdin) != NULL) {
        filter_input[strcspn(filter_input, "\n")] = '\0';
        int level = parse_log_level(filter_input);
        if (level != -1) {
            filter_level = level;
            printf("\nEntries with level %s:\n", level_to_string(filter_level));
            int found = 0;
            for (int i = 0; i < entry_count; i++) {
                if (entries[i].level == filter_level) {
                    printf("%s %s %s\n", entries[i].timestamp, 
                           level