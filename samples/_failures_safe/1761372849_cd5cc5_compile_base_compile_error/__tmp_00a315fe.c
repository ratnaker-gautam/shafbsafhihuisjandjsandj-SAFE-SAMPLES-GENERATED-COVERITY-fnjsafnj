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

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_timestamp(const char* str, char* timestamp) {
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else if (!isdigit(str[i])) {
            return 0;
        }
    }
    strncpy(timestamp, str, TIMESTAMP_LEN - 1);
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_level(const char* str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strncmp(str, level_names[i], strlen(level_names[i])) == 0) {
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
        
        char* pos = line;
        while (*pos && isspace(*pos)) pos++;
        if (*pos == '\0') continue;
        
        char timestamp[TIMESTAMP_LEN];
        if (!parse_timestamp(pos, timestamp)) continue;
        
        pos += TIMESTAMP_LEN - 1;
        while (*pos && isspace(*pos)) pos++;
        if (*pos == '\0') continue;
        
        int level = parse_level(pos);
        if (level == -1) continue;
        
        pos += strlen(level_names[level]);
        while (*pos && isspace(*pos)) pos++;
        if (*pos == '\0') continue;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, pos, MAX_LINE_LEN - TIMESTAMP_LEN - 10);
        entries[count].message[MAX_LINE_LEN - TIMESTAMP_LEN - 11] = '\0';
        
        char* newline = strchr(entries[count].message, '\n');
        if (newline) *newline = '\0';
        
        count++;
    }
    
    return count;
}

void generate_summary(LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= 0 && entries[i].level < LOG_LEVELS) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
    
    if (count > 0) {
        printf("\nFirst timestamp: %s\n", entries[0].timestamp);
        printf("Last timestamp: %s\n", entries[count-1].timestamp);
    }
}

void filter_by_level(LogEntry entries[], int count, int min_level) {
    if (min_level < 0 || min_level >= LOG_LEVELS) return;
    
    printf("\nEntries with level %s and above:\n", level_names[min_level]);
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= min_level) {
            printf("%s [%s] %s\n", entries[i].timestamp, 
                   level_names[entries[i].level], entries[i].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D to finish input\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    generate_summary(entries, count);
    
    printf("\nEnter minimum level to filter (0=DEBUG, 1=INFO, 2=WARNING, 3=ERROR): ");
    int min_level;
    if (scanf("%d", &min_level