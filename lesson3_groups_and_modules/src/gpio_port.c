// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "gpio_port.h"

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

#define GPIO_PORT_COUNT        (2U)
#define GPIO_PIN_COUNT         (16U)
#define GPIO_INVALID_PIN_STATE (2U)

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

static bool gpio_port_is_valid_pin(gpio_port_pin_t pin);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

static bool g_gpio_ready = false;
static gpio_port_level_t g_pin_levels[GPIO_PORT_COUNT][GPIO_PIN_COUNT];

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

gpio_port_status_t gpio_port_init(void)
{
  uint8_t port_index = 0U;
  uint8_t pin_index = 0U;

  while (port_index < GPIO_PORT_COUNT)
  {
    pin_index = 0U;

    while (pin_index < GPIO_PIN_COUNT)
    {
      g_pin_levels[port_index][pin_index] = GPIO_PORT_LEVEL_LOW;
      pin_index++;
    }

    port_index++;
  }

  g_gpio_ready = true;
  return GPIO_PORT_STATUS_OK;
}

gpio_port_status_t gpio_port_write(gpio_port_pin_t pin, gpio_port_level_t level)
{
  if (!g_gpio_ready)
  {
    return GPIO_PORT_STATUS_NOT_READY;
  }

  if (!gpio_port_is_valid_pin(pin))
  {
    return GPIO_PORT_STATUS_INVALID_ARG;
  }

  g_pin_levels[pin.port_index][pin.pin_index] = level;
  return GPIO_PORT_STATUS_OK;
}

gpio_port_status_t gpio_port_read(gpio_port_pin_t pin, gpio_port_level_t *level)
{
  if (level == 0)
  {
    return GPIO_PORT_STATUS_INVALID_ARG;
  }

  if (!g_gpio_ready)
  {
    return GPIO_PORT_STATUS_NOT_READY;
  }

  if (!gpio_port_is_valid_pin(pin))
  {
    return GPIO_PORT_STATUS_INVALID_ARG;
  }

  *level = g_pin_levels[pin.port_index][pin.pin_index];
  return GPIO_PORT_STATUS_OK;
}

gpio_port_status_t gpio_port_toggle(gpio_port_pin_t pin)
{
  if (!g_gpio_ready)
  {
    return GPIO_PORT_STATUS_NOT_READY;
  }

  if (!gpio_port_is_valid_pin(pin))
  {
    return GPIO_PORT_STATUS_INVALID_ARG;
  }

  if (g_pin_levels[pin.port_index][pin.pin_index] == GPIO_PORT_LEVEL_LOW)
  {
    g_pin_levels[pin.port_index][pin.pin_index] = GPIO_PORT_LEVEL_HIGH;
  }
  else
  {
    g_pin_levels[pin.port_index][pin.pin_index] = GPIO_PORT_LEVEL_LOW;
  }

  return GPIO_PORT_STATUS_OK;
}

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

static bool gpio_port_is_valid_pin(gpio_port_pin_t pin)
{
  if ((pin.port_index >= GPIO_PORT_COUNT) || (pin.pin_index >= GPIO_PIN_COUNT))
  {
    return false;
  }

  return true;
}
