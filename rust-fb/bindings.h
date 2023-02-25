#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>
using VOffsetT = int8_t;

constexpr static const uint8_t ENUM_MIN_EVENT = 0;

constexpr static const uint8_t ENUM_MAX_EVENT = 2;

constexpr static const VOffsetT PlayerSpawn_VT_PLAYER_ID = 4;

constexpr static const VOffsetT PlayerSpawn_VT_POS = 6;

constexpr static const VOffsetT PlayerMoved_VT_POS1 = 6;

constexpr static const VOffsetT PlayerMoved_VT_POS2 = 8;

constexpr static const VOffsetT PlayerEvent_VT_TS = 4;

constexpr static const VOffsetT PlayerEvent_VT_EVENT_TYPE = 6;

constexpr static const VOffsetT PlayerEvent_VT_EVENT = 8;

constexpr static const VOffsetT EventBatch_VT_EVENT_BATCH = 4;

extern "C" {

float distance_points(float x1, float y1, float z1, float x2, float y2, float z2);

float calculate_event_distance(const uint8_t *buf, uintptr_t len);

} // extern "C"
