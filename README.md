# SoftwareFactory的输入输出模块

## 文件读写
* 打开文件
    * 输入参数：
        * 文件路径
        * 打开模式，参见fopen
    * 输出参数：
        * 大于0打开成功，获得文件句柄；小于0，打开文件失败
* 读文件
    * 输入参数
        * 文件句柄
        * 读文件长度
    * 输出参数
        * 读文件实际长度
        * 读出数据
* 写文件
    * 输入参数
        * 文件句柄
        * 写入数据
    * 输出参数
        * 大于0，实际写文件长度；小于0，写文件失败
* 关闭文件
    * 输入参数
        * 文件句柄