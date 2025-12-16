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

void val_push(double x) {
    valStack[++valTop] = x;
}

double val_pop() {
    if (valTop >= 0) {
        return valStack[valTop--];
    }
    return 0.0;
}

double val_top_val() {
    return valStack[valTop];
}

int val_size() {
    return valTop + 1;
}

// 运算符栈
char opStack[MAX_STACK];
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
    return 0;
}

// --- 执行运算 ---
void applyOp(char op) {
    if (val_size() < 2) return;
    
    double val2 = val_pop();
    double val1 = val_pop();
    
    double result = 0;
    switch (op) {
        case '+': result = val1 + val2; break;
        case '-': result = val1 - val2; break;
        case '*': result = val1 * val2; break;
        case '/': result = val1 / val2; break;
    }
    val_push(result);
}

// --- 核心求值函数 ---
void evaluateExpression(char* expression) {
    clear_stacks();
    int len = strlen(expression);
    
    // 如果是用 fgets 读取的，可能会包含换行符，需要处理长度或忽略
    if (len > 0 && expression[len-1] == '\n') {
        expression[len-1] = '\0';
        len--;
    }

    for (int i = 0; i < len; ++i) {
        if (expression[i] == ' ') continue;

        // 检查是否为单目负号：
        // 逻辑：当前是'-'，且 (位于开头 OR 前一个是'(' OR 前一个是运算符)
        bool is_unary_minus = false;
        if (expression[i] == '-') {
            if (i == 0 || expression[i-1] == '(' || precedence(expression[i-1]) > 0) {
                is_unary_minus = true;
            }
        }

        // 1. 遇到数字 或 单目负号
        // 注意：原逻辑是 isdigit 才进，但如果是单目负号也得处理
        if (isdigit(expression[i]) || is_unary_minus) {
            
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
                i++;
            }
            
            // 如果题目输入包含小数，需要如下处理（原代码用了stod支持小数，这里手动实现支持）
            if (i < len && expression[i] == '.') {
                i++; // 跳过小数点
                double fraction = 0.1;
                while (i < len && isdigit(expression[i])) {
                    val += (expression[i] - '0') * fraction;
                    fraction /= 10.0;
                    i++;
                }
            }
            
            i--; // 回退一步，因为for循环还会i++
            val_push(val * sign);
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