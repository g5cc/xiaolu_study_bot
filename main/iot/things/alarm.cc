
#include "iot/thing.h"
#include "AlarmClock.h"
#include "application.h"
#include <esp_log.h>
#include "AlarmClock.h"
#if CONFIG_USE_ALARM
#define TAG "AlarmIot"
// extern AlarmManager* alarm_m_;
namespace iot {

// 这里仅定义 AlarmIot 的属性和方法，不包含具体的实现
class AlarmIot : public Thing {
public:
    AlarmIot() : Thing("Alarm", "可以定时提醒闹钟, 可以设置多个闹钟") {
         // 定义设备的属性
        properties_.AddStringProperty("Alarms", "Alarms", [this]() -> std::string {
            auto& app = Application::GetInstance();
            if(app.alarm_m_ == nullptr){
                ESP_LOGE(TAG, "AlarmManager is nullptr");
                return "There is no alarm clock here";
            }
            std::string info = app.alarm_m_->GetAlarmsInfo();
                return info;
        });

        // 定义设备可以被远程执行的指令
        methods_.AddMethod("SetAlarm", "设置一个闹钟", ParameterList({
            Parameter("seconde_from_now", "闹钟多少秒以后响", kValueTypeNumber, false),
            Parameter("alarm_name", "时钟的描述(名字)", kValueTypeString, true),
            Parameter("datetime_str", "闹钟具体时间字符串,有云端控制转换(格式: 2025-06-01 08:30:00)", kValueTypeString, false)
        }), [this](const ParameterList& parameters) {
            auto& app = Application::GetInstance();
            if(app.alarm_m_ == nullptr){
                ESP_LOGE(TAG, "AlarmManager is nullptr");
                return "There is no alarm clock here";
            }
            ESP_LOGI(TAG, "SetAlarm");
            int seconde_from_now = parameters["seconde_from_now"].number();
            std::string alarm_name = parameters["alarm_name"].string();
            std::string datetime_str = parameters["datetime_str"].string();
            app.alarm_m_->SetAlarm(seconde_from_now, alarm_name, datetime_str);
            return "ok";
        });
        methods_.AddMethod("DeleteAlarm", "删除一个闹钟", ParameterList({
            Parameter("alarm_name", "要删除的闹钟名字", kValueTypeString, true)
        }), [this](const ParameterList& parameters) {
            auto& app = Application::GetInstance();
            if(app.alarm_m_ == nullptr){
                ESP_LOGE(TAG, "AlarmManager is nullptr");
                return "There is no alarm clock here";
            }
            ESP_LOGI(TAG, "DeleteAlarm");
            std::string alarm_name = parameters["alarm_name"].string();
            app.alarm_m_->DeleteAlarm(alarm_name);
            return "ok";
        });
        methods_.AddMethod("GetAlarmsInfo", "获取所有闹钟信息", ParameterList({
            Parameter("alarm_all", "查询所有的闹钟", kValueTypeString, true)
        }), [this](const ParameterList& parameters) {
            auto& app = Application::GetInstance();
            if(app.alarm_m_ == nullptr){
                ESP_LOGE(TAG, "AlarmManager is nullptr");
                return "There is no alarm clock here";
            }
            ESP_LOGI(TAG, "GetAlarmsInfo");
            app.alarm_m_->GetAlarmsInfo();
            return "ok";
        });
        methods_.AddMethod("GetAlarm", "获取闹钟信息", ParameterList({
            Parameter("alarm_name", "要查询的闹钟名字", kValueTypeString, true)
        }), [this](const ParameterList& parameters) {
            auto& app = Application::GetInstance();
            if(app.alarm_m_ == nullptr){
                ESP_LOGE(TAG, "AlarmManager is nullptr");
                return "There is no alarm clock here";
            }
            ESP_LOGI(TAG, "GetAlarm");
            std::string alarm_name = parameters["alarm_name"].string();
            app.alarm_m_->GetAlarm(alarm_name);
            return "ok";
        });
    }
};

} // namespace iot

DECLARE_THING(AlarmIot);
#endif

