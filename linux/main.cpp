/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

extern "C" {
#include <config.h>
#include <arrow/device.h>
#include <arrow/connection.h>
#include <arrow/mqtt.h>
#include <json/telemetry.h>
#include <ntp/ntp.h>
#include <time/time.h>
#include <arrow/events.h>
#include <arrow/state.h>
#include <arrow/devicecommand.h>
#include <stdio.h>
#if defined(__probook_4540s__)
#include <json/probook.h>
#else
#include <json/pm.h>
#endif
}

#include <iostream>

extern int get_telemetry_data(void *data);

static int test_cmd_proc(const char *str) {
  printf("test: [%s]", str);
  return 0;
}

static int fail_cmd_proc(const char *str) {
  printf("fail: [%s]", str);
  return -1;
}

int main() {
    std::cout<<std::endl<<"--- Starting new run ---"<<std::endl;

//    Hided it because we supposed linux already synchronized a time 
//    ntp_set_time_cycle();

    time_t ctTime = time(NULL);
    std::cout<<"Time is set to (UTC): "<<ctime(&ctTime)<<std::endl;

    // gateway
    arrow_gateway_t gateway;
    
    // gateway configuration
    arrow_gateway_config_t gate_config;
    
    // device
    arrow_device_t device;

    std::cout<<"register gateway via API"<<std::endl;
    while ( arrow_connect_gateway(&gateway) < 0) {
      std::cerr<<"arrow gateway connect fail..."<<std::endl;
      sleep(5);
    }

    // try to get a gateway configuration
    while ( arrow_config(&gateway, &gate_config) < 0 ) {
      std::cerr<<"arrow gateway config fail...";
      sleep(5);
    }

    // type of the configuration: 0 - IoT configuration
    std::cout<<"config: "<<gate_config.type<<std::endl;

    // device registaration
    std::cout<<"register device via API"<<std::endl;

    while (arrow_connect_device(&gateway, &device) < 0 ) {
      std::cerr<<"device connect fail..."<<std::endl;
      sleep(1);
    }

    std::cout<<"------------------------"<<std::endl;

    add_state("led", "on");
    arrow_post_state_update(&device);

    std::cout<<"send telemetry via API"<<std::endl;
    
#if defined(__probook_4540s__)
    probook_data_t data;
#else
    pm_data_t data;
#endif
    get_telemetry_data(&data);

    while ( arrow_send_telemetry(&device, &data) < 0) {
      std::cout<<"arrow: send telemetry fail"<<std::endl;
      sleep(1);
    }

    // init MQTT
    std::cout<<"mqtt connect..."<<std::endl;
    while ( mqtt_connect(&gateway, &device, &gate_config) < 0 ) {
      std::cerr<<"mqtt connect fail"<<std::endl;
      sleep(1);
    } //every sec try to connect
    
    // FIXME just for a test
    add_cmd_handler("test", &test_cmd_proc);
    add_cmd_handler("fail", &fail_cmd_proc);
    arrow_state_mqtt_run(&device);

    mqtt_subscribe();

    int i = 0;
    while (true) {
      // every 5 sec send data via mqtt
      mqtt_yield(TELEMETRY_DELAY);
      get_telemetry_data(&data);
#if defined(__probook_4540s__)
      std::cout<<"mqtt publish ["<<i++
              <<"]: T("<<data.temperature_core0
              <<", "<<data.temperature_core1<<")..."<<std::endl;
#else
      std::cout<<"mqtt publish ["<<i++
              <<"]: T("<<data.pm_2_5
              <<", "<<data.pm_10<<")..."<<std::endl;
#endif
      if ( mqtt_publish(&device, &data) < 0 ) {
        std::cerr<<"mqtt publish failure..."<<std::endl;
        mqtt_disconnect();
        while (mqtt_connect(&gateway, &device, &gate_config) < 0) { sleep(1);}
      }
    }

    arrow_device_free(&device);
    arrow_gateway_free(&gateway);

    std::cout<<std::endl<<"disconnecting...."<<std::endl;
}
