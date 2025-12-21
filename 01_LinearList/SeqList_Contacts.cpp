#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

// 联系人结构体定义
struct Contact {
    char name[50];   // 姓名
    char phone[20];  // 电话号码
    int classNum;    // 班级
    int dorm;        // 宿舍号
};

const int MAX_SIZE = 20001;  // 最大容量

// 顺序线性表类定义
class SeqList {
private:
    Contact data[MAX_SIZE];  // 存储联系人的数组
    int length;              // 当前长度
    
public:
    // 构造函数
    SeqList() : length(0) {}
    
    // 查找联系人（根据姓名）
    int find(const char* name) {
        for (int i = 0; i < length; i++) {
            if (strcmp(data[i].name, name) == 0) {
                return i;
            }
        }
        return -1;
    }
    
    // 插入联系人
    //   const is used to prevent modification of input strings
    void insert(const char* name, const char* phone, int classNum, int dorm) {
        strcpy(data[length].name, name);
        strcpy(data[length].phone, phone);
        data[length].classNum = classNum;
        data[length].dorm = dorm;
        length++;
    }
    
    // 删除联系人
    void remove(const char* name) {
        int index = find(name);
        if (index != -1) {
            // 后续元素前移
            for (int i = index; i < length - 1; i++) {
                data[i] = data[i + 1];
            }
            length--;
        }
    }
    
    // 编辑联系人信息
    void edit(const char* name, int item, const char* newVal) {
        int index = find(name);
        if (index != -1) {
            switch (item) {
                case 1:  // 编辑电话号码
                    strcpy(data[index].phone, newVal);
                    break;
                case 2:  // 编辑班级
                    data[index].classNum = atoi(newVal);
                    break;
                case 3:  // 编辑宿舍号
                    data[index].dorm = atoi(newVal);
                    break;
            }
        }
    }
    
    // 按班级计算宿舍号异或值
    int xorByClass(int classNum) {
        int xorResult = 0;
        bool hasMember = false;
        for (int i = 0; i < length; i++) {
            if (data[i].classNum == classNum) {
                xorResult ^= data[i].dorm;
                hasMember = true;
            }
        }
        return hasMember ? xorResult : -1;
    }
};

// 主函数
int main() {
    int n;
    cin >> n;  // 操作数量
    SeqList contactList;  // 创建通讯录对象
    
    while (n--) {
        int op;
        cin >> op;  // 操作码
        
        switch (op) {
            case 0: {  // 插入操作
                char name[50], phone[20];
                int classNum, dorm;
                cin >> name >> phone >> classNum >> dorm;
                contactList.insert(name, phone, classNum, dorm);
                break;
            }
            case 1: {  // 删除操作
                char name[50];
                cin >> name;
                contactList.remove(name);
                break;
            }
            case 2: {  // 编辑操作
                char name[50], newVal[50];
                int item;
                cin >> name >> item >> newVal;
                contactList.edit(name, item, newVal);
                break;
            }
            case 3: {  // 查找操作
                char name[50];
                cin >> name;
                cout << (contactList.find(name) != -1 ? 1 : 0) << endl;
                break;
            }
            case 4: {  // 计算班级宿舍异或值
                int classNum;
                cin >> classNum;
                cout << contactList.xorByClass(classNum) << endl;
                break;
            }
        }
    }
    
    return 0;
}