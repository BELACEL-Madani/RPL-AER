#ifndef ATTACK_DETECTOR_H
#define ATTACK_DETECTOR_H

/**
 * \file attack_detector.h
 * \brief Attack detection module for RPL-AER protocol.
 *
 * This module monitors behavioral trust indicators and detects routing attacks.
 * It provides interfaces for trust analysis, attack detection, and event reporting.
 *
 * Main interfaces:
 *  - analyze_behavioral_trust(): Analyze trust indicators.
 *  - detect_attack(): Detect attacks based on indicators.
 *  - report_attack_event(): Log and report detected attacks.
 */

#include <stdint.h>

/** \brief Structure holding trust/attack indicators */
typedef struct {
  float ack_ratio;      /**< ACK reception ratio */
  float forwarding_rate;/**< Forwarding behavior */
  float rssi;           /**< Received Signal Strength Indicator */
  float rank;           /**< RPL rank value */
  float trust_variation;/**< Trust score variation */
} trust_attack_indicators_t;

/** \brief Analyze behavioral trust indicators */
void analyze_behavioral_trust(const trust_attack_indicators_t *indicators);

/** \brief Detect attacks based on trust/behavioral indicators */
int detect_attack(const trust_attack_indicators_t *indicators);

/** \brief Report/log detected attack event */
void report_attack_event(const char *attack_type, float confidence);

#endif // ATTACK_DETECTOR_H