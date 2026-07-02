# PX4 v1.14 SITL and MERIVUS Notes

## Ground station changes in this commit

- `custom/res/Merivus/CommandCenterOverlay.qml`
  - Unified the top three command cards for altitude, ground speed, and climb speed.
  - Each top card now shows live telemetry, command input, +/- buttons, and a send action.
  - Climb speed sends `MAV_CMD_DO_CHANGE_SPEED` with speed type `2` through `Vehicle.sendCommand`.
  - The lower heading, battery, and GPS cards are compacted so the two metric rows stay visually balanced.
  - The map target summary is more prominent when a vehicle or queue is selected.
  - The vehicle-list clear action is now a rounded compact button.
  - The right-side attitude target selector uses checkbox-style options while still selecting one displayed UAV at a time.

- `custom/res/Merivus/FlyViewMap.qml`
  - Left-button marquee selection uses the MERIVUS/QGC accent color, translucent gradient fill, rounded border, and softer fade.
  - Right-click go-to feedback uses a waypoint dot, short stem, halo, and two fading ripple rings.
  - Shift-right-click queued waypoints keep the amber queue color; normal go-to follows the system accent color.

- `src/AutoPilotPlugins/PX4/SafetyComponent.qml`
  - Reduced the custom GNSS/visual-positioning card to PX4-backed navigation-loss settings.
  - Bound navigation-loss action to `COM_POSCTL_NAVL` and delay to `COM_POS_FS_DELAY`.
  - Reduced the IoT/TCP card to MERIVUS-side TCP detection placeholders plus PX4 MAVLink data-link failsafe settings.
  - Bound MAVLink link-loss action to `NAV_DLL_ACT` and timeout to `COM_DL_LOSS_T`.
  - Removed unused placeholder action/source models from the new safety cards.

## PX4 v1.14 SITL items to change outside this repository

PX4 v1.14 does not provide a true stick-input-disabled `COM_RC_IN_MODE=4` mode. In v1.14, `COM_RC_IN_MODE=1` disables traditional RC checks, but commander can still require MAVLink manual-control/joystick input before accepting Mission mode. If MERIVUS disables the virtual joystick and does not send manual-control input, Mission mode can still be rejected with `No manual control input`.

Recommended options:

1. Keep PX4 v1.14 and provide a MAVLink `MANUAL_CONTROL` heartbeat from MERIVUS, or from a small SITL companion process, when mouse-based command control replaces the virtual joystick.
2. Backport the newer PX4 stick-input-disabled behavior into PX4 v1.14 commander/manual-control logic, then set that value in SITL.
3. Upgrade PX4 to a version that supports the stick-input-disabled `COM_RC_IN_MODE` value, then set that value for SITL.

Suggested PX4-side files/locations:

- `ROMFS/px4fmu_common/init.d-posix/rcS`
  - Put SITL parameter overrides after the selected airframe defaults, otherwise the airframe script can overwrite them.
  - Verify after every SITL boot with `param show COM_RC_IN_MODE`.

- PX4 commander parameter definition for `COM_RC_IN_MODE`
  - Extend accepted values only when intentionally backporting the newer disabled-stick behavior.

- PX4 commander/manual-control preflight checks
  - If backporting, skip manual-control presence only for the new disabled-stick mode.
  - Do not skip the check for normal RC or joystick modes.

- SITL regression failsafe parameters
  - `COM_POSCTL_NAVL`: position-control navigation-loss response.
  - `COM_POS_FS_DELAY`: navigation-loss delay.
  - `NAV_DLL_ACT`: data-link-loss action; use Return when the vehicle should RTL after MAVLink loss.
  - `COM_DL_LOSS_T`: data-link-loss timeout.

## Safety-card data judgment rules

- Navigation/GNSS loss should be judged by PX4 estimator validity, not by MERIVUS guessing from the map view.
  - Use PX4 failsafe parameters `COM_POSCTL_NAVL` and `COM_POS_FS_DELAY` for the action and delay.
  - Use vehicle telemetry only for display: GPS satellite count, global/local position validity, EKF health, and current navigation state.
  - If PX4 reports navigation loss and the configured action is Return, MERIVUS should display the fault and avoid sending conflicting mission/goto commands.

- Cloud IoT TCP loss is a MERIVUS-side link fault while MAVLink may still be alive.
  - Detection condition: `now - lastCloudTcpHeartbeatTime > configuredTcpTimeout`.
  - Data to display: TCP connected/disconnected, last heartbeat age, configured timeout, and current response state.
  - Response: command Return through MAVLink while MAVLink is still connected; if MAVLink is also lost, PX4 must handle `NAV_DLL_ACT`/`COM_DL_LOSS_T` onboard.

- Waypoint missions that stop between two waypoints should be debugged only after the RC/manual-control gate is fixed.
  - First confirm Mission mode can be entered without `No manual control input`.
  - Then confirm the vehicle has a valid global position estimate before upload/start.
  - If a safety rejection appears after a stuck segment, check navigation validity, geofence, battery failsafe, data-link loss, and current nav state before re-sending commands.

## Validation checklist

- Boot PX4 SITL and confirm runtime value with `param show COM_RC_IN_MODE`.
- Confirm Mission mode no longer reports `No manual control input` before evaluating waypoint behavior.
- Confirm global position estimate is valid before uploading a temporary mission.
- Test single right-click go-to and Shift-right-click queued route separately.
- For TCP/MAVLink loss tests, distinguish cloud TCP loss from MAVLink data-link loss: cloud TCP loss is handled by MERIVUS while MAVLink is connected; MAVLink loss is handled by PX4 failsafe parameters.
