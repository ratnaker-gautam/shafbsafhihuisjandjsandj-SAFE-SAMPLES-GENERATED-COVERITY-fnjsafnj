//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    time_t timestamp;
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return -1;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[32];
    char message[256];
    int year, month, day, hour, min, sec;
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
        return 0;
    }
    
    if (sscanf(line, "%d-%d-%d %d:%d:%d %31s %255[^\n]", 
               &year, &month, &day, &hour, &min, &sec, 
               level_str, message) != 8) {
        return -1;
    }
    
    if (year < 1900 || year > 2100 || month < 1 || month > 12 || 
        day < 1 || day > 31 || hour < 0 || hour > 23 || 
        min < 0 || min > 59 || sec < 0 || sec > 59) {
        return -1;
    }
    
    struct tm timeinfo = {0};
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min = min;
    timeinfo.tm_sec = sec;
    timeinfo.tm_isdst = -1;
    
    entry->timestamp = mktime(&timeinfo);
    if (entry->timestamp == (time_t)-1) {
        return -1;
    }
    
    int level = parse_log_level(level_str);
    if (level == -1) {
        return -1;
    }
    entry->level = level;
    
    if (strlen(message) >= sizeof(entry->message)) {
        return -1;
    }
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_log_level(struct LogEntry* entries, int count, enum LogLevel level) {
    int level_count = 0;
    time_t first_time = 0, last_time = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == level) {
            level_count++;
            if (first_time == 0 || entries[i].timestamp < first_time) {
                first_time = entries[i].timestamp;
            }
            if (entries[i].timestamp > last_time) {
                last_time = entries[i].timestamp;
            }
        }
    }
    
    if (level_count > 0) {
        printf("%s: %d occurrences\n", level_to_string(level), level_count);
        char first_str[64], last_str[64];
        struct tm tm_first, tm_last;
        struct tm* tm_info = localtime(&first_time);
        if (tm_info != NULL) {
            memcpy(&tm_first, tm_info, sizeof(struct tm));
            strftime(first_str, sizeof(first_str), "%Y-%m-%d %H:%M:%S", &tm_first);
        } else {
            strcpy(first_str, "Invalid time");
        }
        tm_info = localtime(&last_time);
        if (tm_info != NULL) {
            memcpy(&tm_last, tm_info, sizeof(struct tm));
            strftime(last_str, sizeof(last_str), "%Y-%m-%d %H:%M:%S", &tm_last);
        } else {
            strcpy(last_str, "Invalid time");
        }
        printf("  First: %s\n", first_str);
        printf("  Last:  %s\n", last_str);
    }
}

int main(void) {
    struct LogEntry entries[1000];
    int entry_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < 1000) {
        printf("Log entry %d: ", entry_count + 1);
        fflush(std