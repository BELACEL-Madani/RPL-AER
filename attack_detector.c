#include "attack_detector.h"
#include <stdio.h>
#include <string.h>

/**
 * \brief Analyze behavioral trust indicators (stub for scientific logic).
 *        Logs [TRUST_ANOMALY] if abnormal behavior is detected.
 */
void analyze_behavioral_trust(const trust_attack_indicators_t *indicators) {
  // Example: log anomaly if trust variation is high
  if(indicators->trust_variation > 0.5f) {
    printf("[TRUST_ANOMALY] trust_variation=%.2f\n", indicators->trust_variation);
  }
}

/**
 * \brief Detect attacks based on trust/behavioral indicators (stub, to be completed).
 *        Returns 1 if attack detected, 0 otherwise. Logs [ATTACK] or [FP] (false positive).
 */
int detect_attack(const trust_attack_indicators_t *indicators) {
  // Example: simple threshold-based detection
  if(indicators->ack_ratio < 0.3f) {
    printf("[ATTACK] Sinkhole detected (ack_ratio=%.2f)\n", indicators->ack_ratio);
    return 1;
  }
  // TODO: Add more detection logic (Selective Forwarding, Hello Flood, Rank Attack)
  return 0;
}

/**
 * \brief Report/log detected attack event.
 */
void report_attack_event(const char *attack_type, float confidence) {
  if(confidence < 0.5f) {
    printf("[FP] False positive: %s (confidence=%.2f)\n", attack_type, confidence);
  } else {
    printf("[ATTACK] %s detected (confidence=%.2f)\n", attack_type, confidence);
  }
}