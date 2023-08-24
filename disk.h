#ifndef __DISK_H
#define __DISK_H

#include <cstdint>
#include <string>
#include <vector>
#include <QMessageBox>


using namespace std;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

class Disk {


	string bitmap_filename; //存放要读取的文件位置
public:
    u16 n, m;
    u8* bitmap;

    Disk(string bitmap_filename); //只用文件名的构造函数
	Disk(u16 n, u16 m, string bitmap_filename); //构造磁盘的构造函数
	void setbit(u16 x, u16 y, bool flag); // flag为真表示把当前位置设置为1 如果是假则设置为0
	void setbit(u32 index, bool flag); //
	bool getbit(u16 x, u16 y, bool& err); // 返回一个逻辑值 真：1 假：0
	bool getbit(u32 idx, bool& err); // 返回一个逻辑值 真：1 假：0
    u32 getRest(); // 获取剩余的
	void show_bitmap();

    void sort();

	//用户提出磁盘请求（若干个物理块数，是否要求连续），查找位示图，给出若干个空闲盘块号；
	vector<u32> block_allocate(u32 num, bool continuous);
	vector<u32> block_release(u32 block_id, u32 num); // 多传入一个num 表示释放从id开始的连续num个盘块
    ~Disk(); //释放内存并保存文件
};
#endif
