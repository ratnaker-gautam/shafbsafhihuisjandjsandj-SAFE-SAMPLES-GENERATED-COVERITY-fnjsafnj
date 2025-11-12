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

#define IS_VALID_LEVEL(level) ((level) >= 0 && (level) < LOG_LEVELS)
#define IS_VALID_TIMESTAMP(ts) (strlen(ts) == TIMESTAMP_LEN - 1)
#define LEVEL_NAME(level) (level_names[(level)])

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_log_level(const char *level_str) {
    if (!level_str) return -1;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(level_str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int validate_timestamp(const char *timestamp) {
    if (!timestamp || !IS_VALID_TIMESTAMP(timestamp)) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (strlen(line) == 0) continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[16];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
        
        if (sscanf(line, "%19s %15s %[^\n]", timestamp, level_str, message) != 3) {
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
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
    int level_counts[LOG_LEVELS] = {0};
    int total_messages = 0;
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
            total_messages++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries processed: %d\n", count);
    printf("Valid messages: %d\n", total_messages);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        double percentage = total_messages > 0 ? (level_counts[i] * 100.0) / total_messages : 0.0;
        printf("%-8s: %3d (%5.1f%%)\n", LEVEL_NAME(i), level_counts[i], percentage);
    }
    
    if (count > 0) {
        printf("\nFirst valid entry: %s %s %s\n", 
               entries[0].timestamp, LEVEL_NAME(entries[0].level), entries[0].message);
        
        if (count > 1) {
            printf("Last valid entry:  %s %s %s\n", 
                   entries[count-1].timestamp, LEVEL_NAME(entries[count-1].level), entries[count-1].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}