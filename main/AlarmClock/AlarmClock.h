
#ifndef ALARMCLOCK_H
#define ALARMCLOCK_H

#include <string>
#include <vector>
#include <esp_log.h>
#include <esp_timer.h>
#include "time.h"
#include <mutex>
#include "settings.h"
#include <atomic>
#if CONFIG_USE_ALARM
struct Alarm {
    std::string name;
    int time;
};

class AlarmManager {
public:
    AlarmManager();
    ~AlarmManager();

    // 设置闹钟
    void SetAlarm(int seconde_from_now, std::string alarm_name, std::string datetime_str);
    // 删除闹钟
    void DeleteAlarm(const std::string& alarm_name);
    // 获取闹钟
    std::string GetAlarm(const std::string& alarm_name);
    // 获取闹钟列表
    std::string GetAlarmsInfo();
    // 获取闹钟列表状态
    // std::string GetAlarmsStatus();
    // 清除过时的闹钟
    void ClearOverdueAlarm(time_t now);
    // 获取从现在开始第一个响的闹钟
    Alarm *GetProximateAlarm(time_t now);
    // 闹钟响了的处理函数
    void OnAlarm();
    // 闹钟是不是响了的标志位
    bool IsRing(){ return ring_flag; };
    // 清除闹钟标志位
    void ClearRing(){ESP_LOGI("Alarm", "clear");ring_flag = false;};

    // 延迟铃声循环标志位
    bool IsDelay(){ return delay_flag; };
    // 清除延迟铃声循环标志位
    void ClearDelay(){ESP_LOGI("AlarmSound delay", "clear");delay_flag = false;};

private:
    std::vector<Alarm> alarms_; // 闹钟列表
    std::mutex mutex_; // 互斥锁
    esp_timer_handle_t timer_; // 定时器

    std::atomic<bool> ring_flag{false}; 
    std::atomic<bool> running_flag{false};
    std::atomic<bool> delay_flag{false}; 

    enum {
        ALARM_RING,
        ALARM_IOT,
        ALARM_COMMUNICATION,
        ALARM_FUNCTION
    }alarm_state;
};
#endif
#endif