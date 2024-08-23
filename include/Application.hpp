#include "Window.hpp"

class Application : public Window{

public:
    Application();
protected:
       void onUpdate()override;
       void onRender()override;
};