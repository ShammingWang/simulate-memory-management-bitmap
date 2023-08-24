#include "disk.h"
#include <fstream>
#include <iostream>
#include <cstring>


using namespace std;

u16 grid_idx(u16 index) {
	return index >> 3;
}

u8 in_grid_index(u16 index) {
	return (index & 0x0007);
}

void Disk::setbit(u16 x, u16 y, bool flag) { //把位图看作一个二维数组
	u16 col_grid = ((m - 1) >> 3) + 1; //计算每行所需要的内存格子数量 一个格子是8位 下标都是从0开始算
	//u16 col_grid_virtual = col_grid << 3; // 计算虚拟的格子数量 
	u16 index = grid_idx(y);
	u16 offset = in_grid_index(y); // 取得低三位 作为偏移
	if (x >= n || y >= m) { //数据超出了磁盘定义的大小
		cout << "setbit函数越界访问" << endl;
		return;
	}
	if (flag) { //需要将指定位置1的情况
		bitmap[col_grid * x + index] |= (0x80 >> offset);
	}
	else { //需要将指定位置0的情况
		bitmap[col_grid * x + index] &= ~(0x80 >> offset);
	}
}

void Disk::setbit(u32 idx, bool flag) { //把位图看作一个二维数组
    //u16 col_grid = ((m - 1) >> 3) + 1; //计算每行所需要的内存格子数量 一个格子是8位 下标都是从0开始算
	//u16 col_grid_virtual = col_grid << 3; // 计算虚拟的格子数量
	setbit(idx / m, idx % m, flag); //将每一个比特位换算成二维坐标
}

bool Disk::getbit(u16 x, u16 y, bool& err) { //把位图看作一个二维数组
	u16 col_grid = ((m - 1) >> 3) + 1; //计算每行所需要的内存格子数量 一个格子是8位 下标都是从0开始算
	//u16 col_grid_virtual = col_grid << 3; // 计算虚拟的格子数量 
	u16 index = grid_idx(y);
	u16 offset = in_grid_index(y); // 取得低三位 作为偏移
	if (x >= n || y >= m) { //数据超出了磁盘定义的大小
		cout << "getbit函数越界访问" << endl;
		return err = true;
	}
	return (bitmap[col_grid * x + index] & (0x80 >> offset)) != 0;
}

bool Disk::getbit(u32 idx, bool& err) { //把位图看作一个二维数组
    //u16 col_grid = ((m - 1) >> 3) + 1; //计算每行所需要的内存格子数量 一个格子是8位 下标都是从0开始算
	//u16 col_grid_virtual = col_grid << 3; // 计算虚拟的格子数量 
	return getbit(idx / m, idx % m, err); //将每一个比特位换算成二维坐标
}

void Disk::show_bitmap() {
	for (int i = 0; i < this->n; i++) {
		for (int j = 0; j < this->m; j++) {
			bool err = false;
			bool flag = this->getbit(i, j, err);
			if (!err) {
				cout << (flag ? "1" : "0");
			}
		}
		cout << endl;
	}
}

std::vector<u16> extractNumbers(const std::string& str)
{
    string bitmap_filename = str.substr(str.find_last_of("/\\")+1);
    //QMessageBox::information(nullptr, "警告", QString::fromStdString(bitmap_filename));
    std::vector<u16> numbers;
    std::string currentNumber = "";
    for(const char& c : bitmap_filename) // bitmap_filename
    {
        if(std::isdigit(c))
        {
            currentNumber += c;
        }
        else if(!currentNumber.empty())
        {
            numbers.push_back(std::stoi(currentNumber));
            currentNumber = "";
        }
    }
    if(!currentNumber.empty())
    {
        numbers.push_back(std::stoi(currentNumber));
    }
    return numbers;
}

