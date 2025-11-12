//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

typedef struct {
    LogEntry *entries;
    int count;
    int capacity;
} LogCollection;

int parse_severity(const char *level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
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

int add_log_entry(LogCollection *collection, const char *timestamp, const char *level, const char *message) {
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity <= 0) new_capacity = 10;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }

    if (!validate_timestamp(timestamp)) return 0;
    int severity = parse_severity(level);
    if (severity == -1) return 0;
    if (strlen(message) >= 256) return 0;

    LogEntry *entry = &collection->entries[collection->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = severity;
    collection->count++;
    return 1;
}

void analyze_logs(const LogCollection *collection) {
    int severity_count[5] = {0};
    char latest_timestamp[32] = "";

    for (int i = 0; i < collection->count; i++) {
        const LogEntry *entry = &collection->entries[i];
        if (entry->severity >= 0 && entry->severity < 5) {
            severity_count[entry->severity]++;
        }
        if (strcmp(entry->timestamp, latest_timestamp) > 0) {
            strncpy(latest_timestamp, entry->timestamp, 31);
            latest_timestamp[31] = '\0';
        }
    }

    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", collection->count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("CRITICAL: %d\n", severity_count[4]);
    printf("Latest timestamp: %s\n", latest_timestamp);
}

void free_log_collection(LogCollection *collection) {
    if (collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
    }
    collection->count = 0;
    collection->capacity = 0;
}

int main(void) {
    LogCollection collection;
    collection.entries = NULL;
    collection.count = 0;
    collection.capacity = 0;

    char input_buffer[MAX_LINE_LENGTH];
    char timestamp[32];
    char level[16];
    char message[256];

    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Enter 'END' on a separate line to finish input\n");

    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        if (strncmp(input_buffer, "END", 3) == 0 && (input_buffer[3] == '\n' || input_buffer[3] == '\0')) {
            break;
        }

        int parsed = sscanf(input_buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed == 3) {
            if (add_log_entry(&collection, timestamp, level, message)) {
                printf("Added: %s %s