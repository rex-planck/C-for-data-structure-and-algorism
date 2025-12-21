#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cmath>

// 使用命名空间是为了方便使用一些基础函数，但在无STL场景下通常不是必须的
using namespace std;

#define MAX_LEN 2000 // 假设每行表达式最大长度
#define MAX_STACK 1000 // 栈的最大深度

// --- 手写栈实现 ---

// 数值栈
double valStack[MAX_STACK];
int valTop = -1; // 栈顶指针，-1表示空
/*
栈顶指针的作用是指示当前栈中最后一个元素的位置
当栈为空时，栈顶指针为 -1
*/ 
void val_push(double x) {
    valStack[++valTop] = x;
    // 先将栈顶指针自增 1，然后将数值 x 存储到栈顶位置
    /*
    这个函数的作用是将一个数值 x 压入数值栈中
    具体步骤如下：
    1. 先将栈顶指针 valTop 自增 1，表示栈中增加了一个元素
    2. 然后将数值 x 存储到栈顶位置 valStack[valTop] 中
    通过这种方式，数值栈可以动态地存储多个数值，实现后进先出 (LIFO) 的数据结构特性
    */ 
}

double val_pop() {
    if (valTop >= 0) {
        /*
        这个函数的作用是从数值栈中弹出栈顶元素并返回其值
        具体步骤如下：
        1. 首先检查栈顶指针 valTop 是否大于等于 0，确保栈不为空
        2. 如果栈不为空，返回栈顶元素 valStack[valTop] 的值，并将栈顶指针 valTop 自减 1，表示栈中减少了一个元素
        3. 如果栈为空，返回 0.0 作为默认值      
        */ 
        return valStack[valTop--];
        // 先返回栈顶元素的值，然后栈顶指针减一
    }
    return 0.0;
}

double val_top_val() {
    return valStack[valTop];
    /*
    因为首先入栈的元素在栈底，最后入栈的元素在栈顶
    所以栈顶元素就是最后入栈的元素
    实现后进先出 (LIFO) 的数据结构特性
    */ 
}

int val_size() {
    return valTop + 1;
    // 栈的大小指的是栈中元素的个数，所以返回栈顶指针加 1
    // 因为栈顶指针从 -1 开始，所以栈的大小是 valTop + 1
}

// 运算符栈
char opStack[MAX_STACK];
// 用于存储运算符的栈
int opTop = -1;

void op_push(char op) {
    opStack[++opTop] = op;
}

void op_pop() {
    if (opTop >= 0) {
        opTop--;
    }
}

char op_top_val() {
    return opStack[opTop];
}

int op_size() {
    return opTop + 1;
}

// 清空栈（为下一组数据做准备）
void clear_stacks() {
    valTop = -1;
    opTop = -1;
}

// --- 优先级定义 ---
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    // 短路求值：规则： 如果左边的条件 A 已经是 真 (true) 了，
    // 那么 C++ 编译器根本不会去执行右边的 B。因为无论 B 是什么，结果都已经确定是真了。
    // 如果运算符不是 + - * /，则返回 0，表示无效运算符
    return 0;
}

// --- 执行运算 ---
void applyOp(char op) {
    // 二元运算就是需要两个操作数
    // 相反，一元运算类似于负号、逻辑非、自增等，只需要一个操作数
    if (val_size() < 2) return;
    // 防御性编程：确保栈中有足够的操作数
    
    double val2 = val_pop();
    double val1 = val_pop();
    // 有先后顺序的，先弹出的是右操作数，后弹出的是左操作数
    
    double result = 0;
    // 初始化结果变量
    switch (op) {
        // 按照运算符执行对应的运算
        case '+': result = val1 + val2; break;
        case '-': result = val1 - val2; break;
        case '*': result = val1 * val2; break;
        case '/': result = val1 / val2; break;
    }
    val_push(result);
    // 将结果压回数值栈，以供后续运算使用
}

