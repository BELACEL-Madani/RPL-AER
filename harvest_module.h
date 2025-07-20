#ifndef HARVEST_MODULE_H
#define HARVEST_MODULE_H

/**
 * \file harvest_module.h
 * \brief Solar energy harvesting management for RPL-AER protocol.
 *
 * This module provides functions to track, predict, and manage harvested solar energy.
 * It influences routing decisions based on harvested and predicted energy levels.
 *
 * Main interfaces:
 *  - update_harvested_energy(): Update harvested, stored, and consumed energy.
 *  - get_predicted_energy(): Return predicted energy for next period.
 *  - get_harvest_state(): Return current harvest/storage state.
 */

#include <stdint.h>

/** \brief Structure holding energy harvesting state */
typedef struct {
  float harvested_energy;   /**< Current harvested energy (Joules) */
  float stored_energy;      /**< Current stored energy (Joules) */
  float consumed_energy;    /**< Energy consumed since last update (Joules) */
  float predicted_energy;   /**< Predicted energy for next period (Joules) */
} harvest_state_t;

/** \brief Update harvested, stored, and consumed energy */
void update_harvested_energy(float psolar, float consumption);

/** \brief Get predicted energy for the next period */
float get_predicted_energy(void);

/** \brief Get current harvest/storage state */
harvest_state_t get_harvest_state(void);

#endif // HARVEST_MODULE_H