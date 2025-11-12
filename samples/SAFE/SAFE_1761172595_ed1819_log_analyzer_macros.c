//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define LEVEL_NAME(l) (level_names[(l)])
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ' || (c) == '.')

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_log_level(const char *str) {
    if (!str) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) return i;
    }
    return -1;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (!str || !timestamp) return 0;
    int len = 0;
    while (str[len] && IS_TIMESTAMP_CHAR(str[len]) && len < TIMESTAMP_LEN - 1) {
        timestamp[len] = str[len];
        len++;
    }
    timestamp[len] = '\0';
    return len > 0;
}

int read_log_entry(FILE *file, LogEntry *entry) {
    if (!file || !entry) return 0;
    char buffer[MAX_LINE_LEN];
    if (!fgets(buffer, sizeof(buffer), file)) return 0;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
    
    char *timestamp_end = strchr(buffer, ' ');
    if (!timestamp_end) return 0;
    
    size_t timestamp_len = timestamp_end - buffer;
    if (timestamp_len >= TIMESTAMP_LEN) return 0;
    
    memcpy(entry->timestamp, buffer, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    if (!parse_timestamp(entry->timestamp, entry->timestamp)) return 0;
    
    char *level_start = timestamp_end + 1;
    char *level_end = strchr(level_start, ' ');
    if (!level_end) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= 10) return 0;
    
    char level_str[10];
    memcpy(level_str, level_start, level_len);
    level_str[level_len] = '\0';
    
    entry->level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    char *message_start = level_end + 1;
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    
    strcpy(entry->message, message_start);
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    
    printf("Enter log entries (timestamp LEVEL message). Empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        LogEntry entry;
        if (!read_log_entry(stdin, &entry)) {
            char buffer[MAX_LINE_LEN];
            if (!fgets(buffer, sizeof(buffer), stdin)) break;
            if (strlen(buffer) == 1 && buffer[0] == '\n') break;
            printf("Invalid log entry format. Skipping.\n");
            continue;
        }
        
        entries[entry_count] = entry;
        level_counts[entry.level]++;
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Level distribution:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("  %s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i], 
               level_counts[i] * 100.0 / entry_count);
    }
    
    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, LEVEL_NAME(entries[i].level), 
               entries[i].message);
    }
    
    int error_count = level_counts[3];
    if (error_count > 0) {
        printf("\nERROR entries found: %d\n", error_count);
        printf("ERROR messages:\n");
        for (int i = 0; i < entry_count; i++) {
            if (entries[i].level == 3) {
                printf("  %s: %s\n", entries[i].timestamp, entries[i].message);
            }
        }
    }
    
    return 0;
}