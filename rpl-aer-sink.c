/**
 * @file rpl-aer-sink.c
 * @brief RPL-AER Sink Node Implementation
 * @author RPL-AER Team
 * @date 2024
 */

#include "contiki.h"
#include "contiki-net.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/ctimer.h"
#include "sys/log.h"
#include "rpl-aer.h"

#define LOG_MODULE "RPL-AER-Sink"
#define LOG_LEVEL LOG_LEVEL_INFO

#define SINK_PORT 5678
#define SINK_INTERVAL (60 * CLOCK_SECOND)

static struct udp_conn *sink_conn;
static struct ctimer sink_timer;
static uint32_t sink_packets = 0;
static uint32_t sink_energy_total = 0;
static float sink_avg_mcs = 0.0f;

/* ===== Sink Functions ===== */

static void sink_timer_callback(void *ptr) {
  LOG_INFO("RPL-AER-Sink: Performance Report\n");
  LOG_INFO("  Total packets received: %lu\n", sink_packets);
  LOG_INFO("  Average MCS: %.3f\n", sink_avg_mcs);
  LOG_INFO("  Total energy consumed: %lu mWh\n", sink_energy_total);
  LOG_INFO("  Attack count: %lu\n", rpl_aer_attack_count);

  // Reset counters
  sink_packets = 0;
  sink_energy_total = 0;
  sink_avg_mcs = 0.0f;

  ctimer_set(&sink_timer, SINK_INTERVAL, sink_timer_callback, NULL);
}

static void tcpip_handler(void) {
  if(uip_newdata()) {
    uint8_t *data = uip_appdata;
    uint8_t data_len = uip_datalen();

    LOG_INFO("RPL-AER-Sink: Received data from ");
    LOG_INFO_6ADDR(&UIP_IP_BUF->srcipaddr);
    LOG_INFO_(" (len=%d)\n", data_len);

    // Extract RPL-AER context
    if (data_len >= 32 + sizeof(rpl_aer_context_t)) {
      rpl_aer_context_t client_ctx;
      memcpy(&client_ctx, &data[32], sizeof(rpl_aer_context_t));

      // Update sink statistics
      sink_packets++;
      sink_energy_total += client_ctx.energy.residual_energy;
      sink_avg_mcs = (sink_avg_mcs * (sink_packets - 1) + client_ctx.mcs.mcs) / sink_packets;

      LOG_INFO("RPL-AER-Sink: Client MCS=%.3f, Energy=%lu mWh\n",
               client_ctx.mcs.mcs, client_ctx.energy.residual_energy);
    }

    // Send acknowledgment
    static uint8_t ack_data[16];
    sprintf((char*)ack_data, "SINK-ACK-%lu", sink_packets);

    uip_udp_packet_sendto(sink_conn, ack_data, strlen((char*)ack_data),
                          &UIP_IP_BUF->srcipaddr, SINK_PORT);
  }
}

/* ===== Process Definition ===== */

PROCESS(rpl_aer_sink_process, "RPL-AER Sink Process");
AUTOSTART_PROCESSES(&rpl_aer_sink_process);

PROCESS_THREAD(rpl_aer_sink_process, ev, data) {
  PROCESS_BEGIN();

  LOG_INFO("RPL-AER-Sink: Starting sink node\n");

  // Initialize RPL-AER
  rpl_aer_init();

  // Create UDP connection
  sink_conn = udp_new(NULL, SINK_PORT, NULL);
  udp_bind(sink_conn, SINK_PORT);

  LOG_INFO("RPL-AER-Sink: Listening on port %d\n", SINK_PORT);

  // Start periodic reporting
  ctimer_set(&sink_timer, SINK_INTERVAL, sink_timer_callback, NULL);

  while(1) {
    PROCESS_WT_EVENT();

    if(ev == tcpip_event) {
      tcpip_handler();
    }
  }

  PROCESS_END();
}