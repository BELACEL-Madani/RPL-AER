/**
 * @file rpl-aer.h
 * @brief RPL-AER Protocol Header
 * @author RPL-AER Team
 * @date 2024
 */

#ifndef RPL_AER_H_
#define RPL_AER_H_

#include "contiki.h"
#include "net/routing/rpl-classic/rpl.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/log.h"

/* ===== RPL-AER Configuration ===== */

/* MCS Metric Structure */
typedef struct {
  float nre;  // Normalized Residual Energy
  float pec;  // Predictive Energy Consumption
  float ecs;  // Environmental Context Score
  float mcs;  // Multi-Criteria Score
} rpl_aer_mcs_t;

/* Energy State Structure */
typedef struct {
  uint32_t residual_energy;  // Current energy in mWh
  uint32_t total_energy;     // Total capacity in mWh
  float consumption_rate;     // Energy consumption rate
  uint8_t energy_source;     // 0: Battery, 1: Solar
  float solar_irradiance;    // Current solar irradiance
} rpl_aer_energy_t;

/* LSTM Prediction Structure */
typedef struct {
  float history[LSTM_WINDOW_SIZE];
  float prediction;
  float weights[LSTM_WINDOW_SIZE];
  uint8_t window_index;
} rpl_aer_lstm_t;

/* Security State Structure */
typedef struct {
  int16_t rssi_threshold;
  uint8_t ack_received;
  uint8_t anomaly_detected;
  uint8_t attack_probability;
} rpl_aer_security_t;

/* Node Context Structure */
typedef struct {
  rpl_aer_mcs_t mcs;
  rpl_aer_energy_t energy;
  rpl_aer_lstm_t lstm;
  rpl_aer_security_t security;
  uint8_t mobility_state;
  uint8_t attack_state;
} rpl_aer_context_t;

/* ===== Function Declarations ===== */

/* MCS Metric Functions */
void rpl_aer_mcs_init(void);
float rpl_aer_calculate_nre(void);
float rpl_aer_calculate_pec(void);
float rpl_aer_calculate_ecs(void);
float rpl_aer_calculate_mcs(void);

/* Energy Management Functions */
void rpl_aer_energy_init(void);
void rpl_aer_energy_update(void);
void rpl_aer_solar_simulation(void);
uint32_t rpl_aer_get_residual_energy(void);

/* LSTM Prediction Functions */
void rpl_aer_lstm_init(void);
void rpl_aer_lstm_update(float new_value);
float rpl_aer_lstm_predict(void);
void rpl_aer_lstm_train(void);

/* Security Functions */
void rpl_aer_security_init(void);
uint8_t rpl_aer_check_rssi_anomaly(int16_t rssi);
uint8_t rpl_aer_validate_ack(void);
void rpl_aer_detect_attacks(void);

/* Context Management */
void rpl_aer_context_init(void);
void rpl_aer_context_update(void);
rpl_aer_context_t* rpl_aer_get_context(void);

/* RPL Integration */
void rpl_aer_objective_function_init(void);
uint16_t rpl_aer_calculate_rank(void);
uint8_t rpl_aer_select_parent(void);

/* Performance Monitoring */
void rpl_aer_performance_init(void);
void rpl_aer_calculate_pdr(void);
void rpl_aer_calculate_latency(void);
void rpl_aer_calculate_throughput(void);

/* Attack Simulation */
void rpl_aer_attack_init(void);
void rpl_aer_simulate_sinkhole(void);
void rpl_aer_simulate_selective_forwarding(void);

/* Mobility Support */
void rpl_aer_mobility_init(void);
void rpl_aer_update_mobility(void);

/* Logging Functions */
void rpl_aer_log_mcs(void);
void rpl_aer_log_energy(void);
void rpl_aer_log_security(void);
void rpl_aer_log_performance(void);

/* ===== Global Variables ===== */
extern rpl_aer_context_t rpl_aer_ctx;
extern uint32_t rpl_aer_packet_count;
extern uint32_t rpl_aer_delivered_count;
extern uint32_t rpl_aer_total_latency;
extern uint32_t rpl_aer_attack_count;

/* ===== Constants ===== */
#define RPL_AER_MCS_UPDATE_INTERVAL (10 * CLOCK_SECOND)
#define RPL_AER_ENERGY_UPDATE_INTERVAL (5 * CLOCK_SECOND)
#define RPL_AER_SECURITY_UPDATE_INTERVAL (2 * CLOCK_SECOND)
#define RPL_AER_PERFORMANCE_UPDATE_INTERVAL (30 * CLOCK_SECOND)

#define RPL_AER_ENERGY_SOURCE_BATTERY 0
#define RPL_AER_ENERGY_SOURCE_SOLAR 1

#define RPL_AER_ATTACK_NONE 0
#define RPL_AER_ATTACK_SINKHOLE 1
#define RPL_AER_ATTACK_SELECTIVE_FORWARDING 2

#define RPL_AER_MOBILITY_STATIC 0
#define RPL_AER_MOBILITY_MOBILE 1

#endif /* RPL_AER_H_ */