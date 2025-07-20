/**
 * @file mcs_calculator.c
 * @brief MCS Calculator for RPL-AER
 * Implements the Multi-Criteria Score: MCS = α·NRE + β·ETXnorm + γ·Trustscore
 *
 * Reference: RPL-AER: A Secure, Sustainable, and Predictive Routing Protocol for Low-Power IoT Networks (section IV-B)
 */

#include "mcs_calculator.h"
#include <stdio.h>

float mcs_calculator_compute(float nre, float etx_norm, float trustscore, float alpha, float beta, float gamma) {
    // Equation (9) from the article
    return alpha * nre + beta * etx_norm + gamma * trustscore;
}

void mcs_calculator_log(float mcs, float nre, float etx_norm, float trustscore, float alpha, float beta, float gamma) {
    printf("[MCS] MCS=%.3f | NRE=%.3f | ETXnorm=%.3f | Trust=%.3f | α=%.2f β=%.2f γ=%.2f\n",
           mcs, nre, etx_norm, trustscore, alpha, beta, gamma);
}