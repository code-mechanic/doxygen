// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include "timer_pwm.h"

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

#define TIMER_INSTANCE_COUNT (2U)

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

/**
 * @brief Tracks runtime state for one timer instance in the lesson driver.
 */
typedef struct
{
  bool is_initialized; /**< Indicates whether the instance has been initialized. */
} timer_pwm_runtime_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                           Function Prototypes                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

static bool timer_pwm_is_valid_instance(timer_instance_t instance);
static timer_regs_t *timer_pwm_get_regs(timer_instance_t instance);

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                          Variable Declarations                           |
// |                                                                          |
// +--------------------------------------------------------------------------+

static timer_regs_t g_timer_regs[TIMER_INSTANCE_COUNT];
static timer_pwm_runtime_t g_timer_runtime[TIMER_INSTANCE_COUNT];

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

timer_pwm_status_t timer_pwm_init(const timer_pwm_config_t *config)
{
  timer_regs_t *regs = 0;

  if ((config == 0) ||
      (!timer_pwm_is_valid_instance(config->instance)) ||
      (config->period_ticks == 0U) ||
      (config->duty_ticks > config->period_ticks))
  {
    return TIMER_PWM_STATUS_INVALID_ARG;
  }

  regs = timer_pwm_get_regs(config->instance);
  regs->CTRL = TIMER_CTRL_MODE_PWM_MASK | TIMER_CTRL_CLOCK_DIV(config->clock_divider);
  regs->PERIOD = config->period_ticks;
  regs->DUTY = config->duty_ticks;
  regs->STATUS = 0U;

  g_timer_runtime[(uint8_t)config->instance].is_initialized = true;
  return TIMER_PWM_STATUS_OK;
}

timer_pwm_status_t timer_pwm_start(timer_instance_t instance)
{
  timer_regs_t *regs = 0;

  if (!timer_pwm_is_valid_instance(instance) ||
      !g_timer_runtime[(uint8_t)instance].is_initialized)
  {
    return TIMER_PWM_STATUS_NOT_READY;
  }

  regs = timer_pwm_get_regs(instance);
  regs->CTRL |= TIMER_CTRL_ENABLE_MASK;
  return TIMER_PWM_STATUS_OK;
}

timer_pwm_status_t timer_pwm_stop(timer_instance_t instance)
{
  timer_regs_t *regs = 0;

  if (!timer_pwm_is_valid_instance(instance) ||
      !g_timer_runtime[(uint8_t)instance].is_initialized)
  {
    return TIMER_PWM_STATUS_NOT_READY;
  }

  regs = timer_pwm_get_regs(instance);
  regs->CTRL &= (uint32_t)(~TIMER_CTRL_ENABLE_MASK);
  return TIMER_PWM_STATUS_OK;
}

timer_pwm_status_t timer_pwm_is_overflow_set(timer_instance_t instance, bool *is_set)
{
  timer_regs_t *regs = 0;

  if (is_set == 0)
  {
    return TIMER_PWM_STATUS_INVALID_ARG;
  }

  if (!timer_pwm_is_valid_instance(instance) ||
      !g_timer_runtime[(uint8_t)instance].is_initialized)
  {
    return TIMER_PWM_STATUS_NOT_READY;
  }

  regs = timer_pwm_get_regs(instance);
  *is_set = ((regs->STATUS & TIMER_STATUS_OVERFLOW_MASK) != 0U);
  return TIMER_PWM_STATUS_OK;
}

timer_pwm_status_t timer_pwm_clear_overflow(timer_instance_t instance)
{
  timer_regs_t *regs = 0;

  if (!timer_pwm_is_valid_instance(instance) ||
      !g_timer_runtime[(uint8_t)instance].is_initialized)
  {
    return TIMER_PWM_STATUS_NOT_READY;
  }

  regs = timer_pwm_get_regs(instance);
  regs->STATUS &= (uint32_t)(~TIMER_STATUS_OVERFLOW_MASK);
  return TIMER_PWM_STATUS_OK;
}

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Private                                  |
// |                                Functions                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

static bool timer_pwm_is_valid_instance(timer_instance_t instance)
{
  return ((uint8_t)instance < TIMER_INSTANCE_COUNT);
}

static timer_regs_t *timer_pwm_get_regs(timer_instance_t instance)
{
  return &g_timer_regs[(uint8_t)instance];
}
