#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

namespace CMPUT350 {

class GameContext;

class GameObject {
public:
    virtual ~GameObject() = default;
    virtual void Initialize(GameContext *context);
    virtual void Update(GameContext *context);
    virtual void LateUpdate(GameContext *context);
    virtual void RenderUI(GameContext *context);
    virtual bool IsAlive() const;
    virtual void Kill();
    virtual void ReceiveNotification(const std::string& key);

    virtual void HandleKeyEvent(GameContext *context, unsigned int unicode); // just so it doesn't error, but this needs to go
};

}  // namespace CMPUT350

#endif  // GAMEOBJECT_H
