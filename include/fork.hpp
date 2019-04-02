#pragma once

class Fork {

    bool _in_use;

public:
    Fork() = default;
    void use();
    void leave();
};