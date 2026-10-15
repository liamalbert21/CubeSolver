#ifndef MOTOR_DEFS_H
#define MOTOR_DEFS_H

#include "Pinout.h"

typedef enum
{
    // PWM1
    MOTOR_SIDE_WHITE,
    MOTOR_SIDE_ORANGE,
    MOTOR_SIDE_GREEN,
    
    // PWM2
    MOTOR_SIDE_BLUE,
    MOTOR_SIDE_RED,
    MOTOR_SIDE_YELLOW,
    
    MOTOR_NUM_SIDES
} MotorSide;

typedef enum
{
    MOTOR_CHANNEL_PWM1,
    MOTOR_CHANNEL_PWM2,
    MOTOR_NUM_PWM_CHANNELS
} MotorPWMChannel;

typedef enum
{
    MOTOR_DIR_CW,
    MOTOR_DIR_CCW
} MotorDir;

typedef enum
{
    MOTOR_ON,
    MOTOR_OFF
} MotorStatus;

typedef struct
{
    MotorSide side;
    MotorDir  dir;
} MotorConfig;

typedef struct
{
    Pinout sleep, dir;
} MotorPinout;

typedef struct
{
    int degrees, time_ms;
} MotorPWMConfig;

#define RED_SLEEP_PORT      GPIOB
#define RED_SLEEP_PIN       LL_GPIO_PIN_2
#define RED_SLEEP_PINOUT    RED_SLEEP_PORT, RED_SLEEP_PIN

#define RED_DIR_PORT        GPIOA
#define RED_DIR_PIN         LL_GPIO_PIN_0
#define RED_DIR_PINOUT      RED_DIR_PORT, RED_DIR_PIN

#define ORANGE_SLEEP_PORT   GPIOB
#define ORANGE_SLEEP_PIN    LL_GPIO_PIN_1
#define ORANGE_SLEEP_PINOUT ORANGE_SLEEP_PORT, ORANGE_SLEEP_PIN

#define ORANGE_DIR_PORT     GPIOA
#define ORANGE_DIR_PIN      LL_GPIO_PIN_1
#define ORANGE_DIR_PINOUT   ORANGE_DIR_PORT, ORANGE_DIR_PIN

#define YELLOW_SLEEP_PORT   GPIOB
#define YELLOW_SLEEP_PIN    LL_GPIO_PIN_15
#define YELLOW_SLEEP_PINOUT YELLOW_SLEEP_PORT, YELLOW_SLEEP_PIN

#define YELLOW_DIR_PORT     0
#define YELLOW_DIR_PIN      0
#define YELLOW_DIR_PINOUT   YELLOW_DIR_PORT, YELLOW_DIR_PIN

#define GREEN_SLEEP_PORT    GPIOB
#define GREEN_SLEEP_PIN     LL_GPIO_PIN_14
#define GREEN_SLEEP_PINOUT  GREEN_SLEEP_PORT, GREEN_SLEEP_PIN

#define GREEN_DIR_PORT      0
#define GREEN_DIR_PIN       0
#define GREEN_DIR_PINOUT    GREEN_DIR_PORT, GREEN_DIR_PIN

#define BLUE_SLEEP_PORT     GPIOB
#define BLUE_SLEEP_PIN      LL_GPIO_PIN_13
#define BLUE_SLEEP_PINOUT   BLUE_SLEEP_PORT, BLUE_SLEEP_PIN

#define BLUE_DIR_PORT       0
#define BLUE_DIR_PIN        0
#define BLUE_DIR_PINOUT     BLUE_DIR_PORT, BLUE_DIR_PIN

#define WHITE_SLEEP_PORT    GPIOC
#define WHITE_SLEEP_PIN     LL_GPIO_PIN_4
#define WHITE_SLEEP_PINOUT  WHITE_SLEEP_PORT, WHITE_SLEEP_PIN

#define WHITE_DIR_PORT      0
#define WHITE_DIR_PIN       0
#define WHITE_DIR_PINOUT    WHITE_DIR_PORT, WHITE_DIR_PIN

#endif