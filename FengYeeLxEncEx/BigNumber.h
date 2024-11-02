// BigNumber.h
// Created by FengYeeLx on 2024-11-02.

#ifndef BIGNUMBER_HPP
#define BIGNUMBER_HPP

#include <string>

namespace BigNumberNamespace {

    class BigNumber {
    public:
        explicit BigNumber(std::string Value);

        BigNumber operator+(const BigNumber &Other) const;

        BigNumber operator-(const BigNumber &Other) const;

        BigNumber operator*(const BigNumber &Other) const;

        BigNumber operator/(const BigNumber &Other) const;

        BigNumber operator%(const BigNumber &Other) const;

        bool operator<(const BigNumber &Other) const;

        bool operator>(const BigNumber &Other) const;

        bool operator==(const BigNumber &Other) const;

        bool operator!=(const BigNumber &Other) const;

        bool operator<=(const BigNumber &Other) const;

        bool operator>=(const BigNumber &Other) const;

        [[nodiscard]] std::string ToString() const;

    private:
        std::string Value;

        static std::string removeLeadingZeros(const std::string &Num);

        static void ValidateInput(const std::string &Value);

        static std::string addStrings(const std::string &num1, const std::string &num2);

        static std::string subtractStrings(const std::string &num1, const std::string &num2);

        static int compareStrings(const std::string &num1, const std::string &num2);

        static std::string multiplyStringByDigit(const std::string &num, char digit);

        static std::string multiplyStrings(const std::string &num1, const std::string &num2);

        static std::string divideStrings(const std::string &dividend, const std::string &divisor);

        static std::string modStrings(const std::string &dividend, const std::string &divisor);

    };

} // namespace BigNumberNamespace

#endif // BIGNUMBER_HPP