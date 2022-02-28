#include <memory>

#include <gtest/gtest.h>

#include "../com/bpio.cpp"

TEST(bpio, BpIOHandle) {
    bp::BpIOHandle bphandle;
    EXPECT_EQ(1, bphandle.AcqHandle());
    EXPECT_EQ(2, bphandle.AcqHandle());
    bphandle.RelHandle(1);
    EXPECT_EQ(1, bphandle.AcqHandle());
    EXPECT_EQ(3, bphandle.AcqHandle());
}

TEST(bpio, BpIOFile) {
    bp::BpIOFile bpfile;
    EXPECT_TRUE(bp::BpIOType::BPIO_FILE == bpfile.GetType());
    EXPECT_GT(bpfile.GetHandle(), 0);
    auto file_info = std::make_shared<bp::BpIOInfoFile>();
    file_info->path = "./BpIOFile_test.txt";
    file_info->mode = "w";
    EXPECT_EQ(0, bpfile.Open(file_info));
    EXPECT_EQ(5, bpfile.Write("Hello"));
    bpfile.Close();
}

TEST(bpio, bpio_file) {
    // open file
    auto path = std::make_shared<bp::String>();
    auto mode = std::make_shared<bp::String>();
    path->set_var("./bpio_file_test.txt");
    mode->set_var("w+");
    std::vector<pb_msg_ptr_t> open_info;
    open_info.emplace_back(path);
    open_info.emplace_back(mode);
    auto pb_handle = open_file(open_info);
    auto handle = std::dynamic_pointer_cast<bp::Int>(pb_handle);
    EXPECT_GE(handle->var(), 0);
    // write file
    for (int i = 0; i < 2; ++i) {
        auto pb_data = std::make_shared<bp::String>();
        pb_data->set_var("hello,world");
        std::vector<pb_msg_ptr_t> wt_info;
        wt_info.emplace_back(pb_handle);
        wt_info.emplace_back(pb_data);
        auto pb_wt_sz = write_file(wt_info);
        auto wt_sz = std::dynamic_pointer_cast<bp::Int>(pb_wt_sz);
        EXPECT_GE(wt_sz->var(), 0);
    }
    // close file
    close_file(handle);
}
