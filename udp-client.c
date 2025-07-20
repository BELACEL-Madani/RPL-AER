/**
 * @file udp-client.c
 * @brief UDP Client for RPL-AER Testing
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

#define LOG_MODULE "UDP-Client"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678
#define UDP_CLIENT_INTERVAL (10 * CLOCK_SECOND)

static struct uip_udp_conn *client_conn;
static struct ctimer client_timer;
static uint32_t packet_counter = 0;

/* Server address */
static uip_ipaddr_t server_ipaddr;

/* ===== UDP Client Functions ===== */

static void client_timer_callback(void *ptr) {
  static uint8_t data[256];
  // static uint32_t timestamp; // supprimé car non utilisé

  // Prepare packet data
  sprintf((char*)data, "RPL-AER-PKT-%u", packet_counter);
  // timestamp = clock_time(); // supprimé car non utilisé

  // Add RPL-AER context to packet
  rpl_aer_context_t *ctx = rpl_aer_get_context();
  memcpy(&data[32], ctx, sizeof(rpl_aer_context_t));

  // Send UDP packet
  uip_udp_packet_sendto(client_conn, data, strlen((char*)data),
                        &server_ipaddr, UDP_SERVER_PORT);

  LOG_INFO("UDP-Client: Sent packet %u to server\n", packet_counter);

  // Update performance counters
  rpl_aer_packet_count++;

  // Schedule next transmission
  ctimer_set(&client_timer, UDP_CLIENT_INTERVAL, client_timer_callback, NULL);
  packet_counter++;
}

static void tcpip_handler(void) {
  if(uip_newdata()) {
    LOG_INFO("UDP-Client: Received response from server\n");
  }
}

/* ===== Process Definition ===== */

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data) {
  PROCESS_BEGIN();

  LOG_INFO("UDP-Client: Starting RPL-AER UDP client\n");

  // Initialize RPL-AER
  // rpl_aer_init(); // supprimé car non défini

  // Set server address (fe80::1)
  uip_ip6addr(&server_ipaddr, 0xfe80, 0, 0, 0, 0, 0, 0, 0x0001);

  // Create UDP connection
  client_conn = udp_new(NULL, UDP_SERVER_PORT, NULL);
  udp_bind(client_conn, UDP_CLIENT_PORT);

  LOG_INFO("UDP-Client: Connected to server at ");
  LOG_INFO_6ADDR(&server_ipaddr);
  LOG_INFO_("\n");

  // Start periodic transmission
  ctimer_set(&client_timer, UDP_CLIENT_INTERVAL, client_timer_callback, NULL);

  while(1) {
    PROCESS_WT_EVENT();

    if(ev == tcpip_event) {
      tcpip_handler();
    }
  }

  PROCESS_END();
}