#ifndef IOT_THINGS_MOVE_H_
#define IOT_THINGS_MOVE_H_

#include "iot/thing.h"
#include "board.h"
#include "settings.h"
#include <esp_log.h>
#include <driver/ledc.h>


#define TAG "Move"
#define MAX_DISTANCE_CM 300  // 最大移动距离3米
#define MAX_ANGLE_DEG 3600   // 最大旋转角度10圈
#define SERVO_PIN GPIO_NUM_20
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT
#define LEDC_FREQ 50         // 50Hz, 20ms周期

namespace iot {

class Move : public Thing {
public:
    Move();
    
    // 舵机控制方法
    void SetServoAngle(int angle);  // 角度范围0-180度
    
private:
    // 内部单位转换方法
    int StepsToCm(int steps) { return steps; }  // 1步=1cm
    int CmToSteps(int cm) { return cm; }
    int DegreesToSteps(int degrees) { return degrees; }
    int CirclesToDegrees(int circles) { return circles * 360; }
    
    // 移动限制检查
    bool CheckDistanceLimit(int distance_cm);
    bool CheckAngleLimit(int angle_deg);
};

} // namespace iot

DECLARE_THING(Move);

#endif  // IOT_THINGS_MOVE_H_
