/**
 * @file trust_module.h
 * @brief Header for Trust Module (RPL-AER)
 * Provides API for trust score computation and anomaly detection.
 *
 * Reference: RPL-AER: A Secure, Sustainable, and Predictive Routing Protocol for Low-Power IoT Networks (section IV-E)
 */

#ifndef TRUST_MODULE_H
#define TRUST_MODULE_H

#include "project-conf.h"

float trust_module_compute(float direct, float indirect, float behavioral, float w1, float w2, float w3);
int trust_module_anomaly_detect(float rssi, int ack, float forwarding_ratio, float rank_consistency);
void trust_module_log(float trustscore, float direct, float indirect, float behavioral, float w1, float w2, float w3);

#endif // TRUST_MODULE_H