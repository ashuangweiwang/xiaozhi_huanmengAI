#include "iot/things/move.h"
#include "board.h"
#include "settings.h"
#include <esp_log.h>
#include <driver/ledc.h>

namespace iot {

Move::Move() : Thing("Move", "移动控制") {
    // 配置LEDC定时器
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQ,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // 配置LEDC通道
    ledc_channel_config_t ledc_channel = {
        .gpio_num = SERVO_PIN,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);

    // 初始化舵机到90度位置
    SetServoAngle(90);

    ESP_LOGI(TAG, "Move initialized with servo control");
}

void Move::SetServoAngle(int angle) {
    // 限制角度范围
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    // 计算占空比
    // 0.5ms (0度) -> 2.5ms (180度)
    // 20ms周期 (50Hz)
    // 13位分辨率: 8192 steps
    int duty = 102 + (angle * 102) / 180;  // 102~512

    // 设置占空比
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);

    ESP_LOGI(TAG, "Set servo angle to %d degrees (duty: %d)", angle, duty);
}

bool Move::CheckDistanceLimit(int distance_cm) {
    return distance_cm > 0 && distance_cm <= MAX_DISTANCE_CM;
}

bool Move::CheckAngleLimit(int angle_deg) {
    return angle_deg > 0 && angle_deg <= MAX_ANGLE_DEG;
}

} // namespace iot
