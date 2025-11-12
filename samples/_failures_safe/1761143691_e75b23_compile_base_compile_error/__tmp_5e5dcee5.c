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
    
    int y, m, d, H, M, S;
    if (sscanf(str, "%d-%d-%d %d:%d:%d", &y, &m, &d, &H, &M, &S) != 6) return 0;
    
    if (y < 2000 || y > 2100) return 0;
    if (m < 1 || m > 12) return 0;
    if (d < 1 || d > 31) return 0;
    if (H < 0 || H > 23) return 0;
    if (M < 0 || M > 59) return 0;
    if (S < 0 || S > 59) return 0;
    
    snprintf(timestamp, TIMESTAMP_LEN, "%04d-%02d-%02d %02d:%02d:%02d", y, m, d, H, M, S);
    return 1;
}

int parse_log_level(const char *str) {
    if (IS_EMPTY_STR(str)) return -1;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcasecmp(str, level_names[i]) == 0) return i;
    }
    return -1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (timestamp level message), empty line to finish:\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        char timestamp_str[TIMESTAMP_LEN];
        char level_str[16];
        char message[MAX_LINE_LEN];
        
        if (sscanf(line, "%19s %15s %[^\n]", timestamp_str, level_str, message) < 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
            continue;
        }
        
        char timestamp[TIMESTAMP_LEN];
        if (!parse_timestamp(timestamp_str, timestamp)) {
            printf("Invalid timestamp: %s\n", timestamp_str);
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            printf("Invalid level: %s\n", level_str);
            continue;
        }
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(const LogEntry entries[], int count) {
    if (count <= 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN] = "";
    char latest[TIMESTAMP_LEN] = "";
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
        
        if (i == 0 || strcmp(entries[i].timestamp, earliest) < 0) {
            strcpy(earliest, entries[i].timestamp);
        }
        if (i == 0 || strcmp(entries[i].timestamp, latest) > 0) {
            strcpy(latest, entries[i].timestamp);
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%-8s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i],
               count > 0 ? (level_counts