#pragma once
// MESSAGE MERIVUS_FTC_DIAGNOSTICS PACKING

#define MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS 60003


typedef struct __mavlink_merivus_ftc_diagnostics_t {
 uint64_t time_usec; /*< [us] PX4 monotonic publication timestamp.*/
 float model_residual; /*<  Motor observer model residual.*/
 float excitation; /*<  Observer excitation score.*/
 float maneuver_intensity; /*<  Observed maneuver intensity.*/
 float external_disturbance_score; /*<  Observed external-disturbance score.*/
 float vibration_score; /*<  Observed vibration score.*/
 float allocation_residual_norm; /*<  Shadow allocation residual norm.*/
 float attitude_error; /*< [rad] Attitude error magnitude.*/
 float rate_error; /*< [rad/s] Angular-rate error magnitude.*/
 float jerk; /*<  Jerk magnitude.*/
 float acceleration_magnitude; /*<  Acceleration magnitude.*/
 float angular_rate; /*< [rad/s] Angular-rate magnitude.*/
 float angular_acceleration; /*<  Angular-acceleration magnitude.*/
 uint32_t system_reason_mask; /*<  Aggregated FTC reason bit mask.*/
 uint8_t protocol_version; /*<  MERIVUS FTC telemetry contract version.*/
 uint8_t flags; /*<  Diagnostic validity and simulation flags.*/
 uint8_t simulation_motor_index; /*<  Injected SITL motor index.*/
 uint8_t simulation_target_effectiveness_pct; /*<  Requested SITL effectiveness using the compact percentage scaling.*/
 uint8_t simulation_applied_effectiveness_pct; /*<  Applied SITL effectiveness using the compact percentage scaling.*/
} mavlink_merivus_ftc_diagnostics_t;

#define MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN 65
#define MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_MIN_LEN 65
#define MAVLINK_MSG_ID_60003_LEN 65
#define MAVLINK_MSG_ID_60003_MIN_LEN 65

