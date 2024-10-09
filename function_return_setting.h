#ifndef STUDENT_MANAGEMENT_SYSTEM__FUNCTION_RETURN_SETTING_H_
#define STUDENT_MANAGEMENT_SYSTEM__FUNCTION_RETURN_SETTING_H_

#include <climits>

namespace functionreturnsetting {
// 函数成功
constexpr int kDialogSuccess = 1;
// 函数失败
constexpr int kDialogFailed = 0;
// 函数退出
constexpr int kDialogClean = 2;
// 设定一个任何数都不可能达到的值作为初始化变量
constexpr int kIntMax = INT_MAX;
}

#endif //STUDENT_MANAGEMENT_SYSTEM__FUNCTION_RETURN_SETTING_H_
