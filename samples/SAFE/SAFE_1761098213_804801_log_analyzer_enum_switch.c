//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG
};

enum LogLevel parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    return LOG_INFO;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char buffer[256];
    int counts[4] = {0};
    int total_logs = 0;
    
    printf("Enter log entries (format: LEVEL MESSAGE). Enter 'END' to finish.\n");
    printf("Example: INFO User logged in\n");
    
    while (1) {
        printf("> ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        char level_str[32];
        char message[224];
        
        if (sscanf(buffer, "%31s %223[^\n]", level_str, message) != 2) {
            printf("Invalid format. Use: LEVEL MESSAGE\n");
            continue;
        }
        
        for (int i = 0; level_str[i]; i++) {
            level_str[i] = toupper((unsigned char)level_str[i]);
        }
        
        enum LogLevel level = parse_log_level(level_str);
        
        switch (level) {
            case LOG_INFO:
                counts[0]++;
                break;
            case LOG_WARNING:
                counts[1]++;
                break;
            case LOG_ERROR:
                counts[2]++;
                break;
            case LOG_DEBUG:
                counts[3]++;
                break;
            default:
                counts[0]++;
                break;
        }
        
        total_logs++;
        
        printf("Processed: [%s] %s\n", level_to_string(level), message);
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total logs processed: %d\n", total_logs);
    printf("INFO: %d\n", counts[0]);
    printf("WARNING: %d\n", counts[1]);
    printf("ERROR: %d\n", counts[2]);
    printf("DEBUG: %d\n", counts[3]);
    
    if (total_logs > 0) {
        printf("\nDistribution:\n");
        printf("INFO: %.1f%%\n", (counts[0] * 100.0) / total_logs);
        printf("WARNING: %.1f%%\n", (counts[1] * 100.0) / total_logs);
        printf("ERROR: %.1f%%\n", (counts[2] * 100.0) / total_logs);
        printf("DEBUG: %.1f%%\n", (counts[3] * 100.0) / total_logs);
    }
    
    return 0;
}