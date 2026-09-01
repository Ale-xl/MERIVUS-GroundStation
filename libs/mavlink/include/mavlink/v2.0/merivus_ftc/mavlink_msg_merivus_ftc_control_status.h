#pragma once
// MESSAGE MERIVUS_FTC_CONTROL_STATUS PACKING

#define MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS 60001


typedef struct __mavlink_merivus_ftc_control_status_t {
 uint64_t time_usec; /*< [us] PX4 monotonic publication timestamp.*/
 uint16_t saturated_mask; /*<  Actuator saturation bit mask.*/
 uint8_t protocol_version; /*<  MERIVUS FTC telemetry contract version.*/
 uint8_t system_state; /*<  Aggregated subsystem state.*/
 uint8_t authority_state; /*<  Remaining control-authority state.*/
 uint8_t control_mode; /*<  Highest represented integration stage.*/
 uint8_t recovery_state; /*<  Recovery-candidate state.*/
 uint8_t flags; /*<  Validity and integration flags.*/
 uint8_t roll_authority_pct; /*<  Remaining roll authority.*/
 uint8_t pitch_authority_pct; /*<  Remaining pitch authority.*/
 uint8_t yaw_authority_pct; /*<  Remaining yaw authority.*/
 uint8_t thrust_authority_pct; /*<  Remaining thrust authority.*/
 uint8_t minimum_attitude_authority_pct; /*<  Minimum attitude-axis authority.*/
 uint8_t actuator_headroom_pct; /*<  Remaining actuator headroom.*/
 uint8_t system_confidence_pct; /*<  Aggregated FTC confidence.*/
 uint8_t recovery_progress_pct; /*<  Recovery-candidate state progress.*/
} mavlink_merivus_ftc_control_status_t;

#define MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN 24
#define MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_MIN_LEN 24
#define MAVLINK_MSG_ID_60001_LEN 24
#define MAVLINK_MSG_ID_60001_MIN_LEN 24

