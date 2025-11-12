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

#define IS_VALID_LEVEL(c) ((c) == 'E' || (c) == 'W' || (c) == 'I' || (c) == 'D')
#define LEVEL_TO_INDEX(c) ((c) == 'E' ? 0 : (c) == 'W' ? 1 : (c) == 'I' ? 2 : 3)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 4];
} LogEntry;

int parse_timestamp(const char* str, char* timestamp) {
    if (!str || !timestamp) return 0;
    size_t len = strlen(str);
    if (len < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit((unsigned char)str[i])) return 0;
        }
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    size_t line_len = strlen(line);
    if (line_len >= MAX_LINE_LEN - 1) return 0;
    
    if (!parse_timestamp(line, entry->timestamp)) return 0;
    
    if (line[TIMESTAMP_LEN] != ' ' || line[TIMESTAMP_LEN + 1] != '[') return 0;
    
    char level_char = line[TIMESTAMP_LEN + 2];
    if (!IS_VALID_LEVEL(level_char)) return 0;
    entry->level = level_char;
    
    if (line[TIMESTAMP_LEN + 3] != ']' || line[TIMESTAMP_LEN + 4] != ' ') return 0;
    
    const char* msg_start = line + TIMESTAMP_LEN + 5;
    size_t msg_len = strlen(msg_start);
    if (msg_len == 0 || msg_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, msg_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void generate_sample_logs(void) {
    const char* levels = "EWID";
    const char* messages[] = {
        "System startup completed",
        "User login successful",
        "Database connection established",
        "Cache cleared",
        "Invalid input detected",
        "Memory usage high",
        "Network timeout",
        "File not found",
        "Permission denied",
        "Configuration updated"
    };
    int msg_count = sizeof(messages) / sizeof(messages[0]);
    
    for (int i = 0; i < 20; i++) {
        time_t now = time(NULL);
        if (now == (time_t)-1) continue;
        struct tm tm_buf;
        struct tm* tm_info = localtime_r(&now, &tm_buf);
        if (!tm_info) continue;
        char timestamp[TIMESTAMP_LEN];
        if (strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info) == 0) continue;
        
        char level = levels[rand() % LOG_LEVELS];
        const char* message = messages[rand() % msg_count];
        
        printf("%s [%c] %s\n", timestamp, level, message);
    }
}

void print_summary(int entry_count, int level_counts[]) {
    printf("\n=== Analysis Summary ===\n");
    printf("Total entries: %d\n", entry_count);
    printf("Error (E): %d\n", level_counts[LOG_LEVEL_ERROR]);
    printf("Warning (W): %d\n", level_counts[LOG_LEVEL_WARN]);
    printf("Info (I): %d\n", level_counts[LOG_LEVEL_INFO]);
    printf("Debug (D): %d\n", level_counts[LOG_LEVEL_DEBUG]);
    
    if (entry_count > 0) {
        printf("\nDistribution:\n");
        const char* level_names[] = {"Error", "Warning", "Info", "Debug"};
        for (int i = 0; i < LOG_LEVELS; i++) {
            float percentage = (float)level_counts[i] / entry_count * 100.0f;
            printf("  %s: %.1f%%\n", level_names[i], percentage);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRI