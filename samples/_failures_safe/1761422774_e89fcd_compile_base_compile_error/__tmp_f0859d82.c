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
#define IS_EMPTY_STR(s) ((s) == NULL || (s)[0] == '\0')

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_timestamp(const char *str, char *timestamp) {
    if (IS_EMPTY_STR(str) || timestamp == NULL) return 0;
    
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
    if (IS_EMPTY_STR(str)) return -1;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcasecmp(str, level_names[i]) == 0)
            return i;
    }
    return -1;
}

int read_log_entry(FILE *file, LogEntry *entry) {
    if (file == NULL || entry == NULL) return 0;
    
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    line[strcspn(line, "\n")] = '\0';
    
    char timestamp_str[TIMESTAMP_LEN];
    char level_str[16];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%19s %15s %[^\n]", timestamp_str, level_str, message) != 3)
        return 0;
    
    if (!parse_timestamp(timestamp_str, entry->timestamp)) return 0;
    
    int level = parse_level(level_str);
    if (!IS_VALID_LEVEL(level)) return 0;
    
    entry->level = level;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN] = "";
    char latest[TIMESTAMP_LEN] = "";
    
    for (int i = 0; i < count; i++) {
        if (!IS_VALID_LEVEL(entries[i].level)) continue;
        
        level_counts[entries[i].level]++;
        
        if (earliest[0] == '\0' || strcmp(entries[i].timestamp, earliest) < 0)
            strcpy(earliest, entries[i].timestamp);
        
        if (latest[0] == '\0' || strcmp(entries[i].timestamp, latest) > 0)
            strcpy(latest, entries[i].timestamp);
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s\n", earliest, latest);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i],
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (timestamp LEVEL message), one per line.\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message\n");
    printf("Levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Enter empty line to finish.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        
        if (IS_EMPTY_STR(line))