// --- 核心求值函数 ---
void evaluateExpression(char* expression) {
    clear_stacks();
    // 每次计算前清空栈
    int len = strlen(expression);
    // 获取表达式长度，这里的字符串是指以 null 结尾的字符数组
    
    // 如果是用 fgets 读取的，可能会包含换行符，需要处理长度或忽略
    if (len > 0 && expression[len-1] == '\n') {
        /*
        这个条件能够实现的原因是题目中声明每个表达式是以换行符结尾的
        fgets 读取一行时会包含换行符 '\n'，所以需要将其去掉
        通过将换行符替换为字符串结束符 '\0'，并将
        */ 
        expression[len-1] = '\0';
        len--;
    }

    for (int i = 0; i < len; ++i) {
        if (expression[i] == ' ') continue;
        // 跳过空格，增加代码健壮性

        // 检查是否为单目负号：
        // 逻辑：当前是'-'，且 (位于开头 OR 前一个是'(' OR 前一个是运算符)
        bool is_unary_minus = false;
        if (expression[i] == '-') {
            if (i == 0 || expression[i-1] == '(' || precedence(expression[i-1]) > 0) {
                is_unary_minus = true;
                // 两个条件满足其一，则视为单目负号
                // 第一个条件是表达式开头
                // 第二个条件是前一个字符是左括号
                // 第三个条件是前一个字符是运算符
            }
        }

        // 1. 遇到数字 或 单目负号
        // 注意：原逻辑是 isdigit 才进，但如果是单目负号也得处理
        if (isdigit(expression[i]) || is_unary_minus) {
            // 如果是单目负号，i 指向 '-'，需要跳过
            
            // 记录符号位
            int sign = 1;
            if (is_unary_minus) {
                sign = -1;
                i++; // 跳过负号，指向数字的第一位
            }

            // 手动解析数字 (替代 stod)
            double val = 0.0;
            // 整数部分
            while (i < len && isdigit(expression[i])) {
                val = val * 10 + (expression[i] - '0');
                // 进位加当前数字，当前数字通过ASCII码转换得到
                i++;
            }
            
            // 如果题目输入包含小数，需要如下处理（原代码用了stod支持小数，这里手动实现支持）
            if (i < len && expression[i] == '.') {
                i++; // 跳过小数点
                double fraction = 0.1;
                while (i < len && isdigit(expression[i])) {
                    // 条件确认了是数字
                    val += (expression[i] - '0') * fraction;
                    // 将当前数字乘以对应的小数位权重加到 val 上
                    fraction /= 10.0;
                    i++;
                    // 因为 for 循环还会 i++，所以这里不需要额外处理
                }
            }
            
            i--; 
            /*
            极其重要的一行。
            因为 WHILE 循环末尾会有 i++
            而上面的 while 循环已经让 i 指向了数字后面的第一个非数字字符（比如运算符）。
            如果不回退一步，外层 for 循环的 i++ 会导致跳过该运算符。
            */ 
            val_push(val * sign);
            // 将解析好的带符号数值入栈。
        }
        // 2. 遇到左括号
        else if (expression[i] == '(') {
            op_push('(');
        }
        // 3. 遇到右括号
        else if (expression[i] == ')') {
            while (op_size() > 0 && op_top_val() != '(') {
                applyOp(op_top_val());
                op_pop();
            }
            if (op_size() > 0) op_pop(); // 弹出 '('
        }
        // 4. 遇到运算符
        else if (precedence(expression[i]) > 0) {
            char current_op = expression[i];
            while (op_size() > 0 && op_top_val() != '(' && precedence(op_top_val()) >= precedence(current_op)) {
                applyOp(op_top_val());
                op_pop();
                // 栈不空 AND 栈顶不是 ( AND 栈顶运算符优先级 >= 当前运算符优先级。
                // 如果栈顶是乘法，当前是加法，必须先算出乘法。所以要 applyOp。
            }
            op_push(current_op);
        }
    }

    // 5. 处理剩余运算
    while (op_size() > 0) {
        applyOp(op_top_val());
        op_pop();
    }

    // 6. 输出结果
    if (val_size() > 0) {
        printf("%.2f\n", val_top_val());
    }
}

int main() {
    int n;
    // 使用 scanf 读取 n
    if (scanf("%d", &n) != 1) return 0;
    
    // 消耗掉 n 后面的换行符
    char buffer[MAX_LEN];
    fgets(buffer, MAX_LEN, stdin); 

    for (int i = 0; i < n; ++i) {
        // 使用 fgets 读取整行
        if (fgets(buffer, MAX_LEN, stdin) == NULL) break;
        evaluateExpression(buffer);
    }
    
    return 0;
}
/*
全部函数的时空复杂度分析：
1. 栈操作函数 (val_push, val_pop, op_push, op_pop, etc.)
   - 时间复杂度：O(1) 每个栈操作都是常数时间操作
   - 空间复杂度：O(1) 使用固定大小的栈空间
2. 优先级函数 (precedence)
   - 时间复杂度：O(1) 直接通过条件判断返回优先级
    - 空间复杂度：O(1) 无额外空间使用
3. 运算函数 (applyOp)
    - 时间复杂度：O(1) 每次运算只涉及两个数值和一个运算符
    - 空间复杂度：O(1) 使用固定数量的临时变量
4. 主求值函数 (evaluateExpression)
    - 时间复杂度：O(m) 其中 m 是表达式的长度。每个字符最多被处理两次（一次入栈，一次出栈）
    - 空间复杂度：O(m) 最坏情况下，栈中可能存储所有数字和运算符
*/ 