#define MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_CRC 153
#define MAVLINK_MSG_ID_60001_CRC 153



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_MERIVUS_FTC_CONTROL_STATUS { \
    60001, \
    "MERIVUS_FTC_CONTROL_STATUS", \
    16, \
    {  { "time_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_merivus_ftc_control_status_t, time_usec) }, \
         { "saturated_mask", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_merivus_ftc_control_status_t, saturated_mask) }, \
         { "protocol_version", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_merivus_ftc_control_status_t, protocol_version) }, \
         { "system_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_merivus_ftc_control_status_t, system_state) }, \
         { "authority_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_merivus_ftc_control_status_t, authority_state) }, \
         { "control_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_merivus_ftc_control_status_t, control_mode) }, \
         { "recovery_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 14, offsetof(mavlink_merivus_ftc_control_status_t, recovery_state) }, \
         { "flags", NULL, MAVLINK_TYPE_UINT8_T, 0, 15, offsetof(mavlink_merivus_ftc_control_status_t, flags) }, \
         { "roll_authority_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_merivus_ftc_control_status_t, roll_authority_pct) }, \
         { "pitch_authority_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_merivus_ftc_control_status_t, pitch_authority_pct) }, \
         { "yaw_authority_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_merivus_ftc_control_status_t, yaw_authority_pct) }, \
         { "thrust_authority_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_merivus_ftc_control_status_t, thrust_authority_pct) }, \
         { "minimum_attitude_authority_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_merivus_ftc_control_status_t, minimum_attitude_authority_pct) }, \
         { "actuator_headroom_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_merivus_ftc_control_status_t, actuator_headroom_pct) }, \
         { "system_confidence_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_merivus_ftc_control_status_t, system_confidence_pct) }, \
         { "recovery_progress_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 23, offsetof(mavlink_merivus_ftc_control_status_t, recovery_progress_pct) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_MERIVUS_FTC_CONTROL_STATUS { \
    "MERIVUS_FTC_CONTROL_STATUS", \
    16, \
    {  { "time_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_merivus_ftc_control_status_t, time_usec) }, \
         { "saturated_mask", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_merivus_ftc_control_status_t, saturated_mask) }, \
         { "protocol_version", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_merivus_ftc_control_status_t, protocol_version) }, \
         { "system_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_merivus_ftc_control_status_t, system_state) }, \
         { "authority_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_merivus_ftc_control_status_t, authority_state) }, \
         { "control_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_merivus_ftc_control_status_t, control_mode) }, \
         { "recovery_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 14, offsetof(mavlink_merivus_ftc_control_status_t, recovery_state) }, \
         { "flags", NULL, MAVLINK_TYPE_UINT8_T, 0, 15, offsetof(mavlink_merivus_ftc_control_status_t, flags) }, \
         { "roll_authority_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_merivus_ftc_control_status_t, roll_authority_pct) }, \
         { "pitch_authority_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_merivus_ftc_control_status_t, pitch_authority_pct) }, \
         { "yaw_authority_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_merivus_ftc_control_status_t, yaw_authority_pct) }, \
         { "thrust_authority_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_merivus_ftc_control_status_t, thrust_authority_pct) }, \
         { "minimum_attitude_authority_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_merivus_ftc_control_status_t, minimum_attitude_authority_pct) }, \
         { "actuator_headroom_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_merivus_ftc_control_status_t, actuator_headroom_pct) }, \
         { "system_confidence_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_merivus_ftc_control_status_t, system_confidence_pct) }, \
         { "recovery_progress_pct", NULL, MAVLINK_TYPE_UINT8_T, 0, 23, offsetof(mavlink_merivus_ftc_control_status_t, recovery_progress_pct) }, \
         } \
}
#endif

/**
 * @brief Pack a merivus_ftc_control_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_usec [us] PX4 monotonic publication timestamp.
 * @param saturated_mask  Actuator saturation bit mask.
 * @param protocol_version  MERIVUS FTC telemetry contract version.
 * @param system_state  Aggregated subsystem state.
 * @param authority_state  Remaining control-authority state.
 * @param control_mode  Highest represented integration stage.
 * @param recovery_state  Recovery-candidate state.
 * @param flags  Validity and integration flags.
 * @param roll_authority_pct  Remaining roll authority.
 * @param pitch_authority_pct  Remaining pitch authority.
 * @param yaw_authority_pct  Remaining yaw authority.
 * @param thrust_authority_pct  Remaining thrust authority.
 * @param minimum_attitude_authority_pct  Minimum attitude-axis authority.
 * @param actuator_headroom_pct  Remaining actuator headroom.
 * @param system_confidence_pct  Aggregated FTC confidence.
 * @param recovery_progress_pct  Recovery-candidate state progress.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_merivus_ftc_control_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint64_t time_usec, uint16_t saturated_mask, uint8_t protocol_version, uint8_t system_state, uint8_t authority_state, uint8_t control_mode, uint8_t recovery_state, uint8_t flags, uint8_t roll_authority_pct, uint8_t pitch_authority_pct, uint8_t yaw_authority_pct, uint8_t thrust_authority_pct, uint8_t minimum_attitude_authority_pct, uint8_t actuator_headroom_pct, uint8_t system_confidence_pct, uint8_t recovery_progress_pct)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_uint16_t(buf, 8, saturated_mask);
    _mav_put_uint8_t(buf, 10, protocol_version);
    _mav_put_uint8_t(buf, 11, system_state);
    _mav_put_uint8_t(buf, 12, authority_state);
    _mav_put_uint8_t(buf, 13, control_mode);
    _mav_put_uint8_t(buf, 14, recovery_state);
    _mav_put_uint8_t(buf, 15, flags);
    _mav_put_uint8_t(buf, 16, roll_authority_pct);
    _mav_put_uint8_t(buf, 17, pitch_authority_pct);
    _mav_put_uint8_t(buf, 18, yaw_authority_pct);
    _mav_put_uint8_t(buf, 19, thrust_authority_pct);
    _mav_put_uint8_t(buf, 20, minimum_attitude_authority_pct);
    _mav_put_uint8_t(buf, 21, actuator_headroom_pct);
    _mav_put_uint8_t(buf, 22, system_confidence_pct);
    _mav_put_uint8_t(buf, 23, recovery_progress_pct);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN);
#else
    mavlink_merivus_ftc_control_status_t packet;
    packet.time_usec = time_usec;
    packet.saturated_mask = saturated_mask;
    packet.protocol_version = protocol_version;
    packet.system_state = system_state;
    packet.authority_state = authority_state;
    packet.control_mode = control_mode;
    packet.recovery_state = recovery_state;
    packet.flags = flags;
    packet.roll_authority_pct = roll_authority_pct;
    packet.pitch_authority_pct = pitch_authority_pct;
    packet.yaw_authority_pct = yaw_authority_pct;
    packet.thrust_authority_pct = thrust_authority_pct;
    packet.minimum_attitude_authority_pct = minimum_attitude_authority_pct;
    packet.actuator_headroom_pct = actuator_headroom_pct;
    packet.system_confidence_pct = system_confidence_pct;
    packet.recovery_progress_pct = recovery_progress_pct;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_CRC);
}

/**
 * @brief Pack a merivus_ftc_control_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_usec [us] PX4 monotonic publication timestamp.
 * @param saturated_mask  Actuator saturation bit mask.
 * @param protocol_version  MERIVUS FTC telemetry contract version.
 * @param system_state  Aggregated subsystem state.
 * @param authority_state  Remaining control-authority state.
 * @param control_mode  Highest represented integration stage.
 * @param recovery_state  Recovery-candidate state.
 * @param flags  Validity and integration flags.
 * @param roll_authority_pct  Remaining roll authority.
 * @param pitch_authority_pct  Remaining pitch authority.
 * @param yaw_authority_pct  Remaining yaw authority.
 * @param thrust_authority_pct  Remaining thrust authority.
 * @param minimum_attitude_authority_pct  Minimum attitude-axis authority.
 * @param actuator_headroom_pct  Remaining actuator headroom.
 * @param system_confidence_pct  Aggregated FTC confidence.
 * @param recovery_progress_pct  Recovery-candidate state progress.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_merivus_ftc_control_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint64_t time_usec,uint16_t saturated_mask,uint8_t protocol_version,uint8_t system_state,uint8_t authority_state,uint8_t control_mode,uint8_t recovery_state,uint8_t flags,uint8_t roll_authority_pct,uint8_t pitch_authority_pct,uint8_t yaw_authority_pct,uint8_t thrust_authority_pct,uint8_t minimum_attitude_authority_pct,uint8_t actuator_headroom_pct,uint8_t system_confidence_pct,uint8_t recovery_progress_pct)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_uint16_t(buf, 8, saturated_mask);
    _mav_put_uint8_t(buf, 10, protocol_version);
    _mav_put_uint8_t(buf, 11, system_state);
    _mav_put_uint8_t(buf, 12, authority_state);
    _mav_put_uint8_t(buf, 13, control_mode);
    _mav_put_uint8_t(buf, 14, recovery_state);
    _mav_put_uint8_t(buf, 15, flags);
    _mav_put_uint8_t(buf, 16, roll_authority_pct);
    _mav_put_uint8_t(buf, 17, pitch_authority_pct);
    _mav_put_uint8_t(buf, 18, yaw_authority_pct);
    _mav_put_uint8_t(buf, 19, thrust_authority_pct);
    _mav_put_uint8_t(buf, 20, minimum_attitude_authority_pct);
    _mav_put_uint8_t(buf, 21, actuator_headroom_pct);
    _mav_put_uint8_t(buf, 22, system_confidence_pct);
    _mav_put_uint8_t(buf, 23, recovery_progress_pct);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN);
#else
    mavlink_merivus_ftc_control_status_t packet;
    packet.time_usec = time_usec;
    packet.saturated_mask = saturated_mask;
    packet.protocol_version = protocol_version;
    packet.system_state = system_state;
    packet.authority_state = authority_state;
    packet.control_mode = control_mode;
    packet.recovery_state = recovery_state;
    packet.flags = flags;
    packet.roll_authority_pct = roll_authority_pct;
    packet.pitch_authority_pct = pitch_authority_pct;
    packet.yaw_authority_pct = yaw_authority_pct;
    packet.thrust_authority_pct = thrust_authority_pct;
    packet.minimum_attitude_authority_pct = minimum_attitude_authority_pct;
    packet.actuator_headroom_pct = actuator_headroom_pct;
    packet.system_confidence_pct = system_confidence_pct;
    packet.recovery_progress_pct = recovery_progress_pct;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_CRC);
}

/**
 * @brief Encode a merivus_ftc_control_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param merivus_ftc_control_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_merivus_ftc_control_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_merivus_ftc_control_status_t* merivus_ftc_control_status)
{
    return mavlink_msg_merivus_ftc_control_status_pack(system_id, component_id, msg, merivus_ftc_control_status->time_usec, merivus_ftc_control_status->saturated_mask, merivus_ftc_control_status->protocol_version, merivus_ftc_control_status->system_state, merivus_ftc_control_status->authority_state, merivus_ftc_control_status->control_mode, merivus_ftc_control_status->recovery_state, merivus_ftc_control_status->flags, merivus_ftc_control_status->roll_authority_pct, merivus_ftc_control_status->pitch_authority_pct, merivus_ftc_control_status->yaw_authority_pct, merivus_ftc_control_status->thrust_authority_pct, merivus_ftc_control_status->minimum_attitude_authority_pct, merivus_ftc_control_status->actuator_headroom_pct, merivus_ftc_control_status->system_confidence_pct, merivus_ftc_control_status->recovery_progress_pct);
}

/**
 * @brief Encode a merivus_ftc_control_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param merivus_ftc_control_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_merivus_ftc_control_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_merivus_ftc_control_status_t* merivus_ftc_control_status)
{
    return mavlink_msg_merivus_ftc_control_status_pack_chan(system_id, component_id, chan, msg, merivus_ftc_control_status->time_usec, merivus_ftc_control_status->saturated_mask, merivus_ftc_control_status->protocol_version, merivus_ftc_control_status->system_state, merivus_ftc_control_status->authority_state, merivus_ftc_control_status->control_mode, merivus_ftc_control_status->recovery_state, merivus_ftc_control_status->flags, merivus_ftc_control_status->roll_authority_pct, merivus_ftc_control_status->pitch_authority_pct, merivus_ftc_control_status->yaw_authority_pct, merivus_ftc_control_status->thrust_authority_pct, merivus_ftc_control_status->minimum_attitude_authority_pct, merivus_ftc_control_status->actuator_headroom_pct, merivus_ftc_control_status->system_confidence_pct, merivus_ftc_control_status->recovery_progress_pct);
}

/**
 * @brief Send a merivus_ftc_control_status message
 * @param chan MAVLink channel to send the message
 *
 * @param time_usec [us] PX4 monotonic publication timestamp.
 * @param saturated_mask  Actuator saturation bit mask.
 * @param protocol_version  MERIVUS FTC telemetry contract version.
 * @param system_state  Aggregated subsystem state.
 * @param authority_state  Remaining control-authority state.
 * @param control_mode  Highest represented integration stage.
 * @param recovery_state  Recovery-candidate state.
 * @param flags  Validity and integration flags.
 * @param roll_authority_pct  Remaining roll authority.
 * @param pitch_authority_pct  Remaining pitch authority.
 * @param yaw_authority_pct  Remaining yaw authority.
 * @param thrust_authority_pct  Remaining thrust authority.
 * @param minimum_attitude_authority_pct  Minimum attitude-axis authority.
 * @param actuator_headroom_pct  Remaining actuator headroom.
 * @param system_confidence_pct  Aggregated FTC confidence.
 * @param recovery_progress_pct  Recovery-candidate state progress.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_merivus_ftc_control_status_send(mavlink_channel_t chan, uint64_t time_usec, uint16_t saturated_mask, uint8_t protocol_version, uint8_t system_state, uint8_t authority_state, uint8_t control_mode, uint8_t recovery_state, uint8_t flags, uint8_t roll_authority_pct, uint8_t pitch_authority_pct, uint8_t yaw_authority_pct, uint8_t thrust_authority_pct, uint8_t minimum_attitude_authority_pct, uint8_t actuator_headroom_pct, uint8_t system_confidence_pct, uint8_t recovery_progress_pct)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN];
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_uint16_t(buf, 8, saturated_mask);
    _mav_put_uint8_t(buf, 10, protocol_version);
    _mav_put_uint8_t(buf, 11, system_state);
    _mav_put_uint8_t(buf, 12, authority_state);
    _mav_put_uint8_t(buf, 13, control_mode);
    _mav_put_uint8_t(buf, 14, recovery_state);
    _mav_put_uint8_t(buf, 15, flags);
    _mav_put_uint8_t(buf, 16, roll_authority_pct);
    _mav_put_uint8_t(buf, 17, pitch_authority_pct);
    _mav_put_uint8_t(buf, 18, yaw_authority_pct);
    _mav_put_uint8_t(buf, 19, thrust_authority_pct);
    _mav_put_uint8_t(buf, 20, minimum_attitude_authority_pct);
    _mav_put_uint8_t(buf, 21, actuator_headroom_pct);
    _mav_put_uint8_t(buf, 22, system_confidence_pct);
    _mav_put_uint8_t(buf, 23, recovery_progress_pct);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS, buf, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_CRC);
#else
    mavlink_merivus_ftc_control_status_t packet;
    packet.time_usec = time_usec;
    packet.saturated_mask = saturated_mask;
    packet.protocol_version = protocol_version;
    packet.system_state = system_state;
    packet.authority_state = authority_state;
    packet.control_mode = control_mode;
    packet.recovery_state = recovery_state;
    packet.flags = flags;
    packet.roll_authority_pct = roll_authority_pct;
    packet.pitch_authority_pct = pitch_authority_pct;
    packet.yaw_authority_pct = yaw_authority_pct;
    packet.thrust_authority_pct = thrust_authority_pct;
    packet.minimum_attitude_authority_pct = minimum_attitude_authority_pct;
    packet.actuator_headroom_pct = actuator_headroom_pct;
    packet.system_confidence_pct = system_confidence_pct;
    packet.recovery_progress_pct = recovery_progress_pct;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS, (const char *)&packet, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_CRC);
#endif
}

/**
 * @brief Send a merivus_ftc_control_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_merivus_ftc_control_status_send_struct(mavlink_channel_t chan, const mavlink_merivus_ftc_control_status_t* merivus_ftc_control_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_merivus_ftc_control_status_send(chan, merivus_ftc_control_status->time_usec, merivus_ftc_control_status->saturated_mask, merivus_ftc_control_status->protocol_version, merivus_ftc_control_status->system_state, merivus_ftc_control_status->authority_state, merivus_ftc_control_status->control_mode, merivus_ftc_control_status->recovery_state, merivus_ftc_control_status->flags, merivus_ftc_control_status->roll_authority_pct, merivus_ftc_control_status->pitch_authority_pct, merivus_ftc_control_status->yaw_authority_pct, merivus_ftc_control_status->thrust_authority_pct, merivus_ftc_control_status->minimum_attitude_authority_pct, merivus_ftc_control_status->actuator_headroom_pct, merivus_ftc_control_status->system_confidence_pct, merivus_ftc_control_status->recovery_progress_pct);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS, (const char *)merivus_ftc_control_status, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_merivus_ftc_control_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint64_t time_usec, uint16_t saturated_mask, uint8_t protocol_version, uint8_t system_state, uint8_t authority_state, uint8_t control_mode, uint8_t recovery_state, uint8_t flags, uint8_t roll_authority_pct, uint8_t pitch_authority_pct, uint8_t yaw_authority_pct, uint8_t thrust_authority_pct, uint8_t minimum_attitude_authority_pct, uint8_t actuator_headroom_pct, uint8_t system_confidence_pct, uint8_t recovery_progress_pct)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, time_usec);
    _mav_put_uint16_t(buf, 8, saturated_mask);
    _mav_put_uint8_t(buf, 10, protocol_version);
    _mav_put_uint8_t(buf, 11, system_state);
    _mav_put_uint8_t(buf, 12, authority_state);
    _mav_put_uint8_t(buf, 13, control_mode);
    _mav_put_uint8_t(buf, 14, recovery_state);
    _mav_put_uint8_t(buf, 15, flags);
    _mav_put_uint8_t(buf, 16, roll_authority_pct);
    _mav_put_uint8_t(buf, 17, pitch_authority_pct);
    _mav_put_uint8_t(buf, 18, yaw_authority_pct);
    _mav_put_uint8_t(buf, 19, thrust_authority_pct);
    _mav_put_uint8_t(buf, 20, minimum_attitude_authority_pct);
    _mav_put_uint8_t(buf, 21, actuator_headroom_pct);
    _mav_put_uint8_t(buf, 22, system_confidence_pct);
    _mav_put_uint8_t(buf, 23, recovery_progress_pct);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS, buf, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_CRC);
#else
    mavlink_merivus_ftc_control_status_t *packet = (mavlink_merivus_ftc_control_status_t *)msgbuf;
    packet->time_usec = time_usec;
    packet->saturated_mask = saturated_mask;
    packet->protocol_version = protocol_version;
    packet->system_state = system_state;
    packet->authority_state = authority_state;
    packet->control_mode = control_mode;
    packet->recovery_state = recovery_state;
    packet->flags = flags;
    packet->roll_authority_pct = roll_authority_pct;
    packet->pitch_authority_pct = pitch_authority_pct;
    packet->yaw_authority_pct = yaw_authority_pct;
    packet->thrust_authority_pct = thrust_authority_pct;
    packet->minimum_attitude_authority_pct = minimum_attitude_authority_pct;
    packet->actuator_headroom_pct = actuator_headroom_pct;
    packet->system_confidence_pct = system_confidence_pct;
    packet->recovery_progress_pct = recovery_progress_pct;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS, (const char *)packet, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_MIN_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE MERIVUS_FTC_CONTROL_STATUS UNPACKING


/**
 * @brief Get field time_usec from merivus_ftc_control_status message
 *
 * @return [us] PX4 monotonic publication timestamp.
 */
static inline uint64_t mavlink_msg_merivus_ftc_control_status_get_time_usec(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field saturated_mask from merivus_ftc_control_status message
 *
 * @return  Actuator saturation bit mask.
 */
static inline uint16_t mavlink_msg_merivus_ftc_control_status_get_saturated_mask(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  8);
}

/**
 * @brief Get field protocol_version from merivus_ftc_control_status message
 *
 * @return  MERIVUS FTC telemetry contract version.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_protocol_version(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Get field system_state from merivus_ftc_control_status message
 *
 * @return  Aggregated subsystem state.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_system_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  11);
}

/**
 * @brief Get field authority_state from merivus_ftc_control_status message
 *
 * @return  Remaining control-authority state.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_authority_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field control_mode from merivus_ftc_control_status message
 *
 * @return  Highest represented integration stage.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_control_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  13);
}

/**
 * @brief Get field recovery_state from merivus_ftc_control_status message
 *
 * @return  Recovery-candidate state.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_recovery_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  14);
}

/**
 * @brief Get field flags from merivus_ftc_control_status message
 *
 * @return  Validity and integration flags.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_flags(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  15);
}

/**
 * @brief Get field roll_authority_pct from merivus_ftc_control_status message
 *
 * @return  Remaining roll authority.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_roll_authority_pct(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  16);
}

/**
 * @brief Get field pitch_authority_pct from merivus_ftc_control_status message
 *
 * @return  Remaining pitch authority.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_pitch_authority_pct(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  17);
}

/**
 * @brief Get field yaw_authority_pct from merivus_ftc_control_status message
 *
 * @return  Remaining yaw authority.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_yaw_authority_pct(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  18);
}

/**
 * @brief Get field thrust_authority_pct from merivus_ftc_control_status message
 *
 * @return  Remaining thrust authority.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_thrust_authority_pct(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  19);
}

/**
 * @brief Get field minimum_attitude_authority_pct from merivus_ftc_control_status message
 *
 * @return  Minimum attitude-axis authority.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_minimum_attitude_authority_pct(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Get field actuator_headroom_pct from merivus_ftc_control_status message
 *
 * @return  Remaining actuator headroom.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_actuator_headroom_pct(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  21);
}

/**
 * @brief Get field system_confidence_pct from merivus_ftc_control_status message
 *
 * @return  Aggregated FTC confidence.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_system_confidence_pct(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  22);
}

/**
 * @brief Get field recovery_progress_pct from merivus_ftc_control_status message
 *
 * @return  Recovery-candidate state progress.
 */
static inline uint8_t mavlink_msg_merivus_ftc_control_status_get_recovery_progress_pct(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  23);
}

