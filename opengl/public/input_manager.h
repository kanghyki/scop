#ifndef INCLUDED_INPUT_MANAGER_H
#define INCLUDED_INPUT_MANAGER_H

#include <vector>

enum class InputState
{
    TAB,
    HOLD,
    AWAY,
    NONE
};

enum class Key
{
    Q,
    W,
    E,
    A,
    S,
    D,
    LEFT_CTRL,
    LEFT_ALT,
    LEFT_SHIFT,
    SPACE,
    ENTER,
    ESC,
    EOL
};

struct InputInfo
{
    InputState state_;
    bool prev_press_;
};

class InputManager
{
  public:
    static InputManager *GetInstance();
    ~InputManager();

    void Init();
    void Update();

    void UpdateKey(int key, int action);

    InputState GetKeyInputState(Key key);

  private:
    InputManager();
    InputManager(const InputManager &c);
    InputManager &operator=(const InputManager &c);

    static InputManager *instance_;

    std::vector<InputInfo> key_infos_;
};

#define KEY_CHECK(key, state) (InputManager::GetInstance()->GetKeyInputState(key) == state)
#define KEY_TAB(key) KEY_CHECK(key, InputState::TAB)
#define KEY_HOLD(key) KEY_CHECK(key, InputState::HOLD)
#define KEY_AWAY(key) KEY_CHECK(key, InputState::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, InputState::NONE)

#endif
