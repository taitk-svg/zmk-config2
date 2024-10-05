#include <device.h>
#include <drivers/uart.h>
#include <zephyr.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/gatt.h>

#define UART_DEVICE_NAME "UART_0"
const struct device *uart_dev;

static void uart_rx_callback(const struct device *dev) {
    uint8_t rx_data;
    while (uart_poll_in(dev, &rx_data) == 0) {
        // Forward data to BLE
        bt_send_data(rx_data);  // Implement this function
    }
}

void setup_uart(void) {
    uart_dev = device_get_binding(UART_DEVICE_NAME);
    if (!uart_dev) {
        return;  // Handle error
    }

    uart_irq_callback_set(uart_dev, uart_rx_callback);
    uart_irq_rx_enable(uart_dev);
}

// Function to send data via BLE
void bt_send_data(uint8_t data) {
    // Implement BLE data sending logic here
}

void main(void) {
    // Initialize Bluetooth
    int err = bt_enable(NULL);
    if (err) {
        return;  // Handle error
    }

    // Set up UART
    setup_uart();

    // Other initialization code
    while (1) {
        // Main loop
        k_sleep(K_MSEC(100));
    }
}

#define BT_UUID_MY_SERVICE BT_UUID_DECLARE_128(0x1234567890abcdef1234567890abcdef)
#define BT_UUID_MY_CHAR    BT_UUID_DECLARE_128(0xabcdef1234567890abcdef1234567890)

static struct bt_gatt_attr attrs[] = {
    BT_GATT_PRIMARY_SERVICE(BT_UUID_MY_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_MY_CHAR, BT_GATT_CHRC_WRITE | BT_GATT_CHRC_NOTIFY),
    BT_GATT_DESCRIPTOR(BT_UUID_MY_CHAR, BT_GATT_PERM_WRITE | BT_GATT_PERM_READ, NULL, NULL, NULL, NULL),
};

// Send data via BLE
void bt_send_data(uint8_t data) {
    // Here you should write the data to your characteristic
    bt_gatt_notify(NULL, &attrs[1], &data, sizeof(data));
}

/*
 * Copyright (c) 2021 Nick Winans
 *
 * SPDX-License-Identifier: MIT
 */

#include <behaviors.dtsi>
#include <dt-bindings/zmk/keys.h>
#include <dt-bindings/zmk/rgb.h>
#include <dt-bindings/zmk/bt.h>

/ {
    keymap {
        compatible = "zmk,keymap";

        default_layer {
// ------------------------------------------------------------------------------------------
// | ESC |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |   BSPC   |
// | TAB  |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |   "|"   |
// | CAPS  |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |     ENTER    |
// |  SHIFT  |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |      SHIFT       |
// |  CTL  |  WIN  |  ALT  |            SPACE              |  ALT  |  WIN  |  MO(1) |  CTL  |
// ------------------------------------------------------------------------------------------
            bindings = <
    &gresc  &kp N1 &kp N2 &kp N3 &kp N4 &kp N5 &kp N6 &kp N7 &kp N8 &kp N9 &kp N0 &kp MINUS &kp EQUAL  &kp BSPC
    &kp TAB  &kp Q  &kp W  &kp E  &kp R  &kp T  &kp Y  &kp U  &kp I  &kp O  &kp P  &kp LBKT  &kp RBKT  &kp BSLH
    &kp CLCK  &kp A  &kp S  &kp D  &kp F  &kp G  &kp H  &kp J  &kp K  &kp L  &kp SEMI &kp SQT           &kp RET
    &kp LSHFT   &kp Z  &kp X  &kp C  &kp V  &kp B  &kp N  &kp M  &kp COMMA &kp DOT &kp FSLH           &kp RSHFT
    &kp LCTRL &kp LGUI &kp LALT             &kp SPACE                     &kp RALT  &kp RGUI  &mo 1   &kp RCTRL
            >;
        };

        rgb_layer {
// ------------------------------------------------------------------------------------------------
// | BT CLR | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10  | F11 | F12 | EFFECT REV |
// |  BT 1   |   |  UP |   | HUEUP | SATUP | BRIUP | SPDUP |   |     |    |     |     |           |
// |   BT 2   | LT |  DN  | RT | HUEDN | SATDN | BRIDN | SPDDN |   |     |   |     |  EFFECT FORW |
// |    BT 3     |     |      |      |     |     |     |     |     |     |     |                  |
// |   BT 4  |      |      |             TOG RGB                | PRT SCR |       |       |  DEL  |
// ------------------------------------------------------------------------------------------------
            bindings = <
    &bt BT_CLR   &kp F1    &kp F2    &kp F3  &kp F4           &kp F5          &kp F6          &kp F7          &kp F8   &kp F9   &kp F10  &kp F11 &kp F12  &rgb_ug RGB_EFR
    &bt BT_SEL 0  &trans    &kp UP    &trans  &rgb_ug RGB_HUI  &rgb_ug RGB_SAI &rgb_ug RGB_BRI &rgb_ug RGB_SPI  &trans   &trans   &trans   &trans  &trans          &trans
    &bt BT_SEL 1   &kp LEFT  &kp DOWN  &kp RIGHT &rgb_ug RGB_HUD &rgb_ug RGB_SAD &rgb_ug RGB_BRD &rgb_ug RGB_SPD &trans   &trans   &trans   &trans        &rgb_ug RGB_EFF
    &bt BT_SEL 2    &trans    &trans    &trans   &trans          &trans          &trans          &trans          &trans   &trans   &trans                          &trans
    &bt BT_SEL 3  &trans   &trans                            &rgb_ug RGB_TOG                                      &kp PSCRN      &trans      &trans               &kp DEL
            >;
        };
    };
};

