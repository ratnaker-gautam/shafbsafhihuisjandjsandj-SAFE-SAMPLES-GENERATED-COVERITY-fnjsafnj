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
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ')

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_timestamp(const char *str, char *timestamp) {
    int count = 0;
    while (*str && count < TIMESTAMP_LEN - 1 && IS_TIMESTAMP_CHAR(*str)) {
        timestamp[count++] = *str++;
    }
    timestamp[count] = '\0';
    return (count > 0);
}

int parse_level(const char *str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strncmp(str, LEVEL_NAME(i), strlen(LEVEL_NAME(i))) == 0) {
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
        
        char *ptr = line;
        char timestamp[TIMESTAMP_LEN];
        
        if (!parse_timestamp(ptr, timestamp)) {
            continue;
        }
        
        ptr += strlen(timestamp);
        while (*ptr && isspace(*ptr)) ptr++;
        
        int level = parse_level(ptr);
        if (!IS_VALID_LEVEL(level)) {
            continue;
        }
        
        ptr += strlen(LEVEL_NAME(level));
        while (*ptr && isspace(*ptr)) ptr++;
        
        if (strlen(ptr) == 0) continue;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, ptr, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        char *newline = strchr(entries[count].message, '\n');
        if (newline) *newline = '\0';
        
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
        printf("%s: %d\n", LEVEL_NAME(i), level_counts[i]);
    }
}

void filter_by_level(const LogEntry entries[], int count, int target_level) {
    if (!IS_VALID_LEVEL(target_level)) {
        printf("Invalid log level\n");
        return;
    }
    
    printf("Entries with level %s:\n", LEVEL_NAME(target_level));
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("[%s] %s: %s\n", entries[i].timestamp, LEVEL_NAME(entries[i].level), entries[i].message);
            found++;
        }
    }
    if (!found) {
        printf("No entries found\n");
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count;
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Example: 2023-10-15 14:30:25 INFO User logged in\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input\n");
    
    entry_count = read_log_entries(entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    printf("\n");
    print_statistics(entries, entry_count);
    
    printf("\nEnter level to filter (0=DEBUG, 1=INFO, 2=WARNING, 3=ERROR): ");
    int filter_level;
    if (scanf("%d", &filter_level) == 1) {
        if (filter_level >= 0 && filter_level < LOG_LEVELS) {
            printf("\n");
            filter_by_level(entries, entry_count, filter_level);
        }