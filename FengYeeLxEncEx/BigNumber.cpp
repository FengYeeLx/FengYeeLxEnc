#include "BigNumber.h"
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <sstream>
#include <vector>

namespace BigNumberNamespace {

    BigNumber::BigNumber(std::string Value) : Value(std::move(Value)) {
        ValidateInput(this->Value);
        if (this->Value[0] == '-') {
            std::string absValue = this->Value.substr(1);
            absValue = removeLeadingZeros(absValue);
            if (absValue == "0") { this->Value = "0"; }
            else { this->Value = "-" + absValue; }
        } else { this->Value = removeLeadingZeros(this->Value); }
    }

    void BigNumber::ValidateInput(const std::string &Value) {
        if (Value.empty()) {
            throw std::invalid_argument("Invalid input: BigNumber must be initialized with a non-empty string.");
        }
        size_t StartIndex = 0;
        if (Value[0] == '-') {
            if (Value.size() == 1) {
                throw std::invalid_argument("Invalid input: Negative sign must be followed by digits.");
            }
            StartIndex = 1;
        }
        if (Value[StartIndex] == '0' && Value.size() > StartIndex + 1) {
            throw std::invalid_argument("Invalid input: BigNumber should not contain leading zeros.");
        }
        if (!std::all_of(Value.cbegin() + static_cast<std::string::difference_type>(StartIndex), Value.cend(),
                         ::isdigit)) {
            throw std::invalid_argument("Invalid input: BigNumber must be initialized with numeric characters only.");
        }
    }

    BigNumber BigNumber::operator+(const BigNumber &Other) const {
        bool isThisNegative = (Value[0] == '-');
        bool isOtherNegative = (Other.Value[0] == '-');

        std::string absThis = isThisNegative ? Value.substr(1) : Value;
        std::string absOther = isOtherNegative ? Other.Value.substr(1) : Other.Value;

        std::string result;
        if (isThisNegative == isOtherNegative) {
            result = addStrings(absThis, absOther);
            result = removeLeadingZeros(result);
            if (isThisNegative) { return BigNumber("-" + result); }
            else { return BigNumber(result); }
        } else {
            int cmp = compareStrings(absThis, absOther);
            if (cmp >= 0) {
                result = subtractStrings(absThis, absOther);
                result = removeLeadingZeros(result);
                if (result == "0") { return BigNumber("0"); }
                if (isThisNegative) { return BigNumber("-" + result); }
                else { return BigNumber(result); }
            } else {
                result = subtractStrings(absOther, absThis);
                result = removeLeadingZeros(result);
                if (result == "0") { return BigNumber("0"); }
                if (isOtherNegative) { return BigNumber("-" + result); }
                else { return BigNumber(result); }
            }
        }
    }

    BigNumber BigNumber::operator-(const BigNumber &Other) const {
        bool isThisNegative = (Value[0] == '-');
        bool isOtherNegative = (Other.Value[0] == '-');

        std::string absThis = isThisNegative ? Value.substr(1) : Value;
        std::string absOther = isOtherNegative ? Other.Value.substr(1) : Other.Value;

        std::string result;
        if (!isThisNegative && !isOtherNegative) {
            int cmp = compareStrings(absThis, absOther);
            if (cmp >= 0) {
                result = subtractStrings(absThis, absOther);
                result = removeLeadingZeros(result);
                return BigNumber(result);
            } else {
                result = subtractStrings(absOther, absThis);
                result = removeLeadingZeros(result);
                return BigNumber("-" + result);
            }
        } else if (isThisNegative && isOtherNegative) {
            int cmp = compareStrings(absThis, absOther);
            if (cmp >= 0) {
                result = subtractStrings(absThis, absOther);
                result = removeLeadingZeros(result);
                if (result == "0") { return BigNumber("0"); }
                return BigNumber("-" + result);
            } else {
                result = subtractStrings(absOther, absThis);
                result = removeLeadingZeros(result);
                return BigNumber(result);
            }
        } else if (isThisNegative) {
            result = addStrings(absThis, absOther);
            result = removeLeadingZeros(result);
            return BigNumber("-" + result);
        } else {
            result = addStrings(absThis, absOther);
            result = removeLeadingZeros(result);
            return BigNumber(result);
        }
    }