#define MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_CRC 5
#define MAVLINK_MSG_ID_60003_CRC 5



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_MERIVUS_FTC_DIAGNOSTICS { \
    60003, \
    "MERIVUS_FTC_DIAGNOSTICS", \
    19, \
    {  { "time_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_merivus_ftc_diagnostics_t, time_usec) }, \
         { "model_residual", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_merivus_ftc_diagnostics_t, model_residual) }, \
         { "excitation", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_merivus_ftc_diagnostics_t, excitation) }, \
         { "maneuver_intensity", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_merivus_ftc_diagnostics_t, maneuver_intensity) }, \
         { "external_disturbance_score", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_merivus_ftc_diagnostics_t, external_disturbance_score) }, \
         { "vibration_score", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_merivus_ftc_diagnostics_t, vibration_score) }, \
         { "allocation_residual_norm", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_merivus_ftc_diagnostics_t, allocation_residual_norm) }, \
         { "attitude_error", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_merivus_ftc_diagnostics_t, attitude_error) }, \
         { "rate_error", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_merivus_ftc_diagnostics_t, rate_error) }, \
         { "jerk", NULL, MAVLINK_TYPE_FLOAT, 0, 40, offsetof(mavlink_merivus_ftc_diagnostics_t, jerk) }, \
         { "acceleration_magnitude", NULL, MAVLINK_TYPE_FLOAT, 0, 44, offsetof(mavlink_merivus_ftc_diagnostics_t, acceleration_magnitude) }, \
         { "angular_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 48, offsetof(mavlink_merivus_ftc_diagnostics_t, angular_rate) }, \
         { "angular_acceleration", NULL, MAVLINK_TYPE_FLOAT, 0, 52, offsetof(mavlink_merivus_ftc_diagnostics_t, angular_acceleration) }, \
         { "system_reason_mask", NULL, MAVLINK_TYPE_UINT32_T, 0, 56, offsetof(mavlink_merivus_ftc_diagnostics_t, system_reason_mask) }, \
         { "protocol_version", NULL, MAVLINK_TYPE_UINT8_T, 0, 60, offsetof(mavlink_merivus_ftc_diagnostics_t, protocol_version) }, \
         { "flags", NULL, MAVLINK_TYPE_UINT8_T, 0, 61, offsetof(mavlink_merivus_ftc_diagnostics_t, flags) }, \
         { "simulation_motor_index", NULL, MAVLINK_TYPE_UINT8_T, 0, 62, offsetof(mavlink_merivus_ftc_diagnostics_t, simulation_motor_index) }, \
         { "simulation_target_effectiveness_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 63, offsetof(mavlink_merivus_ftc_diagnostics_t, simulation_target_effectiveness_pct) }, \
         { "simulation_applied_effectiveness_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 64, offsetof(mavlink_merivus_ftc_diagnostics_t, simulation_applied_effectiveness_pct) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_MERIVUS_FTC_DIAGNOSTICS { \
    "MERIVUS_FTC_DIAGNOSTICS", \
    19, \
    {  { "time_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_merivus_ftc_diagnostics_t, time_usec) }, \
         { "model_residual", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_merivus_ftc_diagnostics_t, model_residual) }, \
         { "excitation", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_merivus_ftc_diagnostics_t, excitation) }, \
         { "maneuver_intensity", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_merivus_ftc_diagnostics_t, maneuver_intensity) }, \
         { "external_disturbance_score", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_merivus_ftc_diagnostics_t, external_disturbance_score) }, \
         { "vibration_score", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_merivus_ftc_diagnostics_t, vibration_score) }, \
         { "allocation_residual_norm", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_merivus_ftc_diagnostics_t, allocation_residual_norm) }, \
         { "attitude_error", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_merivus_ftc_diagnostics_t, attitude_error) }, \
         { "rate_error", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_merivus_ftc_diagnostics_t, rate_error) }, \
         { "jerk", NULL, MAVLINK_TYPE_FLOAT, 0, 40, offsetof(mavlink_merivus_ftc_diagnostics_t, jerk) }, \
         { "acceleration_magnitude", NULL, MAVLINK_TYPE_FLOAT, 0, 44, offsetof(mavlink_merivus_ftc_diagnostics_t, acceleration_magnitude) }, \
         { "angular_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 48, offsetof(mavlink_merivus_ftc_diagnostics_t, angular_rate) }, \
         { "angular_acceleration", NULL, MAVLINK_TYPE_FLOAT, 0, 52, offsetof(mavlink_merivus_ftc_diagnostics_t, angular_acceleration) }, \
         { "system_reason_mask", NULL, MAVLINK_TYPE_UINT32_T, 0, 56, offsetof(mavlink_merivus_ftc_diagnostics_t, system_reason_mask) }, \
         { "protocol_version", NULL, MAVLINK_TYPE_UINT8_T, 0, 60, offsetof(mavlink_merivus_ftc_diagnostics_t, protocol_version) }, \
         { "flags", NULL, MAVLINK_TYPE_UINT8_T, 0, 61, offsetof(mavlink_merivus_ftc_diagnostics_t, flags) }, \
         { "simulation_motor_index", NULL, MAVLINK_TYPE_UINT8_T, 0, 62, offsetof(mavlink_merivus_ftc_diagnostics_t, simulation_motor_index) }, \
         { "simulation_target_effectiveness_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 63, offsetof(mavlink_merivus_ftc_diagnostics_t, simulation_target_effectiveness_pct) }, \
         { "simulation_applied_effectiveness_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 64, offsetof(mavlink_merivus_ftc_diagnostics_t, simulation_applied_effectiveness_pct) }, \
         } \
}
#endif

/**
 * @brief Pack a merivus_ftc_diagnostics message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_usec [us] PX4 monotonic publication timestamp.
 * @param model_residual  Motor observer model residual.
 * @param excitation  Observer excitation score.
 * @param maneuver_intensity  Observed maneuver intensity.
 * @param external_disturbance_score  Observed external-disturbance score.
 * @param vibration_score  Observed vibration score.
 * @param allocation_residual_norm  Shadow allocation residual norm.
 * @param attitude_error [rad] Attitude error magnitude.
 * @param rate_error [rad/s] Angular-rate error magnitude.
 * @param jerk  Jerk magnitude.
 * @param acceleration_magnitude  Acceleration magnitude.
 * @param angular_rate [rad/s] Angular-rate magnitude.
 * @param angular_acceleration  Angular-acceleration magnitude.
 * @param system_reason_mask  Aggregated FTC reason bit mask.
 * @param protocol_version  MERIVUS FTC telemetry contract version.
 * @param flags  Diagnostic validity and simulation flags.
 * @param simulation_motor_index  Injected SITL motor index.
 * @param simulation_target_effectiveness_pct  Requested SITL effectiveness using the compact percentage scaling.
 * @param simulation_applied_effectiveness_pct  Applied SITL effectiveness using the compact percentage scaling.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_merivus_ftc_diagnostics_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint64_t time_usec, float model_residual, float excitation, float maneuver_intensity, float external_disturbance_score, float vibration_score, float allocation_residual_norm, float attitude_error, float rate_error, float jerk, float acceleration_magnitude, float angular_rate, float angular_acceleration, uint32_t system_reason_mask, uint8_t protocol_version, uint8_t flags, uint8_t simulation_motor_index, uint8_t simulation_target_effectiveness_pct, uint8_t simulation_applied_effectiveness_pct)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_float(buf, 8, model_residual);
    _mav_put_float(buf, 12, excitation);
    _mav_put_float(buf, 16, maneuver_intensity);
    _mav_put_float(buf, 20, external_disturbance_score);
    _mav_put_float(buf, 24, vibration_score);
    _mav_put_float(buf, 28, allocation_residual_norm);
    _mav_put_float(buf, 32, attitude_error);
    _mav_put_float(buf, 36, rate_error);
    _mav_put_float(buf, 40, jerk);
    _mav_put_float(buf, 44, acceleration_magnitude);
    _mav_put_float(buf, 48, angular_rate);
    _mav_put_float(buf, 52, angular_acceleration);
    _mav_put_uint32_t(buf, 56, system_reason_mask);
    _mav_put_uint8_t(buf, 60, protocol_version);
    _mav_put_uint8_t(buf, 61, flags);
    _mav_put_uint8_t(buf, 62, simulation_motor_index);
    _mav_put_uint8_t(buf, 63, simulation_target_effectiveness_pct);
    _mav_put_uint8_t(buf, 64, simulation_applied_effectiveness_pct);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN);
#else
    mavlink_merivus_ftc_diagnostics_t packet;
    packet.time_usec = time_usec;
    packet.model_residual = model_residual;
    packet.excitation = excitation;
    packet.maneuver_intensity = maneuver_intensity;
    packet.external_disturbance_score = external_disturbance_score;
    packet.vibration_score = vibration_score;
    packet.allocation_residual_norm = allocation_residual_norm;
    packet.attitude_error = attitude_error;
    packet.rate_error = rate_error;
    packet.jerk = jerk;
    packet.acceleration_magnitude = acceleration_magnitude;
    packet.angular_rate = angular_rate;
    packet.angular_acceleration = angular_acceleration;
    packet.system_reason_mask = system_reason_mask;
    packet.protocol_version = protocol_version;
    packet.flags = flags;
    packet.simulation_motor_index = simulation_motor_index;
    packet.simulation_target_effectiveness_pct = simulation_target_effectiveness_pct;
    packet.simulation_applied_effectiveness_pct = simulation_applied_effectiveness_pct;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_CRC);
}

/**
 * @brief Pack a merivus_ftc_diagnostics message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_usec [us] PX4 monotonic publication timestamp.
 * @param model_residual  Motor observer model residual.
 * @param excitation  Observer excitation score.
 * @param maneuver_intensity  Observed maneuver intensity.
 * @param external_disturbance_score  Observed external-disturbance score.
 * @param vibration_score  Observed vibration score.
 * @param allocation_residual_norm  Shadow allocation residual norm.
 * @param attitude_error [rad] Attitude error magnitude.
 * @param rate_error [rad/s] Angular-rate error magnitude.
 * @param jerk  Jerk magnitude.
 * @param acceleration_magnitude  Acceleration magnitude.
 * @param angular_rate [rad/s] Angular-rate magnitude.
 * @param angular_acceleration  Angular-acceleration magnitude.
 * @param system_reason_mask  Aggregated FTC reason bit mask.
 * @param protocol_version  MERIVUS FTC telemetry contract version.
 * @param flags  Diagnostic validity and simulation flags.
 * @param simulation_motor_index  Injected SITL motor index.
 * @param simulation_target_effectiveness_pct  Requested SITL effectiveness using the compact percentage scaling.
 * @param simulation_applied_effectiveness_pct  Applied SITL effectiveness using the compact percentage scaling.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_merivus_ftc_diagnostics_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint64_t time_usec,float model_residual,float excitation,float maneuver_intensity,float external_disturbance_score,float vibration_score,float allocation_residual_norm,float attitude_error,float rate_error,float jerk,float acceleration_magnitude,float angular_rate,float angular_acceleration,uint32_t system_reason_mask,uint8_t protocol_version,uint8_t flags,uint8_t simulation_motor_index,uint8_t simulation_target_effectiveness_pct,uint8_t simulation_applied_effectiveness_pct)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_float(buf, 8, model_residual);
    _mav_put_float(buf, 12, excitation);
    _mav_put_float(buf, 16, maneuver_intensity);
    _mav_put_float(buf, 20, external_disturbance_score);
    _mav_put_float(buf, 24, vibration_score);
    _mav_put_float(buf, 28, allocation_residual_norm);
    _mav_put_float(buf, 32, attitude_error);
    _mav_put_float(buf, 36, rate_error);
    _mav_put_float(buf, 40, jerk);
    _mav_put_float(buf, 44, acceleration_magnitude);
    _mav_put_float(buf, 48, angular_rate);
    _mav_put_float(buf, 52, angular_acceleration);
    _mav_put_uint32_t(buf, 56, system_reason_mask);
    _mav_put_uint8_t(buf, 60, protocol_version);
    _mav_put_uint8_t(buf, 61, flags);
    _mav_put_uint8_t(buf, 62, simulation_motor_index);
    _mav_put_uint8_t(buf, 63, simulation_target_effectiveness_pct);
    _mav_put_uint8_t(buf, 64, simulation_applied_effectiveness_pct);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN);
#else
    mavlink_merivus_ftc_diagnostics_t packet;
    packet.time_usec = time_usec;
    packet.model_residual = model_residual;
    packet.excitation = excitation;
    packet.maneuver_intensity = maneuver_intensity;
    packet.external_disturbance_score = external_disturbance_score;
    packet.vibration_score = vibration_score;
    packet.allocation_residual_norm = allocation_residual_norm;
    packet.attitude_error = attitude_error;
    packet.rate_error = rate_error;
    packet.jerk = jerk;
    packet.acceleration_magnitude = acceleration_magnitude;
    packet.angular_rate = angular_rate;
    packet.angular_acceleration = angular_acceleration;
    packet.system_reason_mask = system_reason_mask;
    packet.protocol_version = protocol_version;
    packet.flags = flags;
    packet.simulation_motor_index = simulation_motor_index;
    packet.simulation_target_effectiveness_pct = simulation_target_effectiveness_pct;
    packet.simulation_applied_effectiveness_pct = simulation_applied_effectiveness_pct;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_CRC);
}

/**
 * @brief Encode a merivus_ftc_diagnostics struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param merivus_ftc_diagnostics C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_merivus_ftc_diagnostics_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_merivus_ftc_diagnostics_t* merivus_ftc_diagnostics)
{
    return mavlink_msg_merivus_ftc_diagnostics_pack(system_id, component_id, msg, merivus_ftc_diagnostics->time_usec, merivus_ftc_diagnostics->model_residual, merivus_ftc_diagnostics->excitation, merivus_ftc_diagnostics->maneuver_intensity, merivus_ftc_diagnostics->external_disturbance_score, merivus_ftc_diagnostics->vibration_score, merivus_ftc_diagnostics->allocation_residual_norm, merivus_ftc_diagnostics->attitude_error, merivus_ftc_diagnostics->rate_error, merivus_ftc_diagnostics->jerk, merivus_ftc_diagnostics->acceleration_magnitude, merivus_ftc_diagnostics->angular_rate, merivus_ftc_diagnostics->angular_acceleration, merivus_ftc_diagnostics->system_reason_mask, merivus_ftc_diagnostics->protocol_version, merivus_ftc_diagnostics->flags, merivus_ftc_diagnostics->simulation_motor_index, merivus_ftc_diagnostics->simulation_target_effectiveness_pct, merivus_ftc_diagnostics->simulation_applied_effectiveness_pct);
}

/**
 * @brief Encode a merivus_ftc_diagnostics struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param merivus_ftc_diagnostics C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_merivus_ftc_diagnostics_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_merivus_ftc_diagnostics_t* merivus_ftc_diagnostics)
{
    return mavlink_msg_merivus_ftc_diagnostics_pack_chan(system_id, component_id, chan, msg, merivus_ftc_diagnostics->time_usec, merivus_ftc_diagnostics->model_residual, merivus_ftc_diagnostics->excitation, merivus_ftc_diagnostics->maneuver_intensity, merivus_ftc_diagnostics->external_disturbance_score, merivus_ftc_diagnostics->vibration_score, merivus_ftc_diagnostics->allocation_residual_norm, merivus_ftc_diagnostics->attitude_error, merivus_ftc_diagnostics->rate_error, merivus_ftc_diagnostics->jerk, merivus_ftc_diagnostics->acceleration_magnitude, merivus_ftc_diagnostics->angular_rate, merivus_ftc_diagnostics->angular_acceleration, merivus_ftc_diagnostics->system_reason_mask, merivus_ftc_diagnostics->protocol_version, merivus_ftc_diagnostics->flags, merivus_ftc_diagnostics->simulation_motor_index, merivus_ftc_diagnostics->simulation_target_effectiveness_pct, merivus_ftc_diagnostics->simulation_applied_effectiveness_pct);
}

/**
 * @brief Send a merivus_ftc_diagnostics message
 * @param chan MAVLink channel to send the message
 *
 * @param time_usec [us] PX4 monotonic publication timestamp.
 * @param model_residual  Motor observer model residual.
 * @param excitation  Observer excitation score.
 * @param maneuver_intensity  Observed maneuver intensity.
 * @param external_disturbance_score  Observed external-disturbance score.
 * @param vibration_score  Observed vibration score.
 * @param allocation_residual_norm  Shadow allocation residual norm.
 * @param attitude_error [rad] Attitude error magnitude.
 * @param rate_error [rad/s] Angular-rate error magnitude.
 * @param jerk  Jerk magnitude.
 * @param acceleration_magnitude  Acceleration magnitude.
 * @param angular_rate [rad/s] Angular-rate magnitude.
 * @param angular_acceleration  Angular-acceleration magnitude.
 * @param system_reason_mask  Aggregated FTC reason bit mask.
 * @param protocol_version  MERIVUS FTC telemetry contract version.
 * @param flags  Diagnostic validity and simulation flags.
 * @param simulation_motor_index  Injected SITL motor index.
 * @param simulation_target_effectiveness_pct  Requested SITL effectiveness using the compact percentage scaling.
 * @param simulation_applied_effectiveness_pct  Applied SITL effectiveness using the compact percentage scaling.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_merivus_ftc_diagnostics_send(mavlink_channel_t chan, uint64_t time_usec, float model_residual, float excitation, float maneuver_intensity, float external_disturbance_score, float vibration_score, float allocation_residual_norm, float attitude_error, float rate_error, float jerk, float acceleration_magnitude, float angular_rate, float angular_acceleration, uint32_t system_reason_mask, uint8_t protocol_version, uint8_t flags, uint8_t simulation_motor_index, uint8_t simulation_target_effectiveness_pct, uint8_t simulation_applied_effectiveness_pct)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_float(buf, 8, model_residual);
    _mav_put_float(buf, 12, excitation);
    _mav_put_float(buf, 16, maneuver_intensity);
    _mav_put_float(buf, 20, external_disturbance_score);
    _mav_put_float(buf, 24, vibration_score);
    _mav_put_float(buf, 28, allocation_residual_norm);
    _mav_put_float(buf, 32, attitude_error);
    _mav_put_float(buf, 36, rate_error);
    _mav_put_float(buf, 40, jerk);
    _mav_put_float(buf, 44, acceleration_magnitude);
    _mav_put_float(buf, 48, angular_rate);
    _mav_put_float(buf, 52, angular_acceleration);
    _mav_put_uint32_t(buf, 56, system_reason_mask);
    _mav_put_uint8_t(buf, 60, protocol_version);
    _mav_put_uint8_t(buf, 61, flags);
    _mav_put_uint8_t(buf, 62, simulation_motor_index);
    _mav_put_uint8_t(buf, 63, simulation_target_effectiveness_pct);
    _mav_put_uint8_t(buf, 64, simulation_applied_effectiveness_pct);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS, buf, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_CRC);
#else
    mavlink_merivus_ftc_diagnostics_t packet;
    packet.time_usec = time_usec;
    packet.model_residual = model_residual;
    packet.excitation = excitation;
    packet.maneuver_intensity = maneuver_intensity;
    packet.external_disturbance_score = external_disturbance_score;
    packet.vibration_score = vibration_score;
    packet.allocation_residual_norm = allocation_residual_norm;
    packet.attitude_error = attitude_error;
    packet.rate_error = rate_error;
    packet.jerk = jerk;
    packet.acceleration_magnitude = acceleration_magnitude;
    packet.angular_rate = angular_rate;
    packet.angular_acceleration = angular_acceleration;
    packet.system_reason_mask = system_reason_mask;
    packet.protocol_version = protocol_version;
    packet.flags = flags;
    packet.simulation_motor_index = simulation_motor_index;
    packet.simulation_target_effectiveness_pct = simulation_target_effectiveness_pct;
    packet.simulation_applied_effectiveness_pct = simulation_applied_effectiveness_pct;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS, (const char *)&packet, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_CRC);
#endif
}

/**
 * @brief Send a merivus_ftc_diagnostics message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_merivus_ftc_diagnostics_send_struct(mavlink_channel_t chan, const mavlink_merivus_ftc_diagnostics_t* merivus_ftc_diagnostics)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_merivus_ftc_diagnostics_send(chan, merivus_ftc_diagnostics->time_usec, merivus_ftc_diagnostics->model_residual, merivus_ftc_diagnostics->excitation, merivus_ftc_diagnostics->maneuver_intensity, merivus_ftc_diagnostics->external_disturbance_score, merivus_ftc_diagnostics->vibration_score, merivus_ftc_diagnostics->allocation_residual_norm, merivus_ftc_diagnostics->attitude_error, merivus_ftc_diagnostics->rate_error, merivus_ftc_diagnostics->jerk, merivus_ftc_diagnostics->acceleration_magnitude, merivus_ftc_diagnostics->angular_rate, merivus_ftc_diagnostics->angular_acceleration, merivus_ftc_diagnostics->system_reason_mask, merivus_ftc_diagnostics->protocol_version, merivus_ftc_diagnostics->flags, merivus_ftc_diagnostics->simulation_motor_index, merivus_ftc_diagnostics->simulation_target_effectiveness_pct, merivus_ftc_diagnostics->simulation_applied_effectiveness_pct);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS, (const char *)merivus_ftc_diagnostics, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_CRC);
#endif
}

#if MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_merivus_ftc_diagnostics_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint64_t time_usec, float model_residual, float excitation, float maneuver_intensity, float external_disturbance_score, float vibration_score, float allocation_residual_norm, float attitude_error, float rate_error, float jerk, float acceleration_magnitude, float angular_rate, float angular_acceleration, uint32_t system_reason_mask, uint8_t protocol_version, uint8_t flags, uint8_t simulation_motor_index, uint8_t simulation_target_effectiveness_pct, uint8_t simulation_applied_effectiveness_pct)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_float(buf, 8, model_residual);
    _mav_put_float(buf, 12, excitation);
    _mav_put_float(buf, 16, maneuver_intensity);
    _mav_put_float(buf, 20, external_disturbance_score);
    _mav_put_float(buf, 24, vibration_score);
    _mav_put_float(buf, 28, allocation_residual_norm);
    _mav_put_float(buf, 32, attitude_error);
    _mav_put_float(buf, 36, rate_error);
    _mav_put_float(buf, 40, jerk);
    _mav_put_float(buf, 44, acceleration_magnitude);
    _mav_put_float(buf, 48, angular_rate);
    _mav_put_float(buf, 52, angular_acceleration);
    _mav_put_uint32_t(buf, 56, system_reason_mask);
    _mav_put_uint8_t(buf, 60, protocol_version);
    _mav_put_uint8_t(buf, 61, flags);
    _mav_put_uint8_t(buf, 62, simulation_motor_index);
    _mav_put_uint8_t(buf, 63, simulation_target_effectiveness_pct);
    _mav_put_uint8_t(buf, 64, simulation_applied_effectiveness_pct);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS, buf, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_CRC);
#else
    mavlink_merivus_ftc_diagnostics_t *packet = (mavlink_merivus_ftc_diagnostics_t *)msgbuf;
    packet->time_usec = time_usec;
    packet->model_residual = model_residual;
    packet->excitation = excitation;
    packet->maneuver_intensity = maneuver_intensity;
    packet->external_disturbance_score = external_disturbance_score;
    packet->vibration_score = vibration_score;
    packet->allocation_residual_norm = allocation_residual_norm;
    packet->attitude_error = attitude_error;
    packet->rate_error = rate_error;
    packet->jerk = jerk;
    packet->acceleration_magnitude = acceleration_magnitude;
    packet->angular_rate = angular_rate;
    packet->angular_acceleration = angular_acceleration;
    packet->system_reason_mask = system_reason_mask;
    packet->protocol_version = protocol_version;
    packet->flags = flags;
    packet->simulation_motor_index = simulation_motor_index;
    packet->simulation_target_effectiveness_pct = simulation_target_effectiveness_pct;
    packet->simulation_applied_effectiveness_pct = simulation_applied_effectiveness_pct;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS, (const char *)packet, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_CRC);
#endif
}
#endif

#endif

// MESSAGE MERIVUS_FTC_DIAGNOSTICS UNPACKING


/**
 * @brief Get field time_usec from merivus_ftc_diagnostics message
 *
 * @return [us] PX4 monotonic publication timestamp.
 */
static inline uint64_t mavlink_msg_merivus_ftc_diagnostics_get_time_usec(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field model_residual from merivus_ftc_diagnostics message
 *
 * @return  Motor observer model residual.
 */
static inline float mavlink_msg_merivus_ftc_diagnostics_get_model_residual(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field excitation from merivus_ftc_diagnostics message
 *
 * @return  Observer excitation score.
 */
static inline float mavlink_msg_merivus_ftc_diagnostics_get_excitation(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field maneuver_intensity from merivus_ftc_diagnostics message
 *
 * @return  Observed maneuver intensity.
 */
static inline float mavlink_msg_merivus_ftc_diagnostics_get_maneuver_intensity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field external_disturbance_score from merivus_ftc_diagnostics message
 *
 * @return  Observed external-disturbance score.
 */
static inline float mavlink_msg_merivus_ftc_diagnostics_get_external_disturbance_score(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field vibration_score from merivus_ftc_diagnostics message
 *
 * @return  Observed vibration score.
 */
static inline float mavlink_msg_merivus_ftc_diagnostics_get_vibration_score(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field allocation_residual_norm from merivus_ftc_diagnostics message
 *
 * @return  Shadow allocation residual norm.
 */
static inline float mavlink_msg_merivus_ftc_diagnostics_get_allocation_residual_norm(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field attitude_error from merivus_ftc_diagnostics message
 *
 * @return [rad] Attitude error magnitude.
 */
static inline float mavlink_msg_merivus_ftc_diagnostics_get_attitude_error(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Get field rate_error from merivus_ftc_diagnostics message
 *
 * @return [rad/s] Angular-rate error magnitude.
 */
static inline float mavlink_msg_merivus_ftc_diagnostics_get_rate_error(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  36);
}

/**
 * @brief Get field jerk from merivus_ftc_diagnostics message
 *
 * @return  Jerk magnitude.
 */
static inline float mavlink_msg_merivus_ftc_diagnostics_get_jerk(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  40);
}

/**
 * @brief Get field acceleration_magnitude from merivus_ftc_diagnostics message
 *
 * @return  Acceleration magnitude.
 */
static inline float mavlink_msg_merivus_ftc_diagnostics_get_acceleration_magnitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  44);
}

/**
 * @brief Get field angular_rate from merivus_ftc_diagnostics message
 *
 * @return [rad/s] Angular-rate magnitude.
 */
static inline float mavlink_msg_merivus_ftc_diagnostics_get_angular_rate(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  48);
}

/**
 * @brief Get field angular_acceleration from merivus_ftc_diagnostics message
 *
 * @return  Angular-acceleration magnitude.
 */
static inline float mavlink_msg_merivus_ftc_diagnostics_get_angular_acceleration(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  52);
}

/**
 * @brief Get field system_reason_mask from merivus_ftc_diagnostics message
 *
 * @return  Aggregated FTC reason bit mask.
 */
static inline uint32_t mavlink_msg_merivus_ftc_diagnostics_get_system_reason_mask(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  56);
}

/**
 * @brief Get field protocol_version from merivus_ftc_diagnostics message
 *
 * @return  MERIVUS FTC telemetry contract version.
 */
static inline uint8_t mavlink_msg_merivus_ftc_diagnostics_get_protocol_version(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  60);
}

/**
 * @brief Get field flags from merivus_ftc_diagnostics message
 *
 * @return  Diagnostic validity and simulation flags.
 */
static inline uint8_t mavlink_msg_merivus_ftc_diagnostics_get_flags(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  61);
}

/**
 * @brief Get field simulation_motor_index from merivus_ftc_diagnostics message
 *
 * @return  Injected SITL motor index.
 */
static inline uint8_t mavlink_msg_merivus_ftc_diagnostics_get_simulation_motor_index(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  62);
}

