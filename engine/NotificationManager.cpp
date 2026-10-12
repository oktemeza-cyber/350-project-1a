#include "NotificationManager.h"

namespace CMPUT350 {

void NotificationManager::Register(std::weak_ptr<GameObject> object, const std::string& key) {
    mListeners[key].push_back(object);
}

void NotificationManager::Unregister(std::weak_ptr<GameObject> object, const std::string& key) {
    auto it = mListeners.find(key); // Search for key

    if (it == mListeners.end()) {
        return; // Not listening for this key
    }

    auto &vec = it->second; // Get reference to objects vector
    auto target = object.lock(); // Convert weak_ptr to shared_ptr for comparison

    for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
        if (iter->lock() == target) {
            iter = vec.erase(iter);
        }
    }
}

void NotificationManager::Notify(const std::string& message) {
    auto it = mListeners.find(message); // Search for key

    if (it == mListeners.end()) {
        return; // Not listening for this key (later message)
    }

    auto &vec = it->second; // Get reference to objects vector

    for (auto iter = vec.begin(); iter != vec.end(); ) {
        if (auto obj = iter->lock()) { // Convert weak_ptr to shared_ptr
            obj->ReceiveNotification(message);
            ++iter;
        } else {
            iter = vec.erase(iter); // Remove expired weak_ptr
        }
    }
}

}