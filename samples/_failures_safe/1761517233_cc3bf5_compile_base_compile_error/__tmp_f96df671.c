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

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
static int level_counts[LOG_LEVELS] = {0};

int parse_timestamp(const char *str, char *timestamp) {
    if (!str || !timestamp) return 0;
    
    int year, month, day, hour, min, sec;
    if (sscanf(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec) != 6)
        return 0;
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (min < 0 || min > 59) return 0;
    if (sec < 0 || sec > 59) return 0;
    
    snprintf(timestamp, TIMESTAMP_LEN, "%04d-%02d-%02d %02d:%02d:%02d", 
             year, month, day, hour, min, sec);
    return 1;
}

int parse_log_level(const char *str) {
    if (!str) return -1;
    
    if (strncmp(str, "ERROR", 5) == 0) return LOG_LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LOG_LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LOG_LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_LEVEL_DEBUG;
    
    return -1;
}

int parse_log_line(const char *line, struct log_entry *entry) {
    if (!line || !entry) return 0;
    
    char level_str[10];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%19s %9s %[^\n]", entry->timestamp, level_str, message) != 3)
        return 0;
    
    int level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(level)) return 0;
    
    if (!parse_timestamp(entry->timestamp, entry->timestamp))
        return 0;
    
    entry->level = level;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void print_summary(void) {
    printf("Log Level Summary:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (len >= MAX_LINE_LEN - 1) {
            printf("Line too long, skipping.\n");
            continue;
        }
        
        struct log_entry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            level_counts[entry.level]++;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    for (int i = 0; i < entry_count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, 
               level_names[entries[i].level], entries[i].message);
    }
    
    printf("\n");
    print_summary();
    
    int error_count = level_counts[LOG_LEVEL_ERROR];
    int warn_count = level_counts[LOG_LEVEL_WARN];
    
    if (error_count > 0) {
        printf("