/**
 * @brief Get field simulation_target_effectiveness_pct from merivus_ftc_diagnostics message
 *
 * @return  Requested SITL effectiveness using the compact percentage scaling.
 */
static inline uint8_t mavlink_msg_merivus_ftc_diagnostics_get_simulation_target_effectiveness_pct(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  63);
}

/**
 * @brief Get field simulation_applied_effectiveness_pct from merivus_ftc_diagnostics message
 *
 * @return  Applied SITL effectiveness using the compact percentage scaling.
 */
static inline uint8_t mavlink_msg_merivus_ftc_diagnostics_get_simulation_applied_effectiveness_pct(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  64);
}

/**
 * @brief Decode a merivus_ftc_diagnostics message into a struct
 *
 * @param msg The message to decode
 * @param merivus_ftc_diagnostics C-struct to decode the message contents into
 */
static inline void mavlink_msg_merivus_ftc_diagnostics_decode(const mavlink_message_t* msg, mavlink_merivus_ftc_diagnostics_t* merivus_ftc_diagnostics)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    merivus_ftc_diagnostics->time_usec = mavlink_msg_merivus_ftc_diagnostics_get_time_usec(msg);
    merivus_ftc_diagnostics->model_residual = mavlink_msg_merivus_ftc_diagnostics_get_model_residual(msg);
    merivus_ftc_diagnostics->excitation = mavlink_msg_merivus_ftc_diagnostics_get_excitation(msg);
    merivus_ftc_diagnostics->maneuver_intensity = mavlink_msg_merivus_ftc_diagnostics_get_maneuver_intensity(msg);
    merivus_ftc_diagnostics->external_disturbance_score = mavlink_msg_merivus_ftc_diagnostics_get_external_disturbance_score(msg);
    merivus_ftc_diagnostics->vibration_score = mavlink_msg_merivus_ftc_diagnostics_get_vibration_score(msg);
    merivus_ftc_diagnostics->allocation_residual_norm = mavlink_msg_merivus_ftc_diagnostics_get_allocation_residual_norm(msg);
    merivus_ftc_diagnostics->attitude_error = mavlink_msg_merivus_ftc_diagnostics_get_attitude_error(msg);
    merivus_ftc_diagnostics->rate_error = mavlink_msg_merivus_ftc_diagnostics_get_rate_error(msg);
    merivus_ftc_diagnostics->jerk = mavlink_msg_merivus_ftc_diagnostics_get_jerk(msg);
    merivus_ftc_diagnostics->acceleration_magnitude = mavlink_msg_merivus_ftc_diagnostics_get_acceleration_magnitude(msg);
    merivus_ftc_diagnostics->angular_rate = mavlink_msg_merivus_ftc_diagnostics_get_angular_rate(msg);
    merivus_ftc_diagnostics->angular_acceleration = mavlink_msg_merivus_ftc_diagnostics_get_angular_acceleration(msg);
    merivus_ftc_diagnostics->system_reason_mask = mavlink_msg_merivus_ftc_diagnostics_get_system_reason_mask(msg);
    merivus_ftc_diagnostics->protocol_version = mavlink_msg_merivus_ftc_diagnostics_get_protocol_version(msg);
    merivus_ftc_diagnostics->flags = mavlink_msg_merivus_ftc_diagnostics_get_flags(msg);
    merivus_ftc_diagnostics->simulation_motor_index = mavlink_msg_merivus_ftc_diagnostics_get_simulation_motor_index(msg);
    merivus_ftc_diagnostics->simulation_target_effectiveness_pct = mavlink_msg_merivus_ftc_diagnostics_get_simulation_target_effectiveness_pct(msg);
    merivus_ftc_diagnostics->simulation_applied_effectiveness_pct = mavlink_msg_merivus_ftc_diagnostics_get_simulation_applied_effectiveness_pct(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN? msg->len : MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN;
        memset(merivus_ftc_diagnostics, 0, MAVLINK_MSG_ID_MERIVUS_FTC_DIAGNOSTICS_LEN);
    memcpy(merivus_ftc_diagnostics, _MAV_PAYLOAD(msg), len);
#endif
}
