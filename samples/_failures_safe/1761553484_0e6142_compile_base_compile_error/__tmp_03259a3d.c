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

#define LOG_INFO 0
#define LOG_WARN 1
#define LOG_ERROR 2
#define LOG_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char *level_names[] = {"INFO", "WARN", "ERROR", "DEBUG"};
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
    
    if (strncmp(str, "INFO", 4) == 0) return LOG_INFO;
    if (strncmp(str, "WARN", 4) == 0) return LOG_WARN;
    if (strncmp(str, "ERROR", 5) == 0) return LOG_ERROR;
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_DEBUG;
    
    return -1;
}

int read_log_entries(struct log_entry *entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) >= sizeof(line) - 1) {
            continue;
        }
        
        char *timestamp_start = line;
        char *level_start = strchr(line, '[');
        char *level_end = strchr(line, ']');
        char *message_start = NULL;
        
        if (!level_start || !level_end || level_end <= level_start) {
            continue;
        }
        
        *level_end = '\0';
        message_start = level_end + 1;
        
        while (*message_start && isspace(*message_start)) {
            message_start++;
        }
        
        struct log_entry *entry = &entries[count];
        
        if (!parse_timestamp(timestamp_start, entry->timestamp)) {
            continue;
        }
        
        int level = parse_log_level(level_start + 1);
        if (!IS_VALID_LEVEL(level)) {
            continue;
        }
        
        entry->level = level;
        strncpy(entry->message, message_start, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        char *newline = strchr(entry->message, '\n');
        if (newline) *newline = '\0';
        
        level_counts[level]++;
        count++;
    }
    
    return count;
}

void print_summary(int total_entries) {
    printf("Log Analysis Summary\n");
    printf("===================\n");
    printf("Total entries: %d\n", total_entries);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i],
               total_entries > 0 ? (level_counts[i] * 100.0 / total_entries) : 0.0);
    }
}

void print_entries_by_level(struct log_entry *entries, int count, int target_level) {
    if (!entries || count <= 0 || !IS_VALID_LEVEL(target_level)) return;
    
    printf("\n%s entries:\n", level_names[target_level]);
    printf("------------\n");
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("%s [%s] %s\n", entries[i].timestamp, 
                   level_names[entries[i].level], entries[i].message);
        }
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS [LEVEL] message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish