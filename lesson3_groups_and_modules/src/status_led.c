// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "status_led.h"

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

#define STATUS_LED_COUNT (2U)

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                            Typedefs / Enums                              |
// |                                                                          |
// +--------------------------------------------------------------------------+

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                           Function Prototypes                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

static gpio_port_pin_t status_led_map_pin(status_led_id_t led);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

static bool g_status_led_ready = false;
static const gpio_port_pin_t g_status_led_pins[STATUS_LED_COUNT] =
{
  {0U, 5U},
  {1U, 3U}
};

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

gpio_port_status_t status_led_init(void)
{
  if (gpio_port_init() != GPIO_PORT_STATUS_OK)
  {
    return GPIO_PORT_STATUS_NOT_READY;
  }

  g_status_led_ready = true;
  return GPIO_PORT_STATUS_OK;
}

gpio_port_status_t status_led_set(status_led_id_t led, bool is_on)
{
  gpio_port_pin_t pin = status_led_map_pin(led);
  gpio_port_level_t level = (is_on) ? GPIO_PORT_LEVEL_HIGH : GPIO_PORT_LEVEL_LOW;

  if (!g_status_led_ready)
  {
    return GPIO_PORT_STATUS_NOT_READY;
  }

  return gpio_port_write(pin, level);
}

gpio_port_status_t status_led_toggle(status_led_id_t led)
{
  gpio_port_pin_t pin = status_led_map_pin(led);

  if (!g_status_led_ready)
  {
    return GPIO_PORT_STATUS_NOT_READY;
  }

  return gpio_port_toggle(pin);
}

gpio_port_status_t status_led_get_pin(status_led_id_t led, gpio_port_pin_t *pin)
{
  if (pin == 0)
  {
    return GPIO_PORT_STATUS_INVALID_ARG;
  }

  *pin = status_led_map_pin(led);
  return GPIO_PORT_STATUS_OK;
}

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

static gpio_port_pin_t status_led_map_pin(status_led_id_t led)
{
  if ((uint8_t)led >= STATUS_LED_COUNT)
  {
    gpio_port_pin_t invalid_pin = {0U, 0U};
    return invalid_pin;
  }

  return g_status_led_pins[(uint8_t)led];
}
