/**
 * @file project-conf.h
 * @brief RPL-AER Project Configuration
 * @author RPL-AER Team
 * @date 2024
 */

#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

/* ===== RPL-AER Configuration ===== */

/* Enable RPL-AER protocol */
#define RPL_AER 1

/* MCS Metric Configuration */
#define MCS_METRIC 1
#define NRE_WEIGHT 0.4f
#define PEC_WEIGHT 0.35f
#define ECS_WEIGHT 0.25f

/* LSTM Prediction Configuration */
#define LSTM_PREDICTION 1
#define LSTM_WINDOW_SIZE 10
#define LSTM_PREDICTION_HORIZON 5
#define LSTM_UPDATE_INTERVAL (15 * CLOCK_SECOND)

/* Energy Monitoring */
#define ENABLE_ENERGY_MONITOR 1
#define ENERGY_UPDATE_INTERVAL (5 * CLOCK_SECOND)
#define SOLAR_ENERGY_SIM 1
#define ENERGY_HARVESTING 1

/* Security Configuration */
#define SECURITY_ACK 1
#define RSSI_ANOMALY 1
#define ATTACK_SIMULATION 1
#define SINKHOLE_DETECTION 1
#define RSSI_THRESHOLD -85
#define ACK_TIMEOUT (2 * CLOCK_SECOND)

/* Mobility Support */
#define MOBILITY_SUPPORT 1
#define RANDOM_WAYPOINT 1
#define MOBILITY_SPEED 5  // m/s
#define MOBILITY_PAUSE_TIME 10  // seconds

/* Network Configuration */
#define UDP_CLIENT_CONF_N 1
#define UDP_SERVER_CONF_N 1
#define UDP_CLIENT_CONF_RETRANSMISSIONS 3
#define UDP_SERVER_CONF_RETRANSMISSIONS 3

/* RPL Configuration */
#define RPL_CONF_STATS 1
#define RPL_CONF_LEAF_ONLY 0
#define RPL_CONF_DEFAULT_INSTANCE 0
#define RPL_CONF_DEFAULT_DIO_INTERVAL_MIN 12
#define RPL_CONF_DEFAULT_DIO_INTERVAL_DOUBLINGS 8
#define RPL_CONF_DEFAULT_DIO_REDUNDANCY 10
#define RPL_CONF_DEFAULT_DIS_INTERVAL 60
#define RPL_CONF_DEFAULT_TRICKLE_DELAY 1
#define RPL_CONF_DEFAULT_TRICKLE_IMIN 12
#define RPL_CONF_DEFAULT_TRICKLE_IMAX 9
#define RPL_CONF_DEFAULT_TRICKLE_K 1

/* IPv6 Configuration */
#define UIP_CONF_IPV6 1
#define UIP_CONF_IPV6_RPL 1
#define UIP_CONF_ND6_SEND_NA 1
#define UIP_CONF_ND6_SEND_NS 1
#define UIP_CONF_ND6_SEND_RA 1
#define UIP_CONF_ND6_SEND_RS 1
#define UIP_CONF_ND6_SEND_DNS 1
#define UIP_CONF_ND6_SEND_DA 1

/* MAC Configuration */
#define IEEE802154_CONF_PANID 0xABCD
#define IEEE802154_CONF_DEFAULT_CHANNEL 26

/* Logging Configuration */
#ifndef DEBUG
#define DEBUG 1
#endif
#ifndef PRINTF
#define PRINTF 1
#endif
#define LOG_CONF_LEVEL_RPL LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_MAC LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_FRAMER LOG_LEVEL_INFO

/* Simulation Configuration */
#define SIMULATION_ENERGY_TRACE 1
#define SIMULATION_PACKET_TRACE 1
#define SIMULATION_ATTACK_TRACE 1

/* Performance Monitoring */
#define ENABLE_PERFORMANCE_MONITOR 1
#define PDR_CALCULATION_INTERVAL (30 * CLOCK_SECOND)
#define LATENCY_CALCULATION_INTERVAL (10 * CLOCK_SECOND)
#define THROUGHPUT_CALCULATION_INTERVAL (20 * CLOCK_SECOND)

/* Attack Simulation Parameters */
#define ATTACK_PROBABILITY 0.1f  // 10% chance of attack
#define SINKHOLE_PROBABILITY 0.05f  // 5% chance of sinkhole
#define SELECTIVE_FORWARDING_PROBABILITY 0.05f  // 5% chance of selective forwarding

/* Solar Energy Parameters */
#define SOLAR_UPDATE_INTERVAL (60 * CLOCK_SECOND)
#define BATTERY_CAPACITY 2000     // mAh
#define SOLAR_PANEL_EFFICIENCY 0.15f

/* MCS Metric Parameters */
#define NRE_DECAY_FACTOR 0.95f
#define PEC_LEARNING_RATE 0.01f
#define ECS_WEATHER_FACTOR 1.0f
#define ECS_MOBILITY_FACTOR 0.8f

// === Harvest Module Parameters ===
#define HARVEST_ENERGY_THRESHOLD  10.0f   // Minimum stored energy (J) for routing
#define HARVEST_LOG_ENABLED      1        // Enable [HARVEST] logs

// === Attack Detector Parameters ===
#define ATTACK_DETECTOR_ENABLED  1        // Enable attack detection
#define ATTACK_CONFIDENCE_THRESHOLD 0.5f  // Confidence threshold for reporting attacks
#define ATTACK_LOG_ENABLED       1        // Enable [ATTACK], [FP], [TRUST_ANOMALY] logs

#endif /* PROJECT_CONF_H_ */