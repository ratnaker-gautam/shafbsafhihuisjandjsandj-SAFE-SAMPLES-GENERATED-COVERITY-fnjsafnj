//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    entry->error_count = 0;
    if (strcmp(level, "ERROR") == 0) entry->error_count = 1;
    
    return 1;
}

void analyze_log_patterns(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    char common_words[10][32] = {0};
    int word_counts[10] = {0};
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        
        char message_copy[256];
        strncpy(message_copy, entries[i].message, sizeof(message_copy) - 1);
        message_copy[sizeof(message_copy) - 1] = '\0';
        
        char* word = strtok(message_copy, " ");
        while (word != NULL) {
            for (int j = 0; j < 10; j++) {
                if (strcmp(common_words[j], word) == 0) {
                    word_counts[j]++;
                    break;
                } else if (common_words[j][0] == '\0') {
                    strncpy(common_words[j], word, 31);
                    common_words[j][31] = '\0';
                    word_counts[j] = 1;
                    break;
                }
            }
            word = strtok(NULL, " ");
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Error rate: %.2f%%\n", (float)error_count / count * 100);
    
    printf("Most common words:\n");
    for (int i = 0; i < 5 && common_words[i][0] != '\0'; i++) {
        printf("  %s: %d occurrences\n", common_words[i], word_counts[i]);
    }
}

void generate_sample_logs(LogEntry* entries, int* count) {
    if (entries == NULL || count == NULL) return;
    
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login attempted",
        "Database connection established",
        "Memory usage high",
        "Network timeout occurred",
        "File not found",
        "Configuration loaded",
        "Backup completed",
        "Security alert triggered",
        "Performance degradation detected"
    };
    
    time_t current_time = time(NULL);
    *count = 0;
    
    for (int i = 0; i < 500 && *count < MAX_ENTRIES; i++) {
        LogEntry entry;
        
        time_t log_time = current_time - (rand() % 86400);
        struct tm* timeinfo = localtime(&log_time);
        if (timeinfo == NULL) continue;
        
        strftime(entry.timestamp, sizeof(entry.timestamp), 
                "%Y-%m-%d %H:%M:%S", timeinfo);
        
        int level_idx = rand() % 3;
        strcpy(entry.level, levels[level_idx]);
        
        int msg_idx = rand() % 10;
        strcpy(entry.message, messages[msg_idx]);
        
        entry.error_count = (level_idx == 2) ? 1 : 0;
        
        entries[*count] = entry;
        (*count)++;
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    srand((unsigned int)time(NULL));
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {