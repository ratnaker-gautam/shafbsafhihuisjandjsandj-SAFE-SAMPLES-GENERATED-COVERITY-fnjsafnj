//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define LEVEL_NAME(l) (level_names[(l)])
#define IS_VALID_TIMESTAMP(ts) (strlen(ts) == 19 && isdigit((ts)[0]))

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_log_level(const char *level_str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(level_str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int validate_timestamp(const char *timestamp) {
    if (!IS_VALID_TIMESTAMP(timestamp)) return 0;
    if (timestamp[4] != '-' || timestamp[7] != '-' || timestamp[10] != ' ' ||
        timestamp[13] != ':' || timestamp[16] != ':') return 0;
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n' || line[0] == '\0') continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[16];
        char message[MAX_LINE_LEN];
        
        if (sscanf(line, "%19s %15s %255[^\n]", timestamp, level_str, message) != 3) {
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            continue;
        }
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        count++;
    }
    
    return count;
}

void generate_summary(const LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", LEVEL_NAME(i), level_counts[i]);
    }
}

void filter_by_level(const LogEntry entries[], int count, int filter_level) {
    if (!IS_VALID_LEVEL(filter_level)) {
        printf("Invalid log level\n");
        return;
    }
    
    printf("Entries with level %s:\n", LEVEL_NAME(filter_level));
    for (int i = 0; i < count; i++) {
        if (entries[i].level == filter_level) {
            printf("%s %s %s\n", entries[i].timestamp, LEVEL_NAME(entries[i].level), entries[i].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input\n");
    
    entry_count = read_log_entries(entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    generate_summary(entries, entry_count);
    
    printf("\nEnter log level to filter (DEBUG/INFO/WARNING/ERROR): ");
    char filter_level_str[16];
    if (scanf("%15s", filter_level_str) == 1) {
        int filter_level = parse_log_level(filter_level_str);
        if (IS_VALID_LEVEL(filter_level)) {
            filter_by_level(entries, entry_count, filter_level);
        } else {
            printf("Invalid log level specified.\n");
        }
    }
    
    return 0;
}