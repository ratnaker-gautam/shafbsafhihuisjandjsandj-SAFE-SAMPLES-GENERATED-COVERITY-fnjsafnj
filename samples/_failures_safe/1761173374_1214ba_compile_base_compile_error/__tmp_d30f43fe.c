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

int parse_timestamp(const char* timestamp) {
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
    
    if (!parse_timestamp(entry->timestamp)) return 0;
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    entry->level = level;
    return 1;
}

void analyze_log_level(struct LogEntry* entries, int count) {
    int level_counts[5] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= LOG_DEBUG && entries[i].level <= LOG_CRITICAL) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Level Analysis:\n");
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        printf("%-8s: %d entries\n", level_to_string(level), level_counts[level]);
    }
}

void filter_by_level(struct LogEntry* entries, int count, enum LogLevel min_level) {
    printf("Entries with level %s and above:\n", level_to_string(min_level));
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= min_level) {
            printf("%s %-8s %s\n", 
                   entries[i].timestamp, 
                   level_to_string(entries[i].level), 
                   entries[i].message);
        }
    }
}

int main(void) {
    struct LogEntry entries[1000];
    int entry_count = 0;
    FILE* file;
    char filename[256];
    
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid filename\n");
        return 1;
    }
    
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return 1;
    }
    
    while (entry_count < 1000 && read_log_entry(file, &entries[entry_count])) {
        entry_count++;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    printf("Successfully read %d log entries\n\n", entry_count);
    
    int choice;
    enum LogLevel filter_level;
    
    while (1) {
        printf("1. Show level analysis\n");
        printf("2. Filter by minimum level\n");
        printf("3. Exit\n");
        printf("Choose option: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                analyze_log_level(entries, entry_count);
                break;
            case 2:
                printf("Enter minimum level (0=DEBUG,1=INFO,2=WARNING,3=ERROR,4=CRITICAL): ");
                if (scanf("%d", (int*)&filter_level) != 1 || 
                    filter_level < LOG_DEBUG || filter_level