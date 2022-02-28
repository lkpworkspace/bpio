#include <cmath>
#include <memory>
#include <functional>
#include <unordered_map>

#include <google/protobuf/message.h>
#include <jsoncpp/json/json.h>

#include "bpbase.pb.h"
#include "BpIOHandle.hpp"
#include "BpIOMgr.hpp"
#include "BpIOBase.hpp"
#include "BpIOFile.hpp"

typedef google::protobuf::Message pb_msg_t;
typedef std::shared_ptr<google::protobuf::Message> pb_msg_ptr_t;

static bp::BpIOMgr g_mgr;

static std::unordered_map<std::string, std::function<std::shared_ptr<::google::protobuf::Message>()>> g_msg_map = {
    // {"bpmath.String", [](){ return std::make_shared<bp::String>(); }}
};

std::shared_ptr<::google::protobuf::Message> create_msg(const std::string& msg_name) {
    if (g_msg_map.find(msg_name) == g_msg_map.end()) {
        return nullptr;
    }
    return g_msg_map[msg_name]();
}

/*
input:
    string: file path
    string: mode
output:
    int: handle
*/
pb_msg_ptr_t open_file(std::vector<pb_msg_ptr_t> pb_file_info) {
    auto res = std::make_shared<bp::Int>();
    res->set_var(-1);
    auto info = std::make_shared<bp::BpIOInfoFile>();
    info->path = std::dynamic_pointer_cast<bp::String>(pb_file_info[0])->var();
    info->mode = std::dynamic_pointer_cast<bp::String>(pb_file_info[1])->var();
    // open file
    auto file_obj = std::make_shared<bp::BpIOFile>();
    if (file_obj->Open(info)) {
        return res;
    }
    // reg io obj
    if (0 != g_mgr.Reg(file_obj)) {
        return res;
    }
    res->set_var(file_obj->GetHandle());
    return res;
}

/*
input:
    int: handle
    int: size
output:
    int state
    string: data
*/
std::vector<pb_msg_ptr_t> read_file(std::vector<pb_msg_ptr_t> info) {
    auto res1 = std::make_shared<bp::Int>();
    auto res2 = std::make_shared<bp::String>();
    res1->set_var(-1);
    res2->set_var("");
    std::vector<pb_msg_ptr_t> res;
    res.emplace_back(res1);
    res.emplace_back(res2);

    auto handle = std::dynamic_pointer_cast<bp::Int>(info[0])->var();
    auto size = std::dynamic_pointer_cast<bp::Int>(info[1])->var();
    auto file_obj = g_mgr.GetIO(bp::BpIOType::BPIO_FILE, handle);
    if (file_obj == nullptr) {
        return res;
    }
    // read
    auto rd_sz = file_obj->Read(*res2->mutable_var(), size);
    res1->set_var(rd_sz);
    return res;
}

/*
input:
    int: handle
    string: data
output:
    int state
*/
pb_msg_ptr_t write_file(std::vector<pb_msg_ptr_t> pb_data) {
    auto handle = std::dynamic_pointer_cast<bp::Int>(pb_data[0])->var();
    auto data = std::dynamic_pointer_cast<bp::String>(pb_data[1])->var();
    auto res = std::make_shared<bp::Int>();
    res->set_var(-1);
    // find io obj
    auto file_obj = g_mgr.GetIO(bp::BpIOType::BPIO_FILE, handle);
    if (file_obj == nullptr) {
        return res;
    }
    // write
    auto wt_sz = file_obj->Write(data);
    res->set_var(wt_sz);
    return res;
}

void close_file(pb_msg_ptr_t pb_handle) {
    auto handle = std::dynamic_pointer_cast<bp::Int>(pb_handle)->var();
    // find io obj
    g_mgr.UnReg(bp::BpIOType::BPIO_FILE, handle);
}