/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/storage.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <arrow/utf8.h>
#include <arrow/credentials.h>
#include "flashmbed.h"

int check_mgc() {
  int *c = (int *)flash_start();
  if ( *c != (int) FLASH_MAGIC_NUMBER ) {
    return 0;
  }
  return 1;
}

int restore_gateway_info(arrow_gateway_t *gateway) {
  printf("restore gateway info %08x\r\n", (int)flash_start());
  return -1;
  if (check_mgc()) {
    flash_mem_t *mem = (flash_mem_t *)flash_start();
    if ( utf8check(mem->gateway_hid) && strlen(mem->gateway_hid) > 0 ) {
      property_copy(&gateway->hid, p_const(mem->gateway_hid));
      return 0;
    }
  }
  return -1;
}

void save_gateway_info(const arrow_gateway_t *gateway) {
  printf("new registration\r\n");
  flash_mem_t mem;
  memcpy(&mem, flash_start(), sizeof(flash_mem_t));
  strcpy(mem.gateway_hid, P_VALUE(gateway->hid));
  write_flash((char*)&mem, sizeof(flash_mem_t));
}

int restore_device_info(arrow_device_t *device) {
  return -1;
  if (check_mgc()) {
    flash_mem_t *mem = (flash_mem_t *)flash_start();
    if ( !utf8check(mem->device_hid) || strlen(mem->device_hid) == 0 ) {
      return -1;
    }
    property_copy(&device->hid, p_const(mem->device_hid));
  #if defined(__IBM__)
    if ( !utf8check(mem->device_eid) || strlen(mem->device_eid) == 0 ) {
      return -1;
    }
    property_copy(&device->eid, p_const(mem->device_eid));
  #endif
    return 0;
  }
  return -1;
}

void save_device_info(arrow_device_t *device) {
  flash_mem_t mem;
  memcpy(&mem, flash_start(), sizeof(flash_mem_t));
  strcpy(mem.device_hid, P_VALUE(device->hid));
#if defined(__IBM__)
  strcpy(mem.device_eid, P_VALUE(device->eid));
#endif
  write_flash((char *)&mem, sizeof(flash_mem_t));
}

void save_wifi_setting(const char *ssid, const char *pass, int sec) {
  flash_mem_t mem;
  memcpy(&mem, flash_start(), sizeof(flash_mem_t));
  strcpy(mem.ssid, ssid);
  strcpy(mem.pass, pass);
  mem.sec = sec;
  write_flash((char *)&mem, sizeof(flash_mem_t));
}

int restore_wifi_setting(char *ssid, char *pass, int *sec) {
    wifi_credentials_t *p = credentials_next();
    if ( p ) {
        strcpy(ssid, p->ssid);
        strcpy(pass, p->pass);
        *sec = p->sec;
        return 0;
    } else {
        if (check_mgc()) {
            flash_mem_t *mem = (flash_mem_t *)flash_start();
            strcpy(ssid, mem->ssid);
            strcpy(pass, mem->pass);
            *sec = mem->sec;
            return 0;
        }
    }
    return -1;
}
