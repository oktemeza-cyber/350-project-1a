#ifndef NOTIFICATION_MANAGER_H
#define NOTIFICATION_MANAGER_H

#include "GameObject.h"
#include <memory> // Just kept adding includes until it stopped underlining everything
#include <string>
#include <vector>
#include <unordered_map>

namespace CMPUT350 {

class NotificationManager {
public:
    NotificationManager();
    void Register(std::weak_ptr<GameObject> object, const std::string& key);
    void Unregister(std::weak_ptr<GameObject> object, const std::string& key);
    void Notify(const std::string& message);
private:
    std::unordered_map<std::string, std::vector<std::weak_ptr<GameObject>>> mListeners;
    // std::unordered_map<unsigned int, std::string> mKeyNotifications; This is later
};

} // namespace CMPUT350

#endif // NOTIFICATION_MANAGER_H