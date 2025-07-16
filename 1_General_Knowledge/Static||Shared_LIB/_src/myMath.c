#include <stdio.h>
#include <math.h>
#include "myMath.h"

// Cộng hai số thực
double add(double a, double b) {
    return a + b;
}

// Trừ hai số thực
double subtract(double a, double b) {
    return a - b;
}

// Nhân hai số thực
double multiply(double a, double b) {
    return a * b;
}

// Chia hai số thực (xử lý chia cho 0)
double divide(double a, double b) {
    if (b == 0.0) {
        printf("Lỗi: Chia cho 0 không hợp lệ.\n");
        return 0.0;
    }
    return a / b;
}

// Tính giai thừa n!
unsigned long long factorial(int n) {
    if (n < 0) {
        printf("Lỗi: Giai thừa không xác định cho số âm.\n");
        return 0;
    }
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Tính lũy thừa base^exponent
double power(double base, double exponent) {
    return power(base, exponent); // Sử dụng hàm pow trong math.h
}

// Kiểm tra số nguyên tố
int is_prime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return 0;
    }
    return 1;
}
