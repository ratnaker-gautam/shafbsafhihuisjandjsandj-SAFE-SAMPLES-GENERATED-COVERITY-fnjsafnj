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

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[16];
    
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return 0;
    }
    
    if (!validate_timestamp(entry->timestamp)) return 0;
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    entry->level = level;
    return 1;
}

void analyze_log_level(struct LogEntry* entries, int count, enum LogLevel target_level) {
    int found = 0;
    printf("Entries with level %s:\n", level_to_string(target_level));
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("%s %s %s\n", entries[i].timestamp, 
                   level_to_string(entries[i].level), entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found.\n");
    }
    printf("\n");
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char filename[256];
    
    printf("Enter log filename: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid filename.\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return 1;
    }
    
    while (entry_count < 100 && read_log_entry(file, &entries[entry_count])) {
        entry_count++;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("Log analysis complete. Found %d entries.\n\n", entry_count);
    
    enum LogLevel levels[] = {LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_CRITICAL};
    for (int i = 0; i < 5; i++) {
        analyze_log_level(entries, entry_count, levels[i]);
    }
    
    int debug_count = 0, info_count = 0, warning_count = 0, error_count = 0, critical_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        switch (entries[i].level) {
            case LOG_DEBUG: debug_count++; break;
            case LOG_INFO: info_count++; break;
            case LOG_WARNING: warning_count++; break;
            case LOG_ERROR: error_count++; break;
            case LOG_CRITICAL: critical_count++; break;
        }
    }
    
    printf("Summary:\n");
    printf("DEBUG: %d\n", debug_count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("CRITICAL: %d\n", critical_count);
    
    return 0;
}