Disk::Disk(string bitmap_filename) {
    std::vector<u16> numbers = extractNumbers(bitmap_filename);
    if(numbers.size() != 2) {
        memset(this, 0, sizeof(Disk));
        return;
    }
    u16 n = numbers[0];
    u16 m = numbers[1];
    this->n = n;
    this->m = m;
    this->bitmap_filename = bitmap_filename;
    if (m == 0 || n == 0) {
        bitmap = nullptr;
        return;
    }
    u16 col_grid = ((m - 1) >> 3) + 1; //计算每行所需要的内存格子数量 一个格子是8位 下标都是从0开始算
    //u16 col_grid_virtual = col_grid << 3; // 计算虚拟的格子数量
    bitmap = new u8[n * col_grid]; //为bitmap分配内存空间 大小就是计算出的所需要的格子的数量
    memset(bitmap, 0, n * col_grid);
    fstream fs(bitmap_filename, ios::in | ios::out); //尝试打开文件 bitmap_filename
    if (!fs.is_open()) {
        cout << "文件打开失败" << endl;
        bitmap = nullptr;
        return;
    }
    u16 row = 0;
    u32 idx = 0;
    string line;
    while (row++ < n && getline(fs, line)) { // 只有不越界且读取到数据的时候再做
        for (u16 i = 0; i < line.size() && i < m;i ++) { // 读入m位且不能越界
            char c = line[i];
            setbit(idx++, c == '1');
        }
    }
    if (idx != u32(1) * n * m) {
        cout << "读入文件的0和1数量有误" << endl;
    }
    fs.close();
}

Disk::Disk(u16 n, u16 m, string bitmap_filename) {
	this->n = n;
	this->m = m;
	this->bitmap_filename = bitmap_filename;
	if (m == 0 || n == 0) {
		bitmap = nullptr;
		return;
	}
	u16 col_grid = ((m - 1) >> 3) + 1; //计算每行所需要的内存格子数量 一个格子是8位 下标都是从0开始算
    //u16 col_grid_virtual = col_grid << 3; // 计算虚拟的格子数量
	bitmap = new u8[n * col_grid]; //为bitmap分配内存空间 大小就是计算出的所需要的格子的数量
	memset(bitmap, 0, n * col_grid);
	fstream fs(bitmap_filename, ios::in | ios::out); //尝试打开文件 bitmap_filename
	if (!fs.is_open()) {
		cout << "文件打开失败" << endl;
		bitmap = nullptr;
		return;
	}
	u16 row = 0;
	u32 idx = 0;
	string line;
	while (row++ < n && getline(fs, line)) { // 只有不越界且读取到数据的时候再做
		for (u16 i = 0; i < line.size() && i < m;i ++) { // 读入m位且不能越界
			char c = line[i];
			setbit(idx++, c == '1');
		}
	}
	if (idx != u32(1) * n * m) {
		cout << "读入文件的0和1数量有误" << endl;
	}
	fs.close();
}
Disk::~Disk() {
	// 更新bitmap到文本文件中
	//cout << "~Disk()" << endl;

    if(this->bitmap == nullptr) {
        return;
    }

	bool err;
	ofstream ofs(this->bitmap_filename);
	if (!ofs.is_open()) {
		cout << this->bitmap_filename << "文件打开失败" << endl;
		delete[] bitmap; // 释放bitmap的内存
		return;
	}

	for (u16 i = 0; i < this->n; i++) {
		for (u16 j = 0; j < this->m; j++) {
			err = false;
			bool bit = getbit(i, j, err);
			if (err) {
				delete[] bitmap; // 释放bitmap的内存
				return;
			}
			ofs << (bit ? '1' : '0');
		}
		ofs << endl;
	}

	ofs.close();

	delete[] bitmap; // 释放bitmap的内存
}

