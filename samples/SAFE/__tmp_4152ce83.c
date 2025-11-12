//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define IS_VALID_INDEX(i) ((i) >= 0 && (i) < entry_count)
#define LEVEL_NAME(l) (level_names[(l)])

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
    int len = strlen(str);
    if (len < TIMESTAMP_LEN - 1) return 0;
    
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

static int add_entry(const char *timestamp, int level, const char *message) {
    if (!timestamp || !message || level < 0 || level >= LOG_LEVELS) return 0;
    if (entry_count >= MAX_ENTRIES) return 0;
    
    LogEntry *entry = &entries[entry_count];
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    entry->level = level;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry_count++;
    return 1;
}

static void print_summary(void) {
    int counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < entry_count; i++) {
        if (IS_VALID_INDEX(i) && IS_VALID_LEVEL(entries[i].level)) {
            counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", LEVEL_NAME(i), counts[i]);
    }
}

static void print_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) return;
    
    printf("Entries for %s:\n", LEVEL_NAME(level));
    for (int i = 0; i < entry_count; i++) {
        if (IS_VALID_INDEX(i) && entries[i].level == level) {
            printf("[%s] %s\n", entries[i].timestamp, entries[i].message);
        }
    }
}

static int process_line(const char *line) {
    if (!line || strlen(line) >= MAX_LINE_LEN) return 0;
    
    char timestamp[TIMESTAMP_LEN];
    char level_str[16];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%19s %15s %1023[^\n]", timestamp, level_str, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp, timestamp)) {
        return 0;
    }
    
    int level = parse_level(level_str);
    if (level == -1) {
        return 0;
    }
    
    return add_entry(timestamp, level, message);
}

int main(void) {
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter empty line to finish\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Maximum entries reached\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) {
            break;
        }
        
        if (!process_line(line)) {
            printf("Invalid log entry format\n");
        }
    }
    
    if