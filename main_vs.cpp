#include <iostream>
#include "disk.h"
#define ROW 2
#define COL 8

void menu() {
	cout << "------------------menu-------------------" << endl;
	cout << "0. �˳������浱ǰλʾͼ" << endl;
	cout << "1. ��ʾλ��ͼ" << endl;
	cout << "2. ���̷������" << endl;
	cout << "3. �����ͷŲ���" << endl;
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
		//cout << "������һ��������ʾҪִ�еĲ���" << endl;
		cin >> op;
		switch (op) {
		case 1:
			cout << "-----------------bitmap------------------" << endl;
			disk->show_bitmap(); 
			cout << "-----------------bitmap------------------" << endl;
			break;
		case 2:
			cout << "��ֱ���������������ʾ��Ҫ����Ĵ��̿����Ѿ��Ƿ�����" << endl;
			cin >> num >> continuous;
			idxs = disk->block_allocate(num, continuous);
			if (!idxs.empty()) {
				cout << "������Ĵ��̿��ŷֱ���:";
				for (u32 idx : idxs) {
					cout << idx << " ";
				}
				cout << endl;
			}
			else {
				cout << "���̷���ʧ��" << endl;
			}
			break;
		case 3:
			cout << "��ֱ���������������ʾ��ʼ�ͷŵĴ��̺�����Ҫ�ͷŵĴ��̿���" << endl;
			cin >> start_idx >> num;
			idxs = disk->block_release(start_idx, num);
			if (!idxs.empty()) {
				cout << "���ͷŵĴ��̿��ŷֱ���:";
				for (u32 idx : idxs) {
					cout << idx << " ";
				}
				cout << endl;
			}
			else {
				cout << "�����ͷ�ʧ��" << endl;
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