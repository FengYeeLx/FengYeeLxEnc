#include <iostream>
#include "BigNumber.h"

using namespace BigNumberNamespace;

int main() {
    try {
        // 测试加法
        BigNumber num1("100000000000000000000");
        BigNumber num2("99999999999999999999");

        BigNumber sum = num1 + num2;
        std::cout << "Sum: " << sum.ToString() << std::endl; // Expected: "199999999999999999999"

        BigNumber num3("-100000000000000000000");
        BigNumber num4("99999999999999999999");

        BigNumber sum2 = num3 + num4;
        std::cout << "Sum with negative: " << sum2.ToString() << std::endl; // Expected: "-1"

        BigNumber num5("-100000000000000000000");
        BigNumber num6("-99999999999999999999");

        BigNumber sum3 = num5 + num6;
        std::cout << "Sum of two negatives: " << sum3.ToString() << std::endl; // Expected: "-199999999999999999999"

        // 测试减法
        BigNumber diff1 = num1 - num2;
        std::cout << "Difference: " << diff1.ToString() << std::endl; // Expected: "1"

        BigNumber diff2 = num2 - num1;
        std::cout << "Difference (reverse): " << diff2.ToString() << std::endl; // Expected: "-1"

        // 测试乘法
        BigNumber prod1("123456789");
        BigNumber prod2("987654321");
        BigNumber product = prod1 * prod2;
        std::cout << "Product: " << product.ToString() << std::endl; // Expected: "121932631112635269"

        BigNumber prod3("-123456789");
        BigNumber product2 = prod3 * prod2;
        std::cout << "Product with negative: " << product2.ToString() << std::endl; // Expected: "-121932631112635269"

        // 测试除法
        BigNumber div1("121932631112635269");
        BigNumber div2("123456789");
        BigNumber quotient = div1 / div2;
        std::cout << "Quotient: " << quotient.ToString() << std::endl; // Expected: "987654321"

        BigNumber div3("-121932631112635269");
        BigNumber quotient2 = div3 / div2;
        std::cout << "Quotient with negative: " << quotient2.ToString() << std::endl; // Expected: "-987654321"

        // 测试取模
        BigNumber mod1("100000000000000000000");
        BigNumber mod2("3");
        BigNumber remainder = mod1 % mod2;
        std::cout << "Remainder: " << remainder.ToString() << std::endl; // Expected: "1"

        BigNumber mod3("-100000000000000000000");
        BigNumber remainder2 = mod3 % mod2;
        std::cout << "Remainder with negative: " << remainder2.ToString() << std::endl; // Expected: "-1"

        // 测试比较运算符
        BigNumber cmp1("100");
        BigNumber cmp2("200");
        std::cout << "100 < 200: " << (cmp1 < cmp2) << std::endl; // Expected: 1 (true)
        std::cout << "100 > 200: " << (cmp1 > cmp2) << std::endl; // Expected: 0 (false)
        std::cout << "100 == 100: " << (cmp1 == BigNumber("100")) << std::endl; // Expected: 1 (true)
        std::cout << "100 != 100: " << (cmp1 != BigNumber("100")) << std::endl; // Expected: 0 (false)
        std::cout << "100 <= 200: " << (cmp1 <= cmp2) << std::endl; // Expected: 1 (true)
        std::cout << "200 >= 100: " << (cmp2 >= cmp1) << std::endl; // Expected: 1 (true)

    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
