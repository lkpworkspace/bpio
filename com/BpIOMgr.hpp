#pragma once
#include <memory>
#include <unordered_map>

#include <glog/logging.h>

#include "BpIOBase.hpp"

namespace bp {

class BpIOMgr {
    typedef std::unordered_map<int, std::shared_ptr<BpIOBase>> one_baseio_map_t;
    typedef std::unordered_map<int, one_baseio_map_t> all_baseio_map_t;
public:
    int Reg(std::shared_ptr<BpIOBase> io) {
        if (io == nullptr) {
            LOG(ERROR) << "io obj is nullptr";
            return -1;
        }
        auto handle = io->GetHandle();
        if (handle <= 0) {
            LOG(ERROR) << "io obj no handle";
            return -1;
        }
        auto io_type = (int)io->GetType();
        if (all_base_io_map.find(io_type) == all_base_io_map.end()) {
            one_baseio_map_t one_base_io;
            all_base_io_map[io_type] = one_base_io;
        }
        if (all_base_io_map[io_type].find(handle) != all_base_io_map[io_type].end()) {
            LOG(WARNING) << "io obj has same handle";
        }
        all_base_io_map[io_type][handle] = io;
        return 0;
    }
    void UnReg(BpIOType type, int handle) {
        if (all_base_io_map.find((int)type) != all_base_io_map.end() 
            && all_base_io_map[(int)type].find(handle) != all_base_io_map[(int)type].end()) {
            all_base_io_map[(int)type].erase(handle);
        }
    }
    std::shared_ptr<BpIOBase> GetIO(BpIOType type, int handle) {
        if (all_base_io_map.find((int)type) != all_base_io_map.end() 
            && all_base_io_map[(int)type].find(handle) != all_base_io_map[(int)type].end()) {
            return all_base_io_map[(int)type][handle];
        }
        LOG(WARNING) << "can't find handle, " << handle;
        return nullptr;
    }
    
private:
    all_baseio_map_t all_base_io_map;
};

} // namespace bp