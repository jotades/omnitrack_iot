#ifndef TDMA_H
#define TDMA_H

#include <Arduino.h>

// TDMA Configuration
#define TDMA_CYCLE_DURATION_MS 1000  // Total TDMA cycle duration in ms
#define TDMA_NUM_SLOTS 3           // Number of slots in the TDMA cycle
#define SLOT_DURATION_MS (TDMA_CYCLE_DURATION_MS / TDMA_NUM_SLOTS)

// Assign slot to this device
#ifndef TAG_ID
#define TAG_ID 2 // Default ID; ensure each device has a unique TAG_ID
#endif

#define TAG_SLOT (TAG_ID % TDMA_NUM_SLOTS)  // Slot assigned to this tag

// Global variable to store time offset between anchor and tag
long globalTimeOffset = 0;  // Will be updated based on synchronization

// TDMA Timing Functions
inline unsigned long getSynchronizedTime() {
    // Return the synchronized time adjusted by the offset
    return millis() + globalTimeOffset;
}

inline unsigned long getCurrentSlot() {
    // Use synchronized time for calculating the current slot
    return (getSynchronizedTime() % TDMA_CYCLE_DURATION_MS) / SLOT_DURATION_MS;
}

inline bool isMySlot() {
    // Check if the current slot matches the tag's assigned slot
    return getCurrentSlot() == TAG_SLOT;
}

// Function to synchronize with anchor based on anchor's timestamp
void synchronizeWithAnchor(unsigned long anchorTimestamp) {
    // Calculate the time difference (offset) between anchor's timestamp and local time (millis)
    long timeOffset = anchorTimestamp - millis();
    
    // Update the global time offset
    globalTimeOffset = timeOffset;

    // Optional: Print the offset for debugging
    Serial.print("Synchronization Offset: ");
    Serial.println(globalTimeOffset);
}
#endif // TDMA_H
