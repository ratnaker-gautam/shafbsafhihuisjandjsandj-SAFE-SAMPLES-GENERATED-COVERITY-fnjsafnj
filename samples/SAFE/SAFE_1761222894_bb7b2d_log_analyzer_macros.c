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

int parse_timestamp(const char *str, char *timestamp) {
    if (!str || !timestamp) return 0;
    
    int i = 0;
    while (str[i] && i < TIMESTAMP_LEN - 1) {
        if (!IS_TIMESTAMP_CHAR(str[i])) return 0;
        timestamp[i] = str[i];
        i++;
    }
    timestamp[i] = '\0';
    return i > 0;
}

int parse_level(const char *str) {
    if (!str) return -1;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strstr(str, LEVEL_NAME(i)) != NULL) {
            return i;
        }
    }
    return -1;
}

int read_log_entries(LogEntry *entries, int max_entries) {
    if (!entries || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter empty line to finish.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        
        char timestamp[TIMESTAMP_LEN];
        if (!parse_timestamp(line, timestamp)) {
            printf("Invalid timestamp format. Skipping.\n");
            continue;
        }
        
        const char *level_start = line + TIMESTAMP_LEN;
        while (*level_start == ' ') level_start++;
        
        int level = parse_level(level_start);
        if (!IS_VALID_LEVEL(level)) {
            printf("Invalid log level. Skipping.\n");
            continue;
        }
        
        const char *message_start = strstr(level_start, LEVEL_NAME(level));
        if (!message_start) {
            printf("Could not find message. Skipping.\n");
            continue;
        }
        
        message_start += strlen(LEVEL_NAME(level));
        while (*message_start == ' ') message_start++;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, message_start, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(const LogEntry *entries, int count) {
    if (!entries || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i], 
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
    
    printf("\nRecent entries:\n");
    int show_count = count > 5 ? 5 : count;
    for (int i = count - show_count; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, 
               LEVEL_NAME(entries[i].level), entries[i].message);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}