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

#define IS_VALID_LEVEL(l) ((l) >= LOG_INFO && (l) <= LOG_DEBUG)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

int parse_timestamp(const char* str, char* timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    
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

int parse_level(const char* str) {
    if (str == NULL) return -1;
    
    if (strncmp(str, "INFO", 4) == 0) return LOG_INFO;
    if (strncmp(str, "WARN", 4) == 0) return LOG_WARN;
    if (strncmp(str, "ERROR", 5) == 0) return LOG_ERROR;
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_DEBUG;
    
    return -1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    if (entries == NULL || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter empty line to finish.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
        
        if (sscanf(line, "%19s %9s %255[^\n]", timestamp, level_str, message) != 3) {
            printf("Invalid format. Skipping.\n");
            continue;
        }
        
        char parsed_ts[TIMESTAMP_LEN];
        if (!parse_timestamp(timestamp, parsed_ts)) {
            printf("Invalid timestamp. Skipping.\n");
            continue;
        }
        
        int level = parse_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            printf("Invalid log level. Skipping.\n");
            continue;
        }
        
        strncpy(entries[count].timestamp, parsed_ts, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(const struct log_entry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN] = "";
    char latest[TIMESTAMP_LEN] = "";
    
    for (int i = 0; i < count; i++) {
        if (!IS_VALID_LEVEL(entries[i].level)) continue;
        
        level_counts[entries[i].level]++;
        
        if (i == 0) {
            strncpy(earliest, entries[i].timestamp, TIMESTAMP_LEN - 1);
            earliest[TIMESTAMP_LEN - 1] = '\0';
            strncpy(latest, entries[i].timestamp, TIMESTAMP_LEN - 1);
            latest[TIMESTAMP_LEN - 1] = '\0';
        } else {
            if (strcmp(entries[i].timestamp, earliest) < 0) {
                strncpy(earliest, entries[i].timestamp, TIMESTAMP_LEN - 1);
                earliest[TIMESTAMP_LEN - 1] = '\0';
            }
            if (strcmp(entries[i