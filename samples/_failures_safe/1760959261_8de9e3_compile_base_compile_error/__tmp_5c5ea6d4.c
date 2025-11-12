//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
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
#define LEVEL_TO_IDX(c) ((c) == 'E' ? 0 : (c) == 'W' ? 1 : (c) == 'I' ? 2 : 3)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 3];
} LogEntry;

int parse_timestamp(const char* str, char* timestamp) {
    if (!str || !timestamp) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int read_log_entries(LogEntry* entries, int max_entries, const char* filename) {
    if (!entries || max_entries <= 0 || !filename) return -1;
    
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        
        if (len < TIMESTAMP_LEN + 3) continue;
        
        char timestamp[TIMESTAMP_LEN];
        if (!parse_timestamp(line, timestamp)) continue;
        
        if (line[TIMESTAMP_LEN] != ' ' || line[TIMESTAMP_LEN + 1] != '[') continue;
        
        char level_char = line[TIMESTAMP_LEN + 2];
        if (!IS_VALID_LEVEL(level_char)) continue;
        
        if (line[TIMESTAMP_LEN + 3] != ']' || line[TIMESTAMP_LEN + 4] != ' ') continue;
        
        strcpy(entries[count].timestamp, timestamp);
        entries[count].level = level_char;
        
        const char* message_start = line + TIMESTAMP_LEN + 5;
        size_t message_len = strlen(message_start);
        if (message_len >= sizeof(entries[count].message)) {
            message_len = sizeof(entries[count].message) - 1;
        }
        strncpy(entries[count].message, message_start, message_len);
        entries[count].message[message_len] = '\0';
        
        count++;
    }
    
    fclose(file);
    return count;
}

void generate_sample_log(const char* filename) {
    if (!filename) return;
    
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    const char* levels = "EWID";
    const char* messages[] = {
        "System startup completed",
        "User login successful",
        "Database connection established",
        "Cache cleared",
        "Invalid input detected",
        "Disk space running low",
        "Network timeout occurred",
        "Configuration file missing"
    };
    
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    
    for (int i = 0; i < 20; i++) {
        char timestamp[TIMESTAMP_LEN];
        tm_info.tm_sec = (tm_info.tm_sec + 1) % 60;
        if (tm_info.tm_sec == 0) tm_info.tm_min = (tm_info.tm_min + 1) % 60;
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_info);
        
        char level = levels[rand() % 4];
        const char* message = messages[rand() % 8];
        
        fprintf(file, "%s [%c] %s\n", timestamp, level, message);
    }
    
    fclose(file);
}

int main(void) {
    const char* filename = "sample.log";
    
    printf("Generating sample log file...\n");
    generate_sample_log(filename);
    
    LogEntry entries[MAX_ENTRIES];
    int count = read_log_entries(entries, MAX_ENTRIES, filename);
    
    if (count < 0) {
        printf("Error: Could not read log file\n");
        return