#pragma once
#include <stdio.h>
#include <memory>

#include <glog/logging.h>

#include "BpIOBase.hpp"
#include "BpIOHandle.hpp"

namespace bp {

class BpIOFile : public BpIOBase {
public:
    BpIOFile() {
        _handle = _handle_mgr.AcqHandle();
    }
    ~BpIOFile() {
        Close();
    }
    BpIOType GetType() override {
        return BpIOType::BPIO_FILE;
    }
    int GetHandle() override {
        return _handle;
    }
    int Open(std::shared_ptr<BpIOInfo> info) override {
        auto file_info = std::dynamic_pointer_cast<BpIOInfoFile>(info);
        _fp = fopen(file_info->path.c_str(), file_info->mode.c_str());
        if (_fp == nullptr) {
            LOG(ERROR) << strerror(errno);
            return -1;
        }
        return 0;
    }
    int Read(std::string& buf, size_t len) override {
        buf.clear();
        buf.reserve(len);
        std::shared_ptr<char> tmp_buf(new char[len], [](char *p){ delete [] p; });
        auto rd_len = fread(tmp_buf.get(), 1, len, _fp);
        buf.append(tmp_buf.get(), rd_len);
        return rd_len;
    }
    int Write(const std::string& data) override {
        return fwrite(data.data(), 1, data.size(), _fp);
    }
    void Close() override {
        if (_fp == nullptr) {
            return;
        }
        LOG(INFO) << "close file, handle " << _handle;
        _handle_mgr.RelHandle(_handle);
        fclose(_fp);
        _handle = -1;
        _fp = nullptr;
    }
private:
    FILE* _fp;
    int _handle = -1;
    static BpIOHandle _handle_mgr;
};

BpIOHandle BpIOFile::_handle_mgr;

} // namespace bp