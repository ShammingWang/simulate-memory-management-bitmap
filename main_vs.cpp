#include <iostream>
#include "disk.h"
#define ROW 2
#define COL 8

void menu() {
	cout << "------------------menu-------------------" << endl;
	cout << "0. 退出并保存当前位示图" << endl;
	cout << "1. 显示位视图" << endl;
	cout << "2. 磁盘分配操作" << endl;
	cout << "3. 磁盘释放操作" << endl;
	cout << "------------------menu-------------------" << endl;
}

int main() {
	Disk* disk = new Disk(ROW, COL, "bitmap.txt");
	

	int op = 0;
	int num, continuous;
	int start_idx;
	vector<u32> idxs;
	bool flag = false;
	while (1) {
		if (flag) break;
		menu();
		//cout << "请输入一个整数表示要执行的操作" << endl;
		cin >> op;
		switch (op) {
		case 1:
			cout << "-----------------bitmap------------------" << endl;
			disk->show_bitmap(); 
			cout << "-----------------bitmap------------------" << endl;
			break;
		case 2:
			cout << "请分别输入两个整数表示需要分配的磁盘块数已经是否连续" << endl;
			cin >> num >> continuous;
			idxs = disk->block_allocate(num, continuous);
			if (!idxs.empty()) {
				cout << "所分配的磁盘块编号分别是:";
				for (u32 idx : idxs) {
					cout << idx << " ";
				}
				cout << endl;
			}
			else {
				cout << "磁盘分配失败" << endl;
			}
			break;
		case 3:
			cout << "请分别输入两个整数表示起始释放的磁盘号与需要释放的磁盘块数" << endl;
			cin >> start_idx >> num;
			idxs = disk->block_release(start_idx, num);
			if (!idxs.empty()) {
				cout << "所释放的磁盘块编号分别是:";
				for (u32 idx : idxs) {
					cout << idx << " ";
				}
				cout << endl;
			}
			else {
				cout << "磁盘释放失败" << endl;
			}
			break;
		case 0:
			flag = true;
			break;
		}
		
	}
	delete disk;
	return 0;
}