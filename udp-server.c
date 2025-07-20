/**
 * @file udp-server.c
 * @brief UDP Server for RPL-AER Testing
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

#define LOG_MODULE "UDP-Server"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_SERVER_PORT 5678
#define UDP_CLIENT_PORT 8765

static struct udp_conn *server_conn;
static uint32_t received_packets = 0;
static uint32_t total_latency = 0;

/* ===== UDP Server Functions ===== */

static void tcpip_handler(void) {
  if(uip_newdata()) {
    uint8_t *data = uip_appdata;
    uint8_t data_len = uip_datalen();
    uint32_t current_time = clock_time();

    LOG_INFO("UDP-Server: Received packet from ");
    LOG_INFO_6ADDR(&UIP_IP_BUF->srcipaddr);
    LOG_INFO_(" (len=%d)\n", data_len);

    // Extract RPL-AER context from packet
    if (data_len >= 32 + sizeof(rpl_aer_context_t)) {
      rpl_aer_context_t client_ctx;
      memcpy(&client_ctx, &data[32], sizeof(rpl_aer_context_t));

      LOG_INFO("UDP-Server: Client MCS=%.3f, Energy=%lu mWh\n",
               client_ctx.mcs.mcs, client_ctx.energy.residual_energy);

      // Calculate latency (simplified)
      uint32_t latency = current_time - (uint32_t)data[28]; // Extract timestamp
      total_latency += latency;

      LOG_INFO("UDP-Server: Packet latency = %lu ms\n", latency);
    }

    // Send acknowledgment
    static uint8_t ack_data[16];
    sprintf((char*)ack_data, "ACK-%lu", received_packets);

    uip_udp_packet_sendto(server_conn, ack_data, strlen((char*)ack_data),
                          &UIP_IP_BUF->srcipaddr, UDP_CLIENT_PORT);

    received_packets++;
    rpl_aer_delivered_count++;

    LOG_INFO("UDP-Server: Sent ACK, total received = %lu\n", received_packets);
  }
}

/* ===== Process Definition ===== */

PROCESS(udp_server_process, "UDP Server Process");
AUTOSTART_PROCESSES(&udp_server_process);

PROCESS_THREAD(udp_server_process, ev, data) {
  PROCESS_BEGIN();

  LOG_INFO("UDP-Server: Starting RPL-AER UDP server\n");

  // Initialize RPL-AER
  rpl_aer_init();

  // Create UDP connection
  server_conn = udp_new(NULL, UDP_CLIENT_PORT, NULL);
  udp_bind(server_conn, UDP_SERVER_PORT);

  LOG_INFO("UDP-Server: Listening on port %d\n", UDP_SERVER_PORT);

  while(1) {
    PROCESS_WT_EVENT();

    if(ev == tcpip_event) {
      tcpip_handler();
    }
  }

  PROCESS_END();
}