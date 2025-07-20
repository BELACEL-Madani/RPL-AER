/**
 * @file mcs_calculator.h
 * @brief Header for MCS Calculator (RPL-AER)
 * Provides API for Multi-Criteria Score computation: MCS = α·NRE + β·ETXnorm + γ·Trustscore
 *
 * Reference: RPL-AER: A Secure, Sustainable, and Predictive Routing Protocol for Low-Power IoT Networks (section IV-B)
 */

#ifndef MCS_CALCULATOR_H
#define MCS_CALCULATOR_H

#include "project-conf.h"

float mcs_calculator_compute(float nre, float etx_norm, float trustscore, float alpha, float beta, float gamma);
void mcs_calculator_log(float mcs, float nre, float etx_norm, float trustscore, float alpha, float beta, float gamma);

#endif // MCS_CALCULATOR_H