vector<u32> Disk::block_allocate(u32 num, bool continuous) {
	bool err;
	if (num == 0) return vector<u32>();
	if (num == 1) {
		for (u32 idx = 0; idx < u32(1) * this->n * this->m; idx++) {
			err = false;
			bool bit = getbit(idx, err);
			if (err) return vector<u32>();
			if (!bit) {
				setbit(idx, true);
				return  vector<u32>({ idx });
			}
		}
        QMessageBox::information(nullptr, "警告", "磁盘空间已满，无法分配");
		return vector<u32>();
	}
	// 下面是要分配多块磁盘的情况
	if (continuous) {
		bool lastbit = true; // 初始化设置为上一个为已经使用的状态
		u32 current_block = 0; // 初始化当前已经找到的连续的磁盘数
		u32 block_start_id = -1;
		for (u32 idx = 0; idx < u32(1) * this->n * this->m; idx++) {
			err = false;
			bool bit = getbit(idx, err);
            if (err) {
                QMessageBox::information(nullptr, "警告", "位访问越界");
                return vector<u32>();
            }
			if (!bit) { // 当前位没有被使用
				if (lastbit == false) { // 上一个位没有被使用
					current_block++;
					if(current_block == num) { // 当前连续的个数已经达到了
						break;
					}
				}
				else { // 上一个位被使用了，那么这个就是第一个没有被使用的位
					current_block = 1; // 这个时候不用break 因为至少是多个磁盘块
					block_start_id = idx; // 记录这个可能的开始块号
				}
			}
			else { // 当前位已经被使用了
				current_block = 0;
				block_start_id = -1; // 清除答案
			}
			lastbit = bit;
		}
		if (current_block != num) {
            QMessageBox::information(nullptr, "警告", "找不到连续的" + QString::number(num) + "个磁盘块");
            //cout << "找不到连续的" << num << "个磁盘块" << endl;
			return vector<u32>();
		}
		else {
			vector<u32> ans;
			for (u32 i = 0; i < num; i++) {
				setbit(block_start_id + i, true);
				ans.push_back(block_start_id + i);
			}
			return ans;
		}
	}
	else { // 下面考虑不需要连续分配的情况
		vector<u32> ans;
		for (u32 idx = 0; idx < u32(1) * this->n * this->m; idx++) {
			err = false;
			bool bit = getbit(idx, err);
			if (err) return vector<u32>();
			if (!bit) {
				ans.push_back(idx);
			}
			if (ans.size() == num) break;
		}
		if (ans.size() != num) {
            QMessageBox::information(nullptr, "警告", "找不到离散的" + QString::number(num) + "个磁盘块");
            //cout << "找不到离散的" << num << "个磁盘块" << endl;
			return vector<u32>();
		}
		for (u32 idx : ans) {
			setbit(idx, true);
		}
		return ans;
	}
}

vector<u32> Disk::block_release(u32 start_id, u32 num) {
	u32 max_id = u32(1) * this->n * this->m;
	if (start_id + num - 1 >= max_id) {
        QMessageBox::information(nullptr, "警告", "需要释放的磁盘数量过大");
        //cout << "需要释放的磁盘数量过大" << endl;
		return vector<u32>();
	}
	vector<u32> ans;
	for (u32 i = 0; i < num; i++) {
		setbit(start_id + i, false);
		ans.push_back(start_id + i);
	}
	return ans;
}

void Disk::sort() {
    u32 num = 0;
    u32 max_id = u32(1) * this->n * this->m;
    for (u32 i = 0; i < max_id; i++) {
        bool err=  false;
        if(getbit(i, err)) {
            if(err) {
                QMessageBox::information(nullptr, "警告", "位访问越界");
                return;
            }
            num++;
        }
    }
    for (u32 i = 0; i < max_id; i++) {
        setbit(i, i < num);
    }
}

u32 Disk::getRest() {
    u32 num = 0;
    u32 max_id = u32(1) * this->n * this->m;
    for (u32 i = 0; i < max_id; i++) {
        bool err=  false;
        if(getbit(i, err)) {
            if(err) {
                QMessageBox::information(nullptr, "警告", "位访问越界");
                return 0;
            }
            num++;
        }
    }
    return num;
}
