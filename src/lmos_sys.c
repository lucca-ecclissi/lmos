#include "mgos_system.h"
#include "mgos_mongoose.h"
#include "mgos_net.h"
#include "mgos_sys_config.h"
#include "mgos_timers.h"
#include "mgos_utils.h"
#ifdef MGOS_HAVE_WIFI
#include "mgos_wifi.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lmos_sys.h"

#if MGOS_ENABLE_SYS_SERVICE
int get_sys_info(struct json_out *out) {
    struct mgos_net_ip_info ip_info;
  memset(&ip_info, 0, sizeof(ip_info));
#ifdef MGOS_HAVE_WIFI
  char *status = mgos_wifi_get_status_str();
  char *ssid = mgos_wifi_get_connected_ssid();
  char sta_ip[16], ap_ip[16];
  memset(sta_ip, 0, sizeof(sta_ip));
  memset(ap_ip, 0, sizeof(ap_ip));
  if (mgos_net_get_ip_info(MGOS_NET_IF_TYPE_WIFI, MGOS_NET_IF_WIFI_STA,
                           &ip_info)) {
    mgos_net_ip_to_str(&ip_info.ip, sta_ip);
  }
  if (mgos_net_get_ip_info(MGOS_NET_IF_TYPE_WIFI, MGOS_NET_IF_WIFI_AP,
                           &ip_info)) {
    mgos_net_ip_to_str(&ip_info.ip, ap_ip);
  }
#endif
#ifdef MGOS_HAVE_ETHERNET
  char eth_ip[16];
  memset(eth_ip, 0, sizeof(eth_ip));
  if (mgos_net_get_ip_info(MGOS_NET_IF_TYPE_ETHERNET, 0, &ip_info)) {
    mgos_net_ip_to_str(&ip_info.ip, eth_ip);
  }
#endif
  (void) ip_info;

  int len = json_printf(
      out,
      "{app: %Q, fw_version: %Q, fw_id: %Q, mac: %Q, "
      "arch: %Q, uptime: %lu, "
      "ram_size: %u, ram_free: %u, ram_min_free: %u, "
      "fs_size: %u, fs_free: %u"
#ifdef MGOS_HAVE_WIFI
      ",wifi: {sta_ip: %Q, ap_ip: %Q, status: %Q, ssid: %Q}"
#endif
#ifdef MGOS_HAVE_ETHERNET
      ",eth: {ip: %Q}"
#endif
      "}",
      MGOS_APP, mgos_sys_ro_vars_get_fw_version(), mgos_sys_ro_vars_get_fw_id(),
      mgos_sys_ro_vars_get_mac_address(), mgos_sys_ro_vars_get_arch(),
      (unsigned long) mgos_uptime(), mgos_get_heap_size(),
      mgos_get_free_heap_size(), mgos_get_min_free_heap_size(),
      mgos_get_fs_size(), mgos_get_free_fs_size()
#ifdef MGOS_HAVE_WIFI
                              ,
      sta_ip, ap_ip, status == NULL ? "" : status, ssid == NULL ? "" : ssid
#endif
#ifdef MGOS_HAVE_ETHERNET
      ,
      eth_ip
#endif
      );

#ifdef MGOS_HAVE_WIFI
  free(ssid);
  free(status);
#endif
  return len;
}

void reboot(int delay) {
      int delay_ms = 100;
      if(delay >= 0){
            delay_ms = delay;
      }
      mgos_system_restart_after(delay_ms);
}
#endif

bool mgos_lmos_init(void) { 
      return true; 
}