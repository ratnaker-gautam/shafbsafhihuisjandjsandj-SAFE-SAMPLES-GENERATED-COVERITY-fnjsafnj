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

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
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

int is_valid_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
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
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    line[strcspn(line, "\n")] = 0;
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return 0;
    }
    
    if (!is_valid_timestamp(entry->timestamp)) {
        return 0;
    }
    
    entry->level = parse_log_level(level_str);
    return 1;
}

void process_log_entry(const struct LogEntry* entry) {
    switch (entry->level) {
        case LOG_INFO:
            printf("ℹ️  Info: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_WARNING:
            printf("⚠️  Warning: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_ERROR:
            printf("❌ Error: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_DEBUG:
            printf("🐛 Debug: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_UNKNOWN:
            printf("❓ Unknown: %s - %s\n", entry->timestamp, entry->message);
            break;
    }
}

int main(void) {
    struct LogEntry entry;
    int counts[5] = {0};
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = 0;
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Empty filename\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    printf("\nProcessing log entries:\n");
    printf("======================\n");
    
    while (read_log_entry(file, &entry)) {
        process_log_entry(&entry);
        if (entry.level >= 0 && entry.level < 5) {
            counts[entry.level]++;
        }
    }
    
    fclose(file);
    
    printf("\nSummary:\n");
    printf("========\n");
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", level_to_string((enum LogLevel)i), counts[i]);
    }
    
    return 0;
}