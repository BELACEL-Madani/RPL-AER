/**
 * @file trust_module.c
 * @brief Trust Module for RPL-AER
 * Implements trust score computation and anomaly detection (section IV-E).
 *
 * Reference: RPL-AER: A Secure, Sustainable, and Predictive Routing Protocol for Low-Power IoT Networks (section IV-E)
 */

#include "trust_module.h"
#include <stdio.h>
#include <math.h>

float trust_module_compute(float direct, float indirect, float behavioral, float w1, float w2, float w3) {
    // Weighted sum for trust score
    return w1 * direct + w2 * indirect + w3 * behavioral;
}

int trust_module_anomaly_detect(float rssi, int ack, float forwarding_ratio, float rank_consistency) {
    // Simple statistical anomaly detection (can be extended)
    int anomaly = 0;
    if (rssi < RSSI_THRESHOLD) anomaly = 1;
    if (!ack) anomaly = 1;
    if (forwarding_ratio < 0.8f) anomaly = 1;
    if (rank_consistency < 0.9f) anomaly = 1;
    return anomaly;
}

void trust_module_log(float trustscore, float direct, float indirect, float behavioral, float w1, float w2, float w3) {
    printf("[TRUST] Trust=%.3f | Direct=%.3f | Indirect=%.3f | Behavioral=%.3f | w1=%.2f w2=%.2f w3=%.2f\n",
           trustscore, direct, indirect, behavioral, w1, w2, w3);
}