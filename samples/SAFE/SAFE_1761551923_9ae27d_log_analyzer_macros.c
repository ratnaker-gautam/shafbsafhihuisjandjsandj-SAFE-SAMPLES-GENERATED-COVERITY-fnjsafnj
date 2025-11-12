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
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ')

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_log_level(const char *str) {
    if (strncmp(str, "DEBUG", 5) == 0) return 0;
    if (strncmp(str, "INFO", 4) == 0) return 1;
    if (strncmp(str, "WARNING", 7) == 0) return 2;
    if (strncmp(str, "ERROR", 5) == 0) return 3;
    return -1;
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (!IS_TIMESTAMP_CHAR(timestamp[i])) return 0;
    }
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN];
        
        if (sscanf(line, "%19[^]]] [%9[^]]] %[^\n]", timestamp, level_str, message) != 3) {
            continue;
        }
        
        if (!validate_timestamp(timestamp)) continue;
        
        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) continue;
        
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

void filter_by_level(const LogEntry entries[], int count, int target_level) {
    if (!IS_VALID_LEVEL(target_level)) {
        printf("Invalid log level\n");
        return;
    }
    
    printf("Entries with level %s:\n", LEVEL_NAME(target_level));
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("[%s] [%s] %s\n", entries[i].timestamp, LEVEL_NAME(entries[i].level), entries[i].message);
            found++;
        }
    }
    if (!found) {
        printf("No entries found\n");
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries in format: [timestamp] [LEVEL] message\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Timestamp format: YYYY-MM-DD HH:MM:SS\n");
    printf("Enter empty line to finish input\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    printf("\n");
    generate_summary(entries, count);
    
    printf("\nEnter level to filter (0=DEBUG, 1=INFO, 2=WARNING, 3=ERROR): ");
    int filter_level;
    if (scanf("%d", &filter_level) == 1) {
        printf("\n");
        filter_by_level(entries, count, filter_level);
    } else {
        printf("Invalid input\n");
    }
    
    return 0;
}