#pragma once

#include <stdint.h>

/**
 * WARNING: This not should be edited without re-synchronizing with the struct
 * in the mainboard firmware (see SSL-Electronics/mainboard/firmware)
 */
#define PACKET_SIZE 16
#define PACKET_INSTRUCTIONS 2
#define MAX_ROBOTS 8

#define INSTRUCTION_MASTER 0x00
struct packet_master
{
#define ACTION_ON (1 << 0)         // The robot should be on (else everything is stopped)
#define ACTION_KICK1 (1 << 1)      // Kick on kicker 1 (transition from 0 to 1 trigger kick)
#define ACTION_KICK2 (1 << 2)      // Kick on kicker 2 (transition from 0 to 1 trigger kick)
#define ACTION_DRIBBLE (1 << 3)    // Enable/disable the dribbler
#define ACTION_CHARGE (1 << 5)     // Enable/disable the capacitor charge
#define ACTION_TARE_ODOM (1 << 7)  // Enable/disable the tare
  uint8_t actions;

  int16_t x_speed;  // Kinematic orders (mm/s and mrad/s)
  int16_t y_speed;
  int16_t t_speed;

  uint8_t kickPower;  // Kick power (this is a duration in x25 uS)
} __attribute__((packed));

#define INSTRUCTION_PARAMS 0x01
struct packet_params
{
  float kp, ki, kd;  // Servo parameter
} __attribute__((packed));

// Robot status packet
struct packet_robot
{
  uint8_t id;

#define STATUS_OK (1 << 0)          // The robot is alive and ok
#define STATUS_DRIVER_ERR (1 << 1)  // Error with drivers
#define STATUS_IR (1 << 2)          // The infrared barrier detects something
  uint8_t status;

  uint8_t cap_volt;  // Kick capcaitor voltage (V)

  uint8_t voltage;  // Battery 1 voltage (10th of V)

  int16_t xpos;  // Data planned by odometry
  int16_t ypos;  // In mm
  int16_t ang;   // In rad/1000
} __attribute__((packed));

#define MUSIC_PARAMS 0x02
struct packet_music
{
#define SOUND_ON (1 << 0)   // The sound should be on (else no sound is stopped)
#define BEEPER (1 << 1)     // Enable/disable the sound on the beeper
#define KICK (1 << 2)       // Enable/disable the sound on the kick
#define CHIP_KICK (1 << 3)  // Enable/disable the sound on the chip kick
#define DRIBBLER (1 << 5)   // Enable/disable the sound on the dribbler

  uint8_t instrument;
  uint16_t note;
  uint16_t duration;

} __attribute__((packed));
