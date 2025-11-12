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

#define LOG_INFO 0
#define LOG_WARN 1
#define LOG_ERROR 2
#define LOG_DEBUG 3

#define LEVEL_NAME(l) (level_names[(l)])
#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char *level_names[] = {"INFO", "WARN", "ERROR", "DEBUG"};

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

int parse_level(const char *str) {
    if (!str) return -1;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcasecmp(str, level_names[i]) == 0)
            return i;
    }
    return -1;
}

int read_log_entries(struct log_entry *entries, int max_entries) {
    if (!entries || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter empty line to finish.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        char timestamp_str[TIMESTAMP_LEN];
        char level_str[16];
        char message[MAX_LINE_LEN];
        
        line[strcspn(line, "\n")] = 0;
        
        if (sscanf(line, "%19s %15s %255[^\n]", 
                   timestamp_str, level_str, message) != 3) {
            printf("Invalid format. Skipping.\n");
            continue;
        }
        
        struct log_entry *entry = &entries[count];
        
        if (!parse_timestamp(timestamp_str, entry->timestamp)) {
            printf("Invalid timestamp. Skipping.\n");
            continue;
        }
        
        int level = parse_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            printf("Invalid log level. Skipping.\n");
            continue;
        }
        
        entry->level = level;
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(struct log_entry *entries, int count) {
    if (!entries || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN] = "";
    char latest[TIMESTAMP_LEN] = "";
    
    for (int i = 0; i < count; i++) {
        struct log_entry *entry = &entries[i];
        
        if (IS_VALID_LEVEL(entry->level)) {
            level_counts[entry->level]++;
        }
        
        if (i == 0 || strcmp(entry->timestamp, earliest) < 0) {
            strcpy(earliest, entry->timestamp);
        }
        if (i == 0 || strcmp(entry->timestamp, latest) > 0) {
            strcpy(latest, entry->timestamp);
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i],
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    int count = read_log_entries(entries