//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_UNKNOWN
};

enum LogLevel parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    return LOG_UNKNOWN;
}

void process_log_entry(const char* timestamp, enum LogLevel level, const char* message) {
    switch (level) {
        case LOG_INFO:
            printf("[INFO] %s: %s\n", timestamp, message);
            break;
        case LOG_WARNING:
            printf("[WARNING] %s: %s\n", timestamp, message);
            break;
        case LOG_ERROR:
            printf("[ERROR] %s: %s\n", timestamp, message);
            break;
        case LOG_DEBUG:
            printf("[DEBUG] %s: %s\n", timestamp, message);
            break;
        case LOG_UNKNOWN:
            printf("[UNKNOWN] %s: %s\n", timestamp, message);
            break;
    }
}

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    if (timestamp[4] != '-' || timestamp[7] != '-' || timestamp[10] != ' ' || 
        timestamp[13] != ':' || timestamp[16] != ':') return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(timestamp[i])) return 0;
    }
    
    int year, month, day, hour, minute, second;
    if (sscanf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", 
               &year, &month, &day, &hour, &minute, &second) != 6) return 0;
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    if (second < 0 || second > 59) return 0;
    
    return 1;
}

int main(void) {
    char timestamp[64];
    char level_str[32];
    char message[256];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Enter 'quit' to exit\n\n");
    
    while (1) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(timestamp, sizeof(timestamp), stdin) == NULL) break;
        
        size_t len = strlen(timestamp);
        if (len > 0 && timestamp[len - 1] == '\n') {
            timestamp[len - 1] = '\0';
        }
        
        if (strcmp(timestamp, "quit") == 0) break;
        
        if (strlen(timestamp) == 0) continue;
        
        char input_buffer[512];
        if (strlen(timestamp) >= sizeof(input_buffer)) {
            printf("Error: Input too long\n");
            continue;
        }
        strcpy(input_buffer, timestamp);
        
        char* token = strtok(input_buffer, " ");
        if (token == NULL) {
            printf("Error: Invalid format\n");
            continue;
        }
        
        strcpy(timestamp, token);
        
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Error: Missing level\n");
            continue;
        }
        strcpy(level_str, token);
        
        token = strtok(NULL, "");
        if (token == NULL) {
            printf("Error: Missing message\n");
            continue;
        }
        strcpy(message, token);
        
        if (!validate_timestamp(timestamp)) {
            printf("Error: Invalid timestamp format (expected: YYYY-MM-DD HH:MM:SS)\n");
            continue;
        }
        
        enum LogLevel level = parse_log_level(level_str);
        
        process_log_entry(timestamp, level, message);
        entry_count++;
    }
    
    printf("\nProcessed %d log entries\n", entry_count);
    
    return 0;
}