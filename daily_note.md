# 25/12/17

## Understanding `Class Queue`

实现数组类型的队列类操作，需要以下几步：

1. **定义一个队列类 (`Cardqueue`)**
    1. **private:**
        - `int* array`: for dynamic space allocation
        - `int head`: for array first element pointer
        - `int tail`: for array last element pointer
        - `int count`: for noting the current array size
        - `int capacity`: for allocating array size
    2. **public** (for initializing array):
        - Constructor `Cardqueue(int n)`:
            - `capacity = n + 1`
            - `array = new int[capacity]`
            - `count = 0`, `head = 0`, `tail = 0`
    3. **destruct function**
        - ~Cardqueue
    5. **void push function**
        - for constructing the `array` and circle the usage of `tail`
        - 定义一个int val，将其赋值给`array[tail]`，实现自顶向下的排序插入
        - 将`tail` 更新为`(tail + 1)%capacity`，`capacity` 已经被赋值为n+1，相当于在赋值最后一个n时，`tail`被更新为n+1，通过取模运算，实现了`tail` 重新被赋值为0，循环使用
        - count++
    7. **void pop function**
        - 如果`count`大于0，相当于还存在元素，使得`head`被赋值为`(head+1)%capacity`，等同于push函数的操作
    9. **int front**
        - return `array[head]`
    11. **int size**
        - return `count`
2. **主函数入口**
    1. 首先输入n个数值进入`array`
    2. **`Cardqueue queue(n)`**
    3. 使用for循环进行push的调用，使得
        1. `count` = n
        2. `tail` = n+1
    4. 使用while语句，当`size`大于等于2时
        1. 进行pop
            1. `count` = n-1
            2. `head` = `head`+1
        2. pop一次后，int `cardtomove` = queue.front()实现头的赋值
            1. 此时`cardtomove` = `head`+1
        3. 再进行一次pop()，为什么呢？要将其`count`--，并实现第二张卡牌的移除，首先将卡牌移除，再进行尾插
            1. count = n-2
        5. 再进行一次push(cardtomove)，相当于`array[tail]` = `cardtomove`,`count` = n-1
3. 所以主函数经过一次，count--，直到count = 2时，是最后一次遍历，cout `queue.front()`

## .MD文件格式快捷键
1. 加粗 (Bold): `Ctrl + B`
    - 选中文字按一下，自动变成 **文字**。
2. 斜体 (Italic): `Ctrl + I`
    - 选中文字按一下，自动变成 *文字*。
3. 插入链接 (Link): `Ctrl + K`
    - 自动生成 `[]()` 结构，并将光标定位在括号内。
4. 查找 (Find): `Ctrl + F`
    - 替换 (Replace): Ctrl + H
## 主要更新
此后不会在此文件夹里更新详细的代码分析，详细的代码分析会更新在代码注释中

# 2025/12/18
## 工作与学习时长：7H
## 主要学习内容：5.CPP 
1. 理解了STRUCT和CLASS主要区别，前者默认PUBLIC，更适合存储数据，后者默认PRIVATE，前者为深拷贝，后者为浅拷贝，需要自定义析构与复制构造与赋值运算符重载函数
    1. struct默认public，class默认private
    2. 结构体一般用于存储数据，类用于封装数据和行为
    3. 结构体成员默认public，类成员默认private
    4. 结构体适合简单数据聚合，类适合复杂对象建模
    5. 结构体通常没有成员函数，类通常有成员函数
    6. 结构体更接近C语言风格，类是C++的核心特性
    7. 结构体实例化时可以省略struct关键字，类实例化时必须使用类名
    8. 结构体更适合POD类型，类适合需要封装和继承的场景
    9. 结构体和类在语法上非常相似，但语义不同
    10. 二者不同在于结构体是深拷贝，而类是浅拷贝
        1. 所以在类中需要手动实现复制构造函数和赋值运算符重载
        2. 类支持封装复杂行为，适合实现矩阵的各种操作
        3. 类可以包含成员函数，便于组织代码逻辑

3. 理解5.CPP的主要作用
   1. 构造一个矩阵结构，存储稀疏矩阵的非零元素，最重要的是在结构体中自定义一个行主序排列，能够使得该结构体元素能够以行优先排序，三元结构矩阵：行、列、非零元素
   2. 再构造稀疏矩阵类，因为是CLASS类，需要自定义PUBLIC，否则默认PRIVATE，并且类相较于结构体，除了能运营复制构造与赋值运算符重载实现深拷贝，还可以自定义成员函数，还能够自定义析构函数，实现内存管理：行、列、非零元素数量（这里的行列指的是该稀疏矩阵的全部行列，而非 非零元素的行列）、CAPACITY动态数组容量以及 一个矩阵结构的DATA指针（用于存放三元结构矩阵的非零元素信息）支持矩阵转置、加减法操作
        1. 析构函数：先确定DATA不是指向空指针，DELETE，再将其指向空指针。
        2. 复制构造函数：常量为稀疏矩阵类的OTHER引用，实现ROW、COL、CAPACITY、TERMS的深拷贝，以及DATA的新动态内存的开辟，使用FOR循环，实现稀疏矩阵DATA的深拷贝
        3. 赋值运算符重载：先进行TIHS OTHER的匹配，如果相等直接返回THIS的引用，删除原有的DATA指针，将OTHER稀疏矩阵四元素赋值新的四元素，此时再进行DATA的NEW动态内存分配，
            1. 1、2、3相当于C++的三法则，三者共同实现内存的动态分配，防止出现内存泄漏，指针错配现象
        4. append函数：
            1. 如果TERMS大于CAPACITY，则进行CAPACITY扩容
            2. 实现矩阵结构NEW DATA的新复制（NEW CAPACITY）
            3. 删除原有DATA指针
            4. 新赋值CAPACITY
            5. 对于R\C\V进行赋值（DATA[TERMS]）实现矩阵的尾部赋值
        5. 快速排序函数：没什么可说的，运用的是深度递归，自我调用，创造左右边界[L,R]进行中间值的副本取值[PIVOT]对于L,R范围的不断进行分割排序，时间复杂度为T LOG T，空间复杂度为LOG T，由于一开结构体已经定义了排序逻辑，这里直接使用符号即可
        6. 重置函数：
        7. 定义静态函数（辅助函数）
        8. 转置函数：
     
# 2025/12/19
##工作、学习时长：
## 硬件角度理解堆分配、栈分配、全局/静态分配
## 6.CPP学习
## ||符号表示的是或：二者满足一个即为真（短路求值）



