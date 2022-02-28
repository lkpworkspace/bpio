#pragma once
#include <string>

#include "bpbase.pb.h"

namespace bp {

enum class BpIOType : int {
    BPIO_NONE,
    BPIO_FILE,
};

class BpIOInfo {
public:
    virtual BpIOType GetType() = 0;
};

class BpIOInfoFile : public BpIOInfo {
public:
    BpIOType GetType() override { return BpIOType::BPIO_FILE; }
    std::string path;
    std::string mode;
};

class BpIOBase
{
public:
    virtual BpIOType GetType() = 0;
    virtual int GetHandle() = 0;
    virtual int Open(std::shared_ptr<BpIOInfo> info) = 0;
    virtual int Read(std::string& buf, size_t len) = 0;
    virtual int Write(const std::string& data) = 0;
    virtual void Close() = 0;
};

} // namespace bp