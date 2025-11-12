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
#define LEVEL_NAME(l) (level_names[(l)])
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ')

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

typedef struct {
    char timestamp[TIMESTAMP_LEN + 1];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_timestamp(const char *str, char *timestamp) {
    int i = 0;
    while (str[i] && i < TIMESTAMP_LEN && IS_TIMESTAMP_CHAR(str[i])) {
        timestamp[i] = str[i];
        i++;
    }
    if (i == 0 || i >= TIMESTAMP_LEN) return 0;
    timestamp[i] = '\0';
    return 1;
}

int parse_level(const char *str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strstr(str, LEVEL_NAME(i)) != NULL) {
            return i;
        }
    }
    return -1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN + 1];
        if (!parse_timestamp(line, timestamp)) continue;
        
        int level = parse_level(line);
        if (!IS_VALID_LEVEL(level)) continue;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN);
        entries[count].timestamp[TIMESTAMP_LEN] = '\0';
        entries[count].level = level;
        
        const char *msg_start = strstr(line, LEVEL_NAME(level));
        if (msg_start) {
            msg_start += strlen(LEVEL_NAME(level));
            while (*msg_start && isspace(*msg_start)) msg_start++;
            strncpy(entries[count].message, msg_start, sizeof(entries[count].message) - 1);
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
            char *newline = strchr(entries[count].message, '\n');
            if (newline) *newline = '\0';
        } else {
            strncpy(entries[count].message, "Unknown message", sizeof(entries[count].message) - 1);
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        }
        
        count++;
    }
    
    return count;
}

void print_statistics(const LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Statistics:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i], 
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
}

void print_entries_by_level(const LogEntry entries[], int count, int target_level) {
    if (!IS_VALID_LEVEL(target_level)) return;
    
    printf("\n%s entries:\n", LEVEL_NAME(target_level));
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("[%s] %s\n", entries[i].timestamp, entries[i].message);
            found++;
        }
    }
    if (!found) {
        printf("No %s entries found.\n", LEVEL_NAME(target_level));
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp followed by level and message):\n");
    printf("Example: 2024-01-15 10:30:00 INFO System started\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_statistics(entries, count);
    
    printf("\nSelect level to view (0=DEBUG, 1=INFO, 2=WARNING, 3=ERROR): ");
    int selected_level;
    if (scanf("%d", &selected_level) != 1) {
        printf("Invalid input.\