/**
 * @brief Decode a merivus_ftc_control_status message into a struct
 *
 * @param msg The message to decode
 * @param merivus_ftc_control_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_merivus_ftc_control_status_decode(const mavlink_message_t* msg, mavlink_merivus_ftc_control_status_t* merivus_ftc_control_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    merivus_ftc_control_status->time_usec = mavlink_msg_merivus_ftc_control_status_get_time_usec(msg);
    merivus_ftc_control_status->saturated_mask = mavlink_msg_merivus_ftc_control_status_get_saturated_mask(msg);
    merivus_ftc_control_status->protocol_version = mavlink_msg_merivus_ftc_control_status_get_protocol_version(msg);
    merivus_ftc_control_status->system_state = mavlink_msg_merivus_ftc_control_status_get_system_state(msg);
    merivus_ftc_control_status->authority_state = mavlink_msg_merivus_ftc_control_status_get_authority_state(msg);
    merivus_ftc_control_status->control_mode = mavlink_msg_merivus_ftc_control_status_get_control_mode(msg);
    merivus_ftc_control_status->recovery_state = mavlink_msg_merivus_ftc_control_status_get_recovery_state(msg);
    merivus_ftc_control_status->flags = mavlink_msg_merivus_ftc_control_status_get_flags(msg);
    merivus_ftc_control_status->roll_authority_pct = mavlink_msg_merivus_ftc_control_status_get_roll_authority_pct(msg);
    merivus_ftc_control_status->pitch_authority_pct = mavlink_msg_merivus_ftc_control_status_get_pitch_authority_pct(msg);
    merivus_ftc_control_status->yaw_authority_pct = mavlink_msg_merivus_ftc_control_status_get_yaw_authority_pct(msg);
    merivus_ftc_control_status->thrust_authority_pct = mavlink_msg_merivus_ftc_control_status_get_thrust_authority_pct(msg);
    merivus_ftc_control_status->minimum_attitude_authority_pct = mavlink_msg_merivus_ftc_control_status_get_minimum_attitude_authority_pct(msg);
    merivus_ftc_control_status->actuator_headroom_pct = mavlink_msg_merivus_ftc_control_status_get_actuator_headroom_pct(msg);
    merivus_ftc_control_status->system_confidence_pct = mavlink_msg_merivus_ftc_control_status_get_system_confidence_pct(msg);
    merivus_ftc_control_status->recovery_progress_pct = mavlink_msg_merivus_ftc_control_status_get_recovery_progress_pct(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN? msg->len : MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN;
        memset(merivus_ftc_control_status, 0, MAVLINK_MSG_ID_MERIVUS_FTC_CONTROL_STATUS_LEN);
    memcpy(merivus_ftc_control_status, _MAV_PAYLOAD(msg), len);
#endif
}