    bool BigNumber::operator<(const BigNumber &Other) const {
        int signThis = (Value[0] == '-') ? -1 : 1;
        int signOther = (Other.Value[0] == '-') ? -1 : 1;

        std::string absThis = (signThis == -1) ? Value.substr(1) : Value;
        std::string absOther = (signOther == -1) ? Other.Value.substr(1) : Other.Value;

        absThis = removeLeadingZeros(absThis);
        absOther = removeLeadingZeros(absOther);

        if (signThis != signOther) { return signThis < signOther; }
        int cmp = compareStrings(absThis, absOther);
        if (cmp == 0) { return false; }
        if (signThis == 1) { return cmp < 0; }
        else { return cmp > 0; }
    }

    bool BigNumber::operator>(const BigNumber &Other) const { return Other < *this; }

    bool BigNumber::operator==(const BigNumber &Other) const { return Value == Other.Value; }

    bool BigNumber::operator!=(const BigNumber &Other) const { return !(*this == Other); }

    bool BigNumber::operator<=(const BigNumber &Other) const { return !(*this > Other); }

    bool BigNumber::operator>=(const BigNumber &Other) const { return !(*this < Other); }

    std::string BigNumber::addStrings(const std::string &num1, const std::string &num2) {
        std::string result;
        int carry = 0;
        int i = static_cast<int>(num1.length()) - 1;
        int j = static_cast<int>(num2.length()) - 1;

        while (i >= 0 || j >= 0 || carry) {
            int digit1 = (i >= 0) ? num1[i--] - '0' : 0;
            int digit2 = (j >= 0) ? num2[j--] - '0' : 0;
            int sum = digit1 + digit2 + carry;
            carry = sum / 10;
            result.push_back(static_cast<char>(sum % 10 + '0'));
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    std::string BigNumber::subtractStrings(const std::string &num1, const std::string &num2) {
        std::string result;
        int borrow = 0;
        int i = static_cast<int>(num1.length()) - 1;
        int j = static_cast<int>(num2.length()) - 1;

        while (i >= 0) {
            int digit1 = num1[i--] - '0' - borrow;
            int digit2 = (j >= 0) ? num2[j--] - '0' : 0;
            if (digit1 < digit2) {
                digit1 += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            int diff = digit1 - digit2;
            result.push_back(static_cast<char>(diff + '0'));
        }

        while (result.length() > 1 && result.back() == '0') { result.pop_back(); }
        std::reverse(result.begin(), result.end());
        return result;
    }

    int BigNumber::compareStrings(const std::string &num1, const std::string &num2) {
        if (num1.length() > num2.length()) return 1;
        if (num1.length() < num2.length()) return -1;
        for (size_t i = 0; i < num1.length(); ++i) {
            if (num1[i] > num2[i]) return 1;
            if (num1[i] < num2[i]) return -1;
        }
        return 0;
    }

    std::string BigNumber::removeLeadingZeros(const std::string &num) {
        size_t pos = num.find_first_not_of('0');
        if (pos != std::string::npos) {
            return num.substr(pos);
        } else {
            return "0";
        }
    }

    std::string BigNumber::ToString() const { return Value; }

    std::string BigNumber::multiplyStringByDigit(const std::string &num, char digit) {
        if (digit == '0' || num == "0") { return "0"; }
        int n = digit - '0';
        std::string result;
        int carry = 0;
        for (int i = static_cast<int>(num.length()) - 1; i >= 0; --i) {
            int prod = (num[i] - '0') * n + carry;
            carry = prod / 10;
            result.push_back(static_cast<char>(prod % 10 + '0'));
        }
        if (carry) { result.push_back(static_cast<char>(carry + '0')); }
        std::reverse(result.begin(), result.end());
        return result;
    }

    std::string BigNumber::multiplyStrings(const std::string &num1, const std::string &num2) {
        if (num1 == "0" || num2 == "0") {
            return "0";
        }
        int len1 = static_cast<int>(num1.size());
        int len2 = static_cast<int>(num2.size());
        std::vector<int> result(len1 + len2, 0);

        for (int i = len1 - 1; i >= 0; --i) {
            int n1 = num1[i] - '0';
            for (int j = len2 - 1; j >= 0; --j) {
                int n2 = num2[j] - '0';
                int sum = n1 * n2 + result[i + j + 1];
                result[i + j + 1] = sum % 10;
                result[i + j] += sum / 10;
            }
        }

        std::string resultStr;
        for (int i: result) {
            if (!(resultStr.empty() && i == 0)) {
                resultStr.push_back(static_cast<char>(i + '0'));
            }
        }
        return resultStr.empty() ? "0" : resultStr;
    }

    BigNumber BigNumber::operator*(const BigNumber &Other) const {
        bool isThisNegative = (Value[0] == '-');
        bool isOtherNegative = (Other.Value[0] == '-');

        std::string absThis = isThisNegative ? Value.substr(1) : Value;
        std::string absOther = isOtherNegative ? Other.Value.substr(1) : Other.Value;

        if (absThis == "0" || absOther == "0") { return BigNumber("0"); }

        std::string result = multiplyStrings(absThis, absOther);
        result = removeLeadingZeros(result);

        if (isThisNegative == isOtherNegative) { return BigNumber(result); }
        else { return BigNumber("-" + result); }
    }

    std::string BigNumber::divideStrings(const std::string &dividend, const std::string &divisor) {
        if (divisor == "0") { throw std::invalid_argument("Division by zero"); }
        if (dividend == "0") { return "0"; }
        if (compareStrings(dividend, divisor) < 0) { return "0"; }

        std::string quotient;
        std::string remainder;

        size_t idx = 0;
        while (idx < dividend.size()) {
            remainder.push_back(dividend[idx++]);
            remainder = removeLeadingZeros(remainder);

            if (compareStrings(remainder, divisor) < 0) {
                quotient.push_back('0');
                continue;
            }

            int low = 0, high = 9, count = 0;
            while (low <= high) {
                int mid = (low + high) / 2;
                std::string product = multiplyStringByDigit(divisor, static_cast<char>(mid + '0'));
                int cmp = compareStrings(product, remainder);
                if (cmp == 0) {
                    count = mid;
                    break;
                } else if (cmp < 0) {
                    count = mid;
                    low = mid + 1;
                } else { high = mid - 1; }
            }

            std::string product = multiplyStringByDigit(divisor, static_cast<char>(count + '0'));
            remainder = subtractStrings(remainder, product);
            remainder = removeLeadingZeros(remainder);

            quotient.push_back(static_cast<char>(count + '0'));
        }

        quotient = removeLeadingZeros(quotient);
        return quotient.empty() ? "0" : quotient;
    }

    BigNumber BigNumber::operator/(const BigNumber &Other) const {
        bool isThisNegative = (Value[0] == '-');
        bool isOtherNegative = (Other.Value[0] == '-');

        std::string absThis = isThisNegative ? Value.substr(1) : Value;
        std::string absOther = isOtherNegative ? Other.Value.substr(1) : Other.Value;

        if (absOther == "0") { throw std::invalid_argument("Division by zero"); }

        std::string result = divideStrings(absThis, absOther);
        result = removeLeadingZeros(result);

        if (result == "0") { return BigNumber("0"); }

        if (isThisNegative == isOtherNegative) { return BigNumber(result); }
        else { return BigNumber("-" + result); }
    }

    std::string BigNumber::modStrings(const std::string &dividend, const std::string &divisor) {
        if (divisor == "0") { throw std::invalid_argument("Division by zero"); }
        if (dividend == "0") { return "0"; }
        if (compareStrings(dividend, divisor) < 0) { return dividend; }

        std::string remainder;
        size_t idx = 0;
        while (idx < dividend.size()) {
            remainder.push_back(dividend[idx++]);
            remainder = removeLeadingZeros(remainder);

            if (compareStrings(remainder, divisor) < 0) { continue; }

            int low = 0, high = 9, count = 0;
            while (low <= high) {
                int mid = (low + high) / 2;
                std::string product = multiplyStringByDigit(divisor, static_cast<char>(mid + '0'));
                int cmp = compareStrings(product, remainder);
                if (cmp == 0) {
                    count = mid;
                    break;
                } else if (cmp < 0) {
                    count = mid;
                    low = mid + 1;
                } else { high = mid - 1; }
            }

            std::string product = multiplyStringByDigit(divisor, static_cast<char>(count + '0'));
            remainder = subtractStrings(remainder, product);
            remainder = removeLeadingZeros(remainder);
        }

        remainder = removeLeadingZeros(remainder);
        return remainder.empty() ? "0" : remainder;
    }

    BigNumber BigNumber::operator%(const BigNumber &Other) const {
        bool isThisNegative = (Value[0] == '-');
        bool isOtherNegative = (Other.Value[0] == '-');

        std::string absThis = isThisNegative ? Value.substr(1) : Value;
        std::string absOther = isOtherNegative ? Other.Value.substr(1) : Other.Value;

        if (absOther == "0") { throw std::invalid_argument("Division by zero"); }

        std::string result = modStrings(absThis, absOther);
        result = removeLeadingZeros(result);

        if (result == "0") { return BigNumber("0"); }

        if (isThisNegative) { return BigNumber("-" + result); }
        else { return BigNumber(result); }
    }


} // namespace BigNumberNamespace