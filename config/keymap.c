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

