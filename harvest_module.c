#include "harvest_module.h"
#include <stdio.h>
#include "project-conf.h"

static harvest_state_t harvest_state = {0, 0, 0, 0};

/**
 * \brief Update harvested, stored, and consumed energy.
 *        Log [HARVEST] events for scientific analysis.
 */
void update_harvested_energy(float psolar, float consumption) {
  // Update harvested energy (e.g., from solar panel)
  harvest_state.harvested_energy = psolar;
  // Update consumed energy
  harvest_state.consumed_energy = consumption;
  // Update stored energy (simple model)
  harvest_state.stored_energy += psolar - consumption;
  if(harvest_state.stored_energy < 0) harvest_state.stored_energy = 0;
  // Log event
  printf("[HARVEST] psolar=%.2fJ, stored=%.2fJ, consumed=%.2fJ\n", psolar, harvest_state.stored_energy, consumption);
}

/**
 * \brief Return predicted energy for the next period (stub, to be replaced by LSTM predictor).
 */
float get_predicted_energy(void) {
  // TODO: Integrate with LSTM predictor
  return harvest_state.predicted_energy;
}

/**
 * \brief Return current harvest/storage state.
 */
harvest_state_t get_harvest_state(void) {
  return harvest_state;
}