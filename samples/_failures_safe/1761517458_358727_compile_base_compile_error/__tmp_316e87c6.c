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

#define IS_VALID_LEVEL(level) ((level) >= 0 && (level) < LOG_LEVELS)
#define IS_VALID_INDEX(idx) ((idx) >= 0 && (idx) < entry_count)
#define LEVEL_NAME(level) (level_names[(level)])

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
static LogEntry entries[MAX_ENTRIES];
static int entry_count = 0;

static int parse_timestamp(const char *str, char *timestamp) {
    if (!str || !timestamp) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

static int parse_level(const char *str) {
    if (!str) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

static int add_entry(const char *line) {
    if (!line || entry_count >= MAX_ENTRIES) return 0;
    
    char timestamp[TIMESTAMP_LEN];
    char level_str[10];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%19s %9s %1023[^\n]", timestamp, level_str, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp, timestamp)) return 0;
    
    int level = parse_level(level_str);
    if (!IS_VALID_LEVEL(level)) return 0;
    
    LogEntry *entry = &entries[entry_count];
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    entry->level = level;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry_count++;
    return 1;
}

static void print_stats(void) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < entry_count; i++) {
        if (IS_VALID_INDEX(i)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Statistics:\n");
    printf("Total entries: %d\n", entry_count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", LEVEL_NAME(i), level_counts[i]);
    }
}

static void print_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) return;
    
    printf("Entries with level %s:\n", LEVEL_NAME(level));
    for (int i = 0; i < entry_count; i++) {
        if (IS_VALID_INDEX(i) && entries[i].level == level) {
            printf("%s %s %s\n", entries[i].timestamp, LEVEL_NAME(entries[i].level), entries[i].message);
        }
    }
}

int main(void) {
    char line[MAX_LINE_LEN];
    int choice;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter empty line to finish input.\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (!add_entry(line)) {
            printf("Invalid log entry format. Skipping.\n");
        }
        
        if (entry_count >= MAX_ENTRIES) {
            printf("Maximum entries reached.\n");
            break;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    while (1) {
        printf("\nMenu:\n");
        printf("1. Show statistics\n");
        printf("2. Show DEBUG entries\n");
        printf("3. Show INFO entries\n");