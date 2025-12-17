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

