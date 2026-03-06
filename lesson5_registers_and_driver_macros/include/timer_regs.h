#ifndef LESSON5_TIMER_REGS_H
#define LESSON5_TIMER_REGS_H

/**
 * @file timer_regs.h
 * @brief Timer register map and bit definitions for the lesson driver.
 */

#ifdef __cplusplus
extern "C" {
#endif

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                 Includes                                 |
// |                                                                          |
// +--------------------------------------------------------------------------+

#include <stdint.h>

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                          Definitions and Macros                          |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @defgroup lesson5_timer Timer PWM
 * @brief Example timer register layer and PWM driver for embedded firmware.
 * @{
 */

/** @name Base Addresses */
/** @{ */

#define TIMER0_BASE_ADDRESS (0x40020000UL) /**< Example base address for timer instance 0. */
#define TIMER1_BASE_ADDRESS (0x40021000UL) /**< Example base address for timer instance 1. */

/** @} */

/** @name Register Offsets */
/** @{ */

#define TIMER_CTRL_OFFSET   (0x00UL) /**< Offset of the control register from the timer base. */
#define TIMER_PERIOD_OFFSET (0x04UL) /**< Offset of the period register from the timer base. */
#define TIMER_DUTY_OFFSET   (0x08UL) /**< Offset of the duty-cycle register from the timer base. */
#define TIMER_STATUS_OFFSET (0x0CUL) /**< Offset of the status register from the timer base. */

/** @} */

/** @name CTRL Register Bits */
/** @{ */

#define TIMER_CTRL_ENABLE_MASK    (1UL << 0U) /**< Starts the timer when set. */
#define TIMER_CTRL_MODE_PWM_MASK  (1UL << 1U) /**< Selects PWM mode when set. */
#define TIMER_CTRL_IRQ_ENABLE_MASK (1UL << 2U) /**< Enables timer interrupt generation. */

/** @} */

/** @name CTRL Register Fields */
/** @{ */

#define TIMER_CTRL_CLOCK_DIV_SHIFT (8U) /**< Bit position of the clock-divider field. */
#define TIMER_CTRL_CLOCK_DIV_MASK  (0xFFUL << TIMER_CTRL_CLOCK_DIV_SHIFT) /**< Mask of the clock-divider field. */
#define TIMER_CTRL_CLOCK_DIV(value) \
  ((((uint32_t)(value)) << TIMER_CTRL_CLOCK_DIV_SHIFT) & TIMER_CTRL_CLOCK_DIV_MASK) /**< Encodes a clock-divider value into CTRL. */

/** @} */

/** @name STATUS Register Bits */
/** @{ */

#define TIMER_STATUS_OVERFLOW_MASK      (1UL << 0U) /**< Indicates the timer overflowed. */
#define TIMER_STATUS_COMPARE_MATCH_MASK (1UL << 1U) /**< Indicates duty compare matched. */

/** @} */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Enums                              |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Available timer instances for the lesson driver.
 */
typedef enum
{
  TIMER_INSTANCE_0 = 0, /**< Selects timer instance 0. */
  TIMER_INSTANCE_1      /**< Selects timer instance 1. */
} timer_instance_t;

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                                  Public                                  |
// |                            Typedefs / Structs                            |
// |                                                                          |
// +--------------------------------------------------------------------------+

/**
 * @brief Memory map of one timer peripheral instance.
 */
typedef struct
{
  volatile uint32_t CTRL;   /**< Control register. */
  volatile uint32_t PERIOD; /**< PWM period register. */
  volatile uint32_t DUTY;   /**< PWM duty-cycle register. */
  volatile uint32_t STATUS; /**< Status register. */
} timer_regs_t;

/** @} */

// +--------------------------------------------------------------------------+
// |                                                                          |
// |                          Static inline Functions                         |
// |                                                                          |
// +--------------------------------------------------------------------------+

#ifdef __cplusplus
}
#endif

#endif  // LESSON5_TIMER_REGS_H
