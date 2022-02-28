#pragma once
#include <list>

namespace bp {

class BpIOHandle {
public:
    int AcqHandle() {
        int handle = -1;
        if (!_unused_handle.empty()) {
            handle = _unused_handle.front();
            _unused_handle.pop_front();
        } else {
            handle = _next_handle++;
        }
        return handle;
    }

    void RelHandle(int handle) {
        _unused_handle.push_back(handle);
    }
private:
    int _next_handle = 1;
    std::list<int> _unused_handle;
};

} // namespace bp