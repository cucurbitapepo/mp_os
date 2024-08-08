#include <cstring>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <algorithm>
#include "big_integer.h"

void big_integer::clear()
{
    _oldest_digit = 0;
    //delete[] _other_digits;
    deallocate_with_guard(_other_digits);
    _other_digits = nullptr;
}

void big_integer::copy_from(
    big_integer const &other)
{
    _oldest_digit = other._oldest_digit;
    _other_digits = nullptr;
    if (other._other_digits == nullptr)
    {
        return;
    }


    _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), *other._other_digits));
    //_other_digits = new unsigned int[*other._other_digits];
    std::memcpy(_other_digits, other._other_digits, sizeof(unsigned int) * (*other._other_digits));
}

void big_integer::initialize_from(
    int const *digits,
    size_t digits_count)
{
    if (digits == nullptr)
    {
        throw std::logic_error("pointer to digits array must not be nullptr");
    }

    if (digits_count == 0)
    {
        throw std::logic_error("digits array length must  be GT 0");
    }

    _oldest_digit = digits[digits_count - 1];
    _other_digits = (digits_count == 1
        ? nullptr
        //: new unsigned int[digits_count]);
        : reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count)));


    if (_other_digits == nullptr)
    {
        return;
    }

    *_other_digits = (unsigned int)digits_count;

    std::memcpy(_other_digits + 1, digits, sizeof(unsigned int) * (digits_count - 1));
}

void big_integer::initialize_from(
        const std::vector<unsigned int> &digits,
        size_t digits_count)
{
    _other_digits = nullptr;
    if(digits.empty() || digits_count == 0)
    {
        throw std::logic_error("std::vector<unsigned int> of digits should not be empty");
    }

    _oldest_digit = digits.back();

    if(digits_count == 1) return;

    _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    *_other_digits = digits_count;

    for(int i = 0; i < digits_count - 1; ++i)
    {
        _other_digits[i+1] = digits[i];
    }
}

void big_integer::initialize_from(
    std::vector<int> const &digits,
    size_t digits_count)
{
    _other_digits = nullptr;

    if (digits.empty() || digits_count == 0)
    {
        throw std::logic_error("std::vector<int> of digits should not be empty");
    }

    _oldest_digit = digits[digits_count - 1];

    if (digits_count == 1)
    {
        return;
    }

    //_other_digits = new unsigned int[digits_count];
    _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    *_other_digits = digits_count;

    for (auto i = 0; i < digits_count - 1; ++i)
    {
        _other_digits[i+1] = _other_digits[i];
    }
}

void big_integer::initialize_from(
    std::string const &value,
    size_t base)
{
    std::vector<unsigned int> converted = convert_to_base(value, base);
    initialize_from(converted, converted.size());
}

void big_integer::print_byte(
    std::ostream &stream,
    unsigned char byte_value)
{
    for (int i = 0; i < 8; i++)
    {
        stream << ((byte_value >> (7 - i)) & 1);
    }
}

void big_integer::dump_int_value(
    std::ostream &stream,
    int value)
{
    auto *p = (unsigned char *)&value;
    for (int i = 0; i < sizeof(int); i++)
    {
        print_byte(stream, *p++);
        stream << ' ';
    }
}

big_integer &big_integer::change_sign()
{
    _oldest_digit ^= (1 << ((sizeof(int) << 3) - 1));

    return *this;
}

std::vector<unsigned int> big_integer::convert_to_base(const std::string &number_representation, size_t base)
{
    if(number_representation.empty())
    {
        throw std::logic_error("converting empty number somewhere");
    }

    bool negative = false;
    size_t pos = 0;

    if(number_representation[0] == '-')
    {
        negative = true;
        pos = 1;
    }

    auto divide_str_on_int = [](std::string str, size_t pos = 0) -> std::vector<unsigned int>
    {
        std::vector<unsigned int> result;
        size_t converted = 0;
        size_t base = static_cast<size_t>(std::numeric_limits<unsigned int>::max()) + 1;

        while(pos != str.length())
        {
            std::string next_number;

            while(converted < base)
            {
                if(pos == str.length()) break;
                converted = converted * 10 + str[pos] - '0';
                pos++;
            }
            if(pos == str.length())
            {
                if(converted >= base)
                {
                    result.push_back(converted % base);
                    converted /= base;
                }
                result.push_back(converted);
                return result;
            }

            while(pos != str.length())
            {

                if(converted >= base)
                {
                    next_number.push_back(converted / base + '0');
                    converted %= base;
                }
                else next_number.push_back('0');

                if(pos != str.length())
                {
                    converted = converted * 10 + str[pos] - '0';
                }

                pos++;
            }
            if(converted >= base)
            {
                next_number.push_back(converted / base + '0');
                converted %= base;
            }

            else next_number.push_back('0');

            result.push_back(converted);
            str = std::move(next_number);

            converted = 0;
            pos = 0;
        }

        return result;
    };

    std::vector<unsigned int> result = divide_str_on_int(number_representation, pos);

    if((result.back() & (1 << ((sizeof(unsigned int) << 3) - 1))) != 0)
    {
        result.push_back(0);
    }

    if(result.size() == 1 && result.back() == 0)
    {
        return result;
    }

    if(negative)
    {
        result.back() |= 1 << ((sizeof(unsigned int) << 3) - 1);
    }

    return result;
}

inline int big_integer::get_digits_count() const noexcept
{
    return static_cast<int>(_other_digits == nullptr
         ? 1
         : *_other_digits);
}

inline int big_integer::sign() const noexcept
{
    if (is_equal_to_zero())
    {
        return 1;
    }

    return 1 - (static_cast<int>((*reinterpret_cast<unsigned int const *>(&_oldest_digit) >> ((sizeof(int) << 3) - 1))) << 1);
}

inline bool big_integer::is_equal_to_zero() const noexcept
{
    return _oldest_digit == 0 && _other_digits == nullptr;
}

inline unsigned int big_integer::get_digit(
    int position) const noexcept
{
    if (_other_digits == nullptr)
    {
        return position == 0
            ? _oldest_digit
            : 0;
    }

    int const digits_count = get_digits_count();
    if (position < digits_count - 1)
    {
        return _other_digits[position + 1];
    }

    if (position == digits_count - 1)
    {
        return _oldest_digit;
    }

    return 0;
}

allocator *big_integer::get_allocator() const noexcept
{
    return this->_allocator;
}

big_integer::big_integer(
        std::string const &digits,
        allocator *allocator,
        multiplication_rule multiplication_rule,
        division_rule division_rule)
        : _allocator(allocator),
        _multiplication_rule(multiplication_rule),
        _division_rule(division_rule)
{
    initialize_from(digits, digits.size());
}

big_integer::big_integer(
    int const *digits,
    size_t digits_count,
    allocator *allocator,
    multiplication_rule multiplication_rule,
    division_rule division_rule)
    : _allocator(allocator),
      _multiplication_rule(multiplication_rule),
      _division_rule(division_rule)
{
    initialize_from(digits, digits_count);
}

big_integer::big_integer(
    std::vector<int> const &digits,
    allocator *allocator,
    multiplication_rule multiplication_rule,
    division_rule division_rule)
    : _allocator(allocator),
      _multiplication_rule(multiplication_rule),
      _division_rule(division_rule)
{
    remove_leading_zeroes();
    initialize_from(digits, digits.size());
}

big_integer::big_integer(
    std::string const &value,
    size_t base,
    allocator *allocator,
    multiplication_rule multiplication_rule,
    division_rule division_rule)
    : _allocator(allocator),
      _multiplication_rule(multiplication_rule),
      _division_rule(division_rule)
{
    initialize_from(value, base);
}

big_integer::big_integer(
    big_integer const &other)
{
    this->_allocator = other._allocator;
    this->_multiplication_rule = other._multiplication_rule;
    this->_division_rule = other._division_rule;
    copy_from(other);
}

big_integer &big_integer::operator=(
    big_integer const &other)
{
    if (this != &other)
    {
        this->_allocator = other._allocator;
        this->_multiplication_rule = other._multiplication_rule;
        this->_division_rule = other._division_rule;
        clear();
        copy_from(other);
    }

    return *this;
}

big_integer::~big_integer()
{
    clear();
}

void big_integer::remove_leading_zeroes()
{
    if(_other_digits == nullptr)
    {
        return;
    }
    if(_oldest_digit == 0)
    {
        std::vector<unsigned int> new_digits;
        int count = get_digits_count();
        for(int i = 0; i <= count - 1; ++i)
        {
            new_digits.push_back(get_digit(i));
        }

        while(new_digits.back() == 0)
        {
            new_digits.pop_back();
        }

        new_digits.push_back(0);

        clear();

        initialize_from(new_digits, new_digits.size());
    }
}

big_integer big_integer::euclidean(big_integer first, big_integer second)
{
    big_integer tmp("0");

    const big_integer zero("0");

    while(second != zero)
    {
        tmp = first % second;
        first = second;
        second = tmp;
        second.remove_leading_zeroes();
    }

    return first;
}

big_integer &big_integer::operator+=(
    big_integer const &other)
{
    if (other.is_equal_to_zero())
    {
        return *this;
    }

    if (is_equal_to_zero())
    {
        return *this = other;
    }

    if (sign() == -1)
    {
        change_sign();
        *this += -other;
        return change_sign();
    }

    if (other.sign() == -1)
    {
        return *this -= -other;
    }

    auto const first_value_digits_count = get_digits_count();
    auto const second_value_digits_count = other.get_digits_count();
    auto const digits_count = std::max(first_value_digits_count, second_value_digits_count);

    unsigned int operation_result = 0;

    constexpr int shift = sizeof(unsigned int) << 2;
    constexpr int mask = (1 << shift) - 1;

    std::vector<unsigned int> result_digits(digits_count + 1);

    for (int i = 0; i < digits_count; ++i)
    {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);
        result_digits[i] = 0;

        for (int j = 0; j < 2; ++j)
        {
            operation_result += (first_value_digit & mask) + (second_value_digit & mask);
            first_value_digit >>= shift;
            second_value_digit >>= shift;
            result_digits[i] |= ((operation_result & mask) << shift * j);
            operation_result >>= shift;
        }
    }

    while(result_digits.back() == 0)
    {
        result_digits.pop_back();
    }

    if(operation_result)
    {
        result_digits.back() += operation_result;
    }

    if(result_digits.back() & (1 << ((sizeof(unsigned int) << 3) - 1)))
    {
        result_digits.push_back(0);
    }

    size_t result_digits_count = result_digits.size();

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;

    /*result_digits.back() += *reinterpret_cast<int *>(&operation_result);

    auto *maybe_overflowed_digit_ptr = reinterpret_cast<unsigned int *>(&*(result_digits.end() - 2));
    if (*maybe_overflowed_digit_ptr >> ((sizeof(unsigned int) << 3) - 1))
    {
        *maybe_overflowed_digit_ptr ^= (1 << ((sizeof(unsigned int) << 3) - 1));
        ++result_digits.back();
    }

    auto result_digits_count = result_digits.size();
    if (result_digits.back() == 0)
    {
        --result_digits_count;
    }

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;*/
}

big_integer big_integer::operator+(
    big_integer const &other) const
{
    return big_integer(*this) += other;
}

big_integer &big_integer::operator-=(
    big_integer const &other)
{
	if(this->is_equal_to_zero() && !other.is_equal_to_zero())
    {
        big_integer tmp(other);

        tmp.change_sign();
        *this = std::move(tmp);
        return *this;
    }
    else if(!this->is_equal_to_zero() && other.is_equal_to_zero())
    {
        return *this;
    }
    else if(this->sign() == -1 && other.sign() == 1)
    {
        this->change_sign();
        *this += other;
        this->change_sign();
        return *this;
    }
    else if(this->sign() == 1 && other.sign() == -1)
    {
        big_integer tmp(other);
        tmp.change_sign();
        *this += tmp;
        return *this;
    }
    else if(this->sign() == -1 && other.sign() == -1)
    {
        big_integer tmp(other);
        tmp.change_sign();
        tmp += *this;
        *this = std::move(tmp);
        return *this;
    }

    else if(this->sign() == 1 && other.sign() == 1)
    {
        if(*this < other)
        {
            return *this = -(other - *this);
        }
        else
        {
            bool need_to_borrow = false;

            std::vector<unsigned int> result;
            const int count = this->get_digits_count();
            for(int i = 0; i < count; ++i)
            {
                auto number_one = this->get_digit(i);
                auto number_two = i < other.get_digits_count() ? other.get_digit(i) : 0;

                unsigned int operation_result = number_one - number_two - need_to_borrow;

                need_to_borrow = number_one < number_two;

                result.push_back(operation_result);
            }
            big_integer tmp("0");
            while(result.back() == 0)
                result.pop_back();

            if(result.empty())
                result.push_back(0);

            if((result.back() & (1 << ((sizeof(unsigned int) << 3) - 1))) != 0)
                result.push_back(0);

            tmp.initialize_from(result, result.size());
            *this = std::move(tmp);

            return *this;
        }
    }
    else
    {
        *this = std::move(big_integer("0"));
        return *this;
    }

}

big_integer big_integer::operator-(
    big_integer const &other) const
{
    return big_integer(*this) -= other;
}

big_integer big_integer::operator-() const
{
    return big_integer(*this).change_sign();
}

big_integer &big_integer::operator*=(
    big_integer const &other)
{
    return multiply(*this, const_cast<big_integer &>(other), _allocator, _multiplication_rule);
}


big_integer big_integer::operator*(
    big_integer const &other) const
{
    return big_integer(*this) *= other;
}

big_integer &big_integer::operator/=(
    big_integer const &other)
{
    return divide(*this, other, _allocator, _division_rule, _multiplication_rule);
}

big_integer big_integer::operator/(
    big_integer const &other) const
{
    return big_integer(*this) /= other;
}

big_integer &big_integer::operator%=(
    big_integer const &other)
{
    return modulo(*this, other, _allocator, _division_rule, _multiplication_rule);
}

big_integer big_integer::operator%(
    big_integer const &other) const
{
    return big_integer(*this) %= other;
}

bool big_integer::operator==(
    big_integer const &other) const
{
	if(this->sign() != other.sign()) return false;

    int this_size = this->get_digits_count();
    int other_size = other.get_digits_count();

    if(this_size != other_size) return false;

    for(int i = this_size - 1; i >= 0; --i)
    {
        if(this->get_digit(i) != other.get_digit(i)) return false;
    }

    return true;
}

bool big_integer::operator!=(
    big_integer const &other) const
{
	return !(*this == other);
}

bool big_integer::operator<(
    big_integer const &other) const
{
	if(this->sign() != other.sign())
    {
        return this->sign() == -1;
    }
    else
    {
        big_integer other_copy(other);
        big_integer this_copy(*this);

        if(other.sign() == -1)
        {
            other_copy.change_sign();
            this_copy.change_sign();
        }

        int this_size = this->get_digits_count();
        int other_size = other.get_digits_count();

        if(this_size > other_size)
        {
            return this->sign() == -1;
        }
        else if(this_size < other_size)
        {
            return this->sign() == 1;
        }
        else
        {
            for(int i = this_size - 1; i >= 0; --i)
            {
                if(this_copy.get_digit(i) > other_copy.get_digit(i))
                {
                    return this->sign() == -1;
                }
                else if(this_copy.get_digit(i) < other_copy.get_digit(i))
                {
                    return this->sign() == 1;
                }
            }
            return false;
        }
    }
}

bool big_integer::operator<=(
    big_integer const &other) const
{
	//TODO: might not be very optimal, consider copying operator< to here
    return (*this == other || *this < other);
}

bool big_integer::operator>(
    big_integer const &other) const
{
	//TODO: not optimal maybe, as below
    return !(*this <= other);
}

bool big_integer::operator>=(
    big_integer const &other) const
{
	return (*this > other || *this == other);
}

big_integer big_integer::operator~() const
{
    int count = get_digits_count();

    int* new_digits = reinterpret_cast<int *>(allocate_with_guard(sizeof(int), count + 1));

    for(int i = 1; i < count; i++)
    {
        unsigned int current = get_digit(i);
        current = ~current;
        new_digits[i] = *reinterpret_cast<int *>(&current);
    }

    big_integer result(new_digits, *new_digits);
    return result;
}

big_integer &big_integer::operator&=(
    big_integer const &other)
{
    const int this_size = this->get_digits_count();
    const int other_size = other.get_digits_count();

    const int new_size = std::min(this_size, other_size);

    int *new_digits = reinterpret_cast<int *>(allocate_with_guard(sizeof(int), new_size + 1));
    *new_digits = new_size;

    for(int i = 0; i < new_size; ++i)
    {
        if(i != this_size && i != other_size)
        {
            new_digits[i] = this->get_digit(i) & other.get_digit(i);
        }
        else
        {
            new_digits[i] =
                    i == this_size
                    ? other.get_digit(i)
                    : this->get_digit(i);
        }
    }


    this->clear();
    this->initialize_from(new_digits, new_size);

    return *this;
}

big_integer big_integer::operator&(
    big_integer const &other) const
{
	return big_integer(*this) &= other;
}

big_integer &big_integer::operator|=(
    big_integer const &other)
{
    const int this_size = this->get_digits_count();
    const int other_size = other.get_digits_count();

    const int new_size = std::max(this_size, other_size);

    int *new_digits = reinterpret_cast<int *>(allocate_with_guard(sizeof(int), new_size + 1));
    *new_digits = new_size;

    for(int i = 0; i < new_size; ++i)
    {
        if(i != this_size && i != other_size)
        {
            new_digits[i] = this->get_digit(i) | other.get_digit(i);
        }
        else
        {
            new_digits[i] =
                    i == this_size
                    ? other.get_digit(i)
                    : this->get_digit(i);
        }
    }


    this->clear();
    this->initialize_from(new_digits, new_size);

    return *this;
}

big_integer big_integer::operator|(
    big_integer const &other) const
{
	return big_integer(*this) |= other;
}

big_integer &big_integer::operator^=(
    big_integer const &other)
{
    const int this_size = this->get_digits_count();
    const int other_size = other.get_digits_count();

    const int new_size = std::max(this_size, other_size);

    int *new_digits = reinterpret_cast<int *>(allocate_with_guard(sizeof(int), new_size + 1));
    *new_digits = new_size;

    for(int i = 0; i < new_size; ++i)
    {
        if(i != this_size && i != other_size)
        {
            new_digits[i] = this->get_digit(i) ^ other.get_digit(i);
        }
        else
        {
            new_digits[i] =
                    i == this_size
                    ? other.get_digit(i)
                    : this->get_digit(i);
        }
    }


    this->clear();
    this->initialize_from(new_digits, new_size);

    return *this;
}

big_integer big_integer::operator^(
    big_integer const &other) const
{
	return big_integer(*this) ^= other;
}

big_integer &big_integer::operator<<=(
    size_t shift_value)
{
    if (is_equal_to_zero() || shift_value == 0)
    {
        return *this;
    }

    auto value_sign = sign();
    if (value_sign == -1)
    {
        change_sign();
    }

    auto const added_by_shift_at_other_digits_digits_count = shift_value / (sizeof(unsigned int) << 3);
    shift_value %= (sizeof(unsigned int) << 3);

    auto added_by_shift_at_oldest_digit_digits_count = 0;
    if (_oldest_digit != 0)
    {
        unsigned int oldest_digit = *reinterpret_cast<unsigned int *>(&_oldest_digit);
        int oldest_value_bit_index = 0;
        while (oldest_digit != 1)
        {
            oldest_digit >>= 1;
            ++oldest_value_bit_index;
        }

        if (oldest_value_bit_index + shift_value > (sizeof(int) << 3) - 2)
        {
            ++added_by_shift_at_oldest_digit_digits_count;
        }
    }

    if (added_by_shift_at_oldest_digit_digits_count != 0 || added_by_shift_at_other_digits_digits_count != 0)
    {
        auto const added_digits_count = added_by_shift_at_oldest_digit_digits_count + added_by_shift_at_other_digits_digits_count;

        if (_other_digits == nullptr)
        {
            _other_digits = new unsigned int[added_digits_count + 1];
            *_other_digits = added_digits_count + 1;
            std::memset(_other_digits + 1, 0, sizeof(unsigned int) * (added_digits_count - 1));
            if (added_by_shift_at_oldest_digit_digits_count != 0)
            {
                _other_digits[*_other_digits - 1] = _oldest_digit;
                _oldest_digit = 0;
            }
            else
            {
                _other_digits[*_other_digits - 1] = 0;
            }
        }
        else
        {
            auto *new_digits = new unsigned int[added_digits_count + *_other_digits];
            std::memset(new_digits + 1, 0, sizeof(unsigned int) * added_digits_count);
            if (added_by_shift_at_oldest_digit_digits_count != 0)
            {
                new_digits[added_digits_count + *_other_digits - 1] = _oldest_digit;
                _oldest_digit = 0;
            }
            std::memcpy(new_digits + 1 + added_by_shift_at_other_digits_digits_count, _other_digits + 1, sizeof(unsigned int) * (*_other_digits - 1));
            *new_digits = *_other_digits + added_digits_count;

            clear();
            _other_digits = new_digits;
        }
    }

    if (shift_value != 0)
    {
        auto const digits_count = get_digits_count();
        unsigned int part_to_move_to_next_digit = 0;
        for (auto i = 0; i < digits_count; ++i)
        {
            auto digit_value = get_digit(i);
            auto *digit_address = i == digits_count - 1
                ? reinterpret_cast<unsigned int *>(&_oldest_digit)
                : _other_digits + 1 + i;
            *digit_address <<= shift_value;
            *digit_address |= part_to_move_to_next_digit;
            part_to_move_to_next_digit = digit_value >> ((sizeof(unsigned int) << 3) - shift_value);
        }
    }

    if (value_sign == -1)
    {
        change_sign();
    }

    return *this;
}

big_integer big_integer::operator<<(
    size_t shift_value) const
{
    return big_integer(*this) <<= shift_value;
}

big_integer &big_integer::operator>>=(
    size_t shift_value)
{
    if(is_equal_to_zero() || shift_value == 0)
    {
        return *this;
    }

    if(sign() == -1)
    {
        change_sign();
    }

    auto const to_remove_count = shift_value / (sizeof(unsigned int) << 3);
    shift_value %= (sizeof(unsigned int) << 3);

    if(to_remove_count > 0)
    {
        if(to_remove_count >= *_other_digits)
        {
            deallocate_with_guard(_other_digits);
            _other_digits = nullptr;
            _oldest_digit = 0;
            return *this;
        }

        auto new_size = *_other_digits - to_remove_count;
        unsigned int *new_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), new_size + 1));
        std::memcpy(new_digits + 1, _other_digits + 1 + to_remove_count, sizeof(unsigned int) * (new_size - 1));
        *new_digits = new_size;
        deallocate_with_guard(_other_digits);
        _other_digits = new_digits;
    }

    unsigned int part_to_move_to_previous_digit = 0;
    int const digits_count = get_digits_count();
    for(int i = digits_count - 1; i >= 0; --i)
    {
        unsigned int *digit_address =
                (i == digits_count - 1)
                ? reinterpret_cast<unsigned int *>(&_oldest_digit)
                : _other_digits + 1 + i;
        unsigned int current_digit = get_digit(i);
        *(digit_address) >>= shift_value;
        (*digit_address) |= part_to_move_to_previous_digit;
        part_to_move_to_previous_digit = (current_digit << ((sizeof(unsigned int) << 3) - shift_value));
    }

    if(sign() == -1)
    {
        change_sign();
    }

    return *this;
}

big_integer big_integer::operator>>(
    size_t shift_value) const
{
    return big_integer(*this) >>= shift_value;
}

std::ostream &operator<<(
    std::ostream &stream,
    big_integer const &value)
{
    unsigned int max_int = -1;
    size_t base = static_cast<size_t>(max_int) + 1;
    std::string base_str = std::to_string(base);

    big_integer number(value);

    if(value.sign() == -1)
    {
        number.change_sign();
    }

    auto multiply_strings = [](const std::string &num1, const std::string &num2) -> std::string
    {
        int n1 = num1.size();
        int n2 = num2.size();
        std::vector<int> result(n1 + n2, 0);

        std::string num1_rev(num1.rbegin(), num1.rend());
        std::string num2_rev(num2.rbegin(), num2.rend());

        for(int i = 0; i < n1; ++i)
        {
            for(int j = 0; j < n2; ++j)
            {
                result[i+j] += (num1_rev[i] - '0') * (num2_rev[j] - '0');
                result[i+j+1] += result[i+j] / 10;
                result[i+j] %= 10;
            }
        }

        int i = result.size() - 1;
        while(i > 0 && result[i] == 0)
        {
            --i;
        }

        std::string res;
        while(i >= 0)
        {
            res.push_back(result[i--] + '0');
        }
        return res;
    };

    auto sum_two_numbers_in_string = [](std::string& result, std::string& to_add) -> std::string
    {
        std::reverse(result.begin(), result.end());
        std::reverse(to_add.begin(), to_add.end());

        std::string tmp;

        const int size = std::max(result.length(), to_add.length());
        const int size_diff = result.length() - to_add.length();
        if(size_diff != 0)
        {
            size_diff < 0
            ? result.resize(result.length() - size_diff, '0')
            : to_add.resize(to_add.length() + size_diff, '0');

        }
        int next_degree = 0;
        for(int i = 0; i < size; ++i)
        {
            int first_digit = to_add[i] - '0';
            int second_digit = result[i] -'0';
            int sum = first_digit + second_digit + next_degree;
            tmp.push_back((sum % 10) + '0');
            next_degree = sum / 10;
        }

        if(next_degree != 0)
        {
            tmp.push_back('1');
        }
        std::reverse(tmp.begin(), tmp.end());
        return tmp;
    };

    std::string converted;
    const int count = number.get_digits_count();
    for(int i = 0; i < count - 1; ++i)
    {
        unsigned int digit = number.get_digit(i);
        std::string current("1");

        for(int j = 0; j < i; ++j)
        {
            current = multiply_strings(current, base_str);
        }

        current = multiply_strings(current, std::to_string(digit));

        std::string tmp = sum_two_numbers_in_string(converted, current);
        converted = std::move(tmp);
    }

    {
        int last_digit = number._oldest_digit;
        std::string current("1");

        for(int i = 0; i < count - 1; ++i)
        {
            current = multiply_strings(current, base_str);
        }

        current = multiply_strings(current, std::to_string(last_digit));

        std::string tmp = sum_two_numbers_in_string(converted, current);
        converted = std::move(tmp);
    }

    if(value.sign() == -1)
    {
        converted = "-" + converted;
    }
    stream << converted;
    return stream;
}

std::istream &operator>>(
    std::istream &stream,
    big_integer &value)
{
    std::string source;
    stream >> source;
    big_integer tmp(source);
    value = std::move(tmp);
    return stream;
}

big_integer &big_integer::multiply(
        big_integer &first_multiplier,
        big_integer &second_multiplier,
        allocator *allocator,
        big_integer::multiplication_rule multiplication_rule)
{
    std::unique_ptr<multiplication> method;

    switch(multiplication_rule)
    {
        case multiplication_rule::trivial:
        {
            method = std::make_unique<trivial_multiplication>();
            break;
        }
        case multiplication_rule::Karatsuba:
        {
            method = std::make_unique<Karatsuba_multiplication>();
            break;
        }
        case multiplication_rule::SchonhageStrassen:
        {
            method = std::make_unique<Schonhage_Strassen_multiplication>();
            break;
        }
        default:
            throw std::logic_error("unforseen multiplication rule");
    }

    return method->multiply(first_multiplier, const_cast<big_integer const &>(second_multiplier));
}

big_integer big_integer::multiply(
        big_integer const &first,
        big_integer const &second,
        allocator *allocator,
        big_integer::multiplication_rule multiplication_rule)
{
    std::unique_ptr<multiplication> method;

    switch(multiplication_rule)
    {
        case multiplication_rule::trivial:
        {
            method = std::make_unique<trivial_multiplication>();
            break;
        }
        case multiplication_rule::Karatsuba:
        {
            method = std::make_unique<Karatsuba_multiplication>();
            break;
        }
        case multiplication_rule::SchonhageStrassen:
        {
            method = std::make_unique<Schonhage_Strassen_multiplication>();
            break;
        }
        default:
            throw std::logic_error("unforseen multiplication rule");
    }

    big_integer tmp(first);
    return method->multiply(tmp, second);
}

big_integer &big_integer::divide(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator,
        big_integer::division_rule division_rule,
        big_integer::multiplication_rule multiplication_rule)
{
    std::unique_ptr<multiplication> multiplication_method;
    std::unique_ptr<division> division_method;

    switch(multiplication_rule)
    {
        case multiplication_rule::trivial:
        {
            multiplication_method = std::make_unique<trivial_multiplication>();
            break;
        }
        case multiplication_rule::Karatsuba:
        {
            multiplication_method = std::make_unique<Karatsuba_multiplication>();
            break;
        }
        case multiplication_rule::SchonhageStrassen:
        {
            multiplication_method = std::make_unique<Schonhage_Strassen_multiplication>();
            break;
        }
        default:
            throw std::logic_error("unforseen multiplication rule");
    }

    switch(division_rule)
    {
        case division_rule::trivial:
        {
            division_method = std::make_unique<trivial_division>();
            break;
        }
        case division_rule::Newton:
        {
            division_method = std::make_unique<Newton_division>();
            break;
        }
        case division_rule::BurnikelZiegler:
        {
            division_method = std::make_unique<Burnikel_Ziegler_division>();
            break;
        }
        default:
            throw std::logic_error("unforseen division rule");
    }

    return division_method->divide(dividend, const_cast<big_integer const &>(divisor), multiplication_rule);
}

big_integer big_integer::divide(
        big_integer const &dividend,
        big_integer const &divisor,
        allocator *allocator,
        big_integer::division_rule division_rule,
        big_integer::multiplication_rule multiplication_rule)
{
    std::unique_ptr<multiplication> multiplication_method;
    std::unique_ptr<division> division_method;

    switch(multiplication_rule)
    {
        case multiplication_rule::trivial:
        {
            multiplication_method = std::make_unique<trivial_multiplication>();
            break;
        }
        case multiplication_rule::Karatsuba:
        {
            multiplication_method = std::make_unique<Karatsuba_multiplication>();
            break;
        }
        case multiplication_rule::SchonhageStrassen:
        {
            multiplication_method = std::make_unique<Schonhage_Strassen_multiplication>();
            break;
        }
        default:
            throw std::logic_error("unforseen multiplication rule");
    }

    switch(division_rule)
    {
        case division_rule::trivial:
        {
            division_method = std::make_unique<trivial_division>();
            break;
        }
        case division_rule::Newton:
        {
            division_method = std::make_unique<Newton_division>();
            break;
        }
        case division_rule::BurnikelZiegler:
        {
            division_method = std::make_unique<Burnikel_Ziegler_division>();
            break;
        }
        default:
            throw std::logic_error("unforseen division rule");
    }


    big_integer tmp(dividend);
    return division_method->divide(tmp, divisor, multiplication_rule);
}


big_integer &big_integer::modulo(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator,
        big_integer::division_rule division_rule,
        big_integer::multiplication_rule multiplication_rule)
{
    std::unique_ptr<multiplication> multiplication_method;
    std::unique_ptr<division> division_method;

    switch(multiplication_rule)
    {
        case multiplication_rule::trivial:
        {
            multiplication_method = std::make_unique<trivial_multiplication>();
            break;
        }
        case multiplication_rule::Karatsuba:
        {
            multiplication_method = std::make_unique<Karatsuba_multiplication>();
            break;
        }
        case multiplication_rule::SchonhageStrassen:
        {
            multiplication_method = std::make_unique<Schonhage_Strassen_multiplication>();
            break;
        }
        default:
            throw std::logic_error("unforseen multiplication rule");
    }

    switch(division_rule)
    {
        case division_rule::trivial:
        {
            division_method = std::make_unique<trivial_division>();
            break;
        }
        case division_rule::Newton:
        {
            division_method = std::make_unique<Newton_division>();
            break;
        }
        case division_rule::BurnikelZiegler:
        {
            division_method = std::make_unique<Burnikel_Ziegler_division>();
            break;
        }
        default:
            throw std::logic_error("unforseen division rule");
    }

    return division_method->modulo(dividend, const_cast<big_integer const &>(divisor), multiplication_rule);
}

big_integer big_integer::modulo(
        big_integer const &dividend,
        big_integer const &divisor,
        allocator *allocator,
        big_integer::division_rule division_rule,
        big_integer::multiplication_rule multiplication_rule)
{
    std::unique_ptr<multiplication> multiplication_method;
    std::unique_ptr<division> division_method;

    switch(multiplication_rule)
    {
        case multiplication_rule::trivial:
        {
            multiplication_method = std::make_unique<trivial_multiplication>();
            break;
        }
        case multiplication_rule::Karatsuba:
        {
            multiplication_method = std::make_unique<Karatsuba_multiplication>();
            break;
        }
        case multiplication_rule::SchonhageStrassen:
        {
            multiplication_method = std::make_unique<Schonhage_Strassen_multiplication>();
            break;
        }
        default:
            throw std::logic_error("unforseen multiplication rule");
    }

    switch(division_rule)
    {
        case division_rule::trivial:
        {
            division_method = std::make_unique<trivial_division>();
            break;
        }
        case division_rule::Newton:
        {
            division_method = std::make_unique<Newton_division>();
            break;
        }
        case division_rule::BurnikelZiegler:
        {
            division_method = std::make_unique<Burnikel_Ziegler_division>();
            break;
        }
        default:
            throw std::logic_error("unforseen division rule");
    }


    big_integer tmp(dividend);
    return division_method->modulo(tmp, divisor, multiplication_rule);
}

big_integer &big_integer::trivial_multiplication::multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier) const
{
    big_integer copy(second_multiplier);

    bool negative_result;

    if(first_multiplier.sign() ^ copy.sign())
    {
        negative_result = true;
        first_multiplier.sign() == -1 ? first_multiplier.change_sign() : copy.change_sign();
    }
    else if(first_multiplier.sign() == -1 && second_multiplier.sign() == -1)
    {
        first_multiplier.change_sign();
        copy.change_sign();
        negative_result = false;
    }
    else negative_result = false;

    const int this_size = first_multiplier.get_digits_count();
    const int other_size = copy.get_digits_count();

    constexpr int shift = sizeof(unsigned int) << 2;
    constexpr unsigned int mask = (1U << shift) - 1;

    big_integer result("0");

    int count_this = 2 * this_size;
    int count_other = 2 * other_size;
    for(int i = 0; i < count_this; ++i)
    {
        unsigned int rest = 0;

        unsigned int first_number_half;

        if(i%2 == 0)
        {
            unsigned int number = first_multiplier.get_digit(i/2);
            first_number_half = number & mask;
        }
        else
        {
            unsigned int number = first_multiplier.get_digit(i/2);
            first_number_half = (number >> shift) & mask;
        }

        for(int j = 0; j < count_other; ++j)
        {
            std::vector<unsigned int> digits;
            unsigned int second_number_half;
            if(j % 2 == 0)
            {
                unsigned int number = copy.get_digit(j/2);
                second_number_half = number & mask;
            }
            else
            {
                unsigned int number = copy.get_digit(j/2);
                second_number_half = (number >> shift) & mask;
            }

            unsigned int operation_result = (first_number_half * second_number_half + rest) & mask;
            rest = (first_number_half * second_number_half + rest) >> shift;
            digits.push_back(operation_result);

            big_integer multiplication_result("0");
            multiplication_result.initialize_from(digits, digits.size());
            multiplication_result <<= (shift * (i + j));
            result += multiplication_result;
        }

        if(rest)
        {
            std::vector<unsigned int> rest_vector = {rest};
            big_integer addition_rest("0");
            addition_rest.initialize_from(rest_vector, rest_vector.size());

            addition_rest <<= (shift * (2 * other_size + i));
            result += addition_rest;
        }
    }

    first_multiplier = std::move(result);

    if(negative_result)
    {
        first_multiplier.change_sign();
    }

    return first_multiplier;
}

big_integer &big_integer::Karatsuba_multiplication::multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::Karatsuba_multiplication::multiply(big_integer &first_multiplier, big_integer const &second_multiplier) const", "not implemented");
}

big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(big_integer &first_multiplier, big_integer const &second_multiplier) const", "not implemented");
}

big_integer &big_integer::trivial_division::divide(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const
{
    if(divisor.is_equal_to_zero())
    {
        throw std::logic_error("division by zero will not be tolerated.");
    }
    else if (dividend.is_equal_to_zero())
    {
        return dividend = big_integer("0");
    }

    std::stringstream  dividend_str, divisor_str;
    dividend_str << dividend;
    divisor_str << divisor;

    bool negative_result = false;
    big_integer divisor_copy(divisor);

    if(dividend.sign() ^ divisor.sign())
    {
        dividend.sign() == 1 ? divisor_copy.change_sign() : dividend.change_sign();
        negative_result = true;
    }

    else
    {
        if(dividend.sign() == -1)
        {
            divisor_copy.change_sign();
            dividend.change_sign();
        }
    }

    if(dividend._oldest_digit == 1 && dividend._other_digits == nullptr || divisor_copy._oldest_digit == 1 && divisor_copy._other_digits == nullptr)
    {
        if(divisor_copy._oldest_digit == 1 && divisor_copy._other_digits == nullptr)
        {
            if(negative_result)
            {
                dividend.change_sign();
            }
            return dividend;
        }
    }

    {
        big_integer tmp(divisor);
        const big_integer one("1");
        int shift = 0;
        
        while((tmp & one) != one)
        {
            tmp >>= 1;
            shift++;
        }
        
        if(tmp == one)
        {
            dividend >>= shift;
            if(negative_result)
            {
                dividend.change_sign();
            }
            return dividend;
        }
    }
    
    auto compare_vectors = [](std::vector<unsigned int>& vec1, std::vector<unsigned int>& vec2) -> int
    {
        if(!vec1.empty())
        {
            if(vec1.front() == 0)
            {
                std::vector<unsigned int> new_vec;
                int last_zero = 0;
                size_t vec1_size = vec1.size();
                for(size_t i = 1; vec1_size; ++i)
                {
                    if(vec1[i] == 0)
                    {
                        last_zero++;
                    }
                    else break;
                }
                
                for(size_t i = last_zero + 1; i < vec1_size; ++i)
                {
                    new_vec.push_back(vec1[i]);
                }
                
                vec1 = std::move(new_vec);
            }
        }
        
        if(!vec2.empty()) {
            if (vec2.front() == 0) {
                std::vector<unsigned int> new_vec;
                int last_zero = 0;
                size_t vec2_size = vec2.size();
                for (size_t i = 0; i < vec2_size; ++i) {
                    if (vec2[i] == 0) {
                        last_zero++;
                    } else break;
                }

                for (size_t i = last_zero + 1; i < vec2_size; ++i) {
                    new_vec.push_back(vec2[i]);
                }
                vec2 = std::move(new_vec);
            }
        }
        if(vec1.size() > vec2.size())
        {
            return 1;
        }
        else if (vec1.size() < vec2.size())
        {
            return -1;
        }
        else
        {
            size_t vec1_size = vec1.size();
            for(size_t i = 0; i < vec1_size; ++i)
            {
                if(vec1[i] > vec2[i])
                {
                    return 1;
                }
                if(vec1[i] < vec2[i])
                {
                    return -1;
                }
            }
        }
        return 0;
    };

    auto multiply_vector_on_int = [](unsigned int number, std::vector<unsigned int> const & big_number) -> std::vector<unsigned int>
    {
        std::vector<unsigned int> copy(big_number);
        std::vector<unsigned int> result;
        
        unsigned int const max_int = -1;
        size_t base = static_cast<size_t>(max_int) + 1;
        
        std::reverse(copy.begin(), copy.end());
        
        unsigned int rest = 0;

        size_t copy_size = copy.size();
        for(size_t i = 0; i < copy_size; ++i)
        {
            size_t multiplication_result = static_cast<size_t>(copy[i]) * static_cast<size_t>(number) + rest;

            result.push_back(multiplication_result % base);

            rest = multiplication_result / base;
        }

        if(rest)
        {
            result.push_back(rest);
        }

        std::reverse(result.begin(), result.end());
        return result;
    };

    auto swap_or_add_bits = [](unsigned int& number, bool swap, unsigned int& left, unsigned int& right) -> void
    {

        if(left == right)
        {
            number = left;
        }
        else
        {
            size_t mid = static_cast<size_t>(left) + static_cast<size_t>(right);
            mid /= 2;
            if(swap)
            {
                right = number;
                number = mid;
            }
            else
            {
                left = number;
                number = mid + 1;
            }
        }
    };

    std::vector<unsigned int> result;
    std::vector<unsigned int> dividend_vec;
    std::vector<unsigned int> divisor_vec;

    int count = divisor_copy.get_digits_count();
    for(int i = count - 1; i >= 0; --i)
    {
        if(!(divisor_vec.empty() && divisor_copy.get_digit(i) == 0))
        {
            divisor_vec.push_back(divisor_copy.get_digit(i));
        }
    }

    count = dividend.get_digits_count();
    for(int i = count - 1; i >= 0; --i)
    {
        if(compare_vectors(dividend_vec, divisor_vec) == -1)
        {
            if(!(dividend_vec.empty() && dividend.get_digit(i) == 0))
            {
                dividend_vec.push_back(dividend.get_digit(i));
            }
            result.push_back(0);
            if(compare_vectors(dividend_vec, divisor_vec) == -1) continue;
        }

        unsigned int divide_result = 1 << ((sizeof(unsigned int) << 3) - 1);
        unsigned int left = 1;
        unsigned int right = -1;

        while(true)
        {
            std::vector<unsigned int> multiplication_result = multiply_vector_on_int(divide_result, divisor_vec);

            int comparison_result = compare_vectors(multiplication_result, dividend_vec);

            if(comparison_result == 1)
            {
                swap_or_add_bits(divide_result, true, left, right);
            }
            else if(comparison_result == -1)
            {
                big_integer big_dividend("0");
                std::vector<unsigned int> dividend_vec_copy(dividend_vec);
                std::reverse(dividend_vec_copy.begin(), dividend_vec_copy.end());

                if ((dividend_vec_copy.back() & (1 << ((sizeof(unsigned int) << 3) - 1))) != 0)
                {
                    dividend_vec_copy.push_back(0);
                }

                big_dividend.initialize_from(dividend_vec_copy, dividend_vec_copy.size());

                std::reverse(multiplication_result.begin(), multiplication_result.end());
                big_integer big_mult_result("0");

                if ((multiplication_result.back() & (1 << ((sizeof(unsigned int) << 3) - 1))) != 0)
                {
                    multiplication_result.push_back(0);
                }
                big_mult_result.initialize_from(multiplication_result, multiplication_result.size());

                if (multiplication_result.back() == 0)
                {
                    multiplication_result.pop_back();
                }

                big_integer rest = big_dividend - big_mult_result;

                if (rest >= divisor_copy)
                {
                    swap_or_add_bits(divide_result, false, left, right);
                }
                else
                {
                    result.push_back(static_cast<int>(divide_result));
                    dividend_vec.clear();
                    if (!rest.is_equal_to_zero())
                    {
                        int count = rest.get_digits_count();
                        for (int j = count - 1; j >= 0; --j)
                        {
                            dividend_vec.push_back(rest.get_digit(j));
                        }
                    }

                    break;
                }
            }
            else
            {
                result.push_back(static_cast<int>(divide_result));
                std::reverse(dividend_vec.begin(), dividend_vec.end());
                std::reverse(multiplication_result.begin(), multiplication_result.end());

                big_integer number_1("0");
                number_1.initialize_from(dividend_vec, dividend_vec.size());

                big_integer number_2("0");
                number_2.initialize_from(multiplication_result, multiplication_result.size());

                number_1 -= number_2;

                dividend_vec.clear();

                if(!number_1.is_equal_to_zero())
                {
                    int count_ = number_1.get_digits_count();
                    for(int j = count_ - 1; j >= 0; --j)
                    {
                        dividend_vec.push_back(number_1.get_digit(j));
                    }
                }
                break;
            }
        }
        dividend_vec.push_back(dividend.get_digit(i - 1));
    }

    std::reverse(result.begin(), result.end());

    big_integer result_big("0");
    result_big.initialize_from(result, result.size());

    if(negative_result)
    {
        result_big.change_sign();
    }

    dividend = result_big;
    return dividend;
}

big_integer &big_integer::trivial_division::modulo(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const
{
    if(divisor.is_equal_to_zero())
    {
        throw std::logic_error("division by zero will not be tolerated.");
    }
    else if (dividend.is_equal_to_zero())
    {
        return dividend = big_integer("0");
    }

    bool negative_result = false;
    big_integer divisor_copy(divisor);

    if(dividend.sign() ^ divisor.sign())
    {
        dividend.sign() == 1
        ? divisor_copy.change_sign()
        : dividend.change_sign();
        negative_result = true;
    }

    else
    {
        if(dividend.sign() == -1)
        {
            divisor_copy.change_sign();
            dividend.change_sign();
        }
    }

    if(dividend < divisor)
    {
        return dividend;
    }

    if(divisor_copy._oldest_digit == 1 && divisor_copy._other_digits == nullptr)
    {
        if(negative_result)
        {
            dividend.change_sign();
        }
        dividend = big_integer("0");
        return dividend;
    }

    auto compare_vectors = [](std::vector<unsigned int>& vec1, std::vector<unsigned int>& vec2) -> int
    {
        if(!vec1.empty())
        {
            if(vec1.front() == 0)
            {
                std::vector<unsigned int> new_vec;
                int last_zero = 0;
                size_t vec1_size = vec1.size();
                for(size_t i = 1; vec1_size; ++i)
                {
                    if(vec1[i] == 0)
                    {
                        last_zero++;
                    }
                    else break;
                }

                for(size_t i = last_zero + 1; i < vec1_size; ++i)
                {
                    new_vec.push_back(vec1[i]);
                }

                vec1 = std::move(new_vec);
            }
        }

        if(!vec2.empty())
        {
            if (vec2.front() == 0)
            {
                std::vector<unsigned int> new_vec;
                int last_zero = 0;
                size_t vec2_size = vec2.size();
                for (size_t i = 0; i < vec2_size; ++i)
                {
                    if (vec2[i] == 0)
                    {
                        last_zero++;
                    }
                    else break;
                }

                for (size_t i = last_zero + 1; i < vec2_size; ++i)
                {
                    new_vec.push_back(vec2[i]);
                }
                vec2 = std::move(new_vec);
            }
        }
        if(vec1.size() > vec2.size())
        {
            return 1;
        }
        else if (vec1.size() < vec2.size())
        {
            return -1;
        }
        else
        {
            size_t vec1_size = vec1.size();
            for(size_t i = 0; i < vec1_size; ++i)
            {
                if(vec1[i] > vec2[i])
                {
                    return 1;
                }
                if(vec1[i] < vec2[i])
                {
                    return -1;
                }
            }
        }
        return 0;
    };

    auto multiply_vector_on_int = [](unsigned int number, std::vector<unsigned int> const & big_number) -> std::vector<unsigned int>
    {
        std::vector<unsigned int> copy(big_number);
        std::vector<unsigned int> result;

        unsigned int const max_int = -1;
        size_t base = static_cast<size_t>(max_int) + 1;

        std::reverse(copy.begin(), copy.end());

        unsigned int rest = 0;

        size_t copy_size = copy.size();
        for(size_t i = 0; i < copy_size; ++i)
        {
            size_t multiplication_result = static_cast<size_t>(copy[i]) * static_cast<size_t>(number) + rest;

            result.push_back(multiplication_result % base);

            rest = multiplication_result / base;
        }

        if(rest)
        {
            result.push_back(rest);
        }

        std::reverse(result.begin(), result.end());
        return result;
    };

    auto swap_or_add_bits = [](unsigned int& number, bool swap, unsigned int& left, unsigned int& right) -> void
    {

        if(left == right)
        {
            number = left;
        }
        else
        {
            size_t mid = static_cast<size_t>(left) + static_cast<size_t>(right);
            mid /= 2;
            if(swap)
            {
                right = number;
                number = mid;
            }
            else
            {
                left = number;
                number = mid + 1;
            }
        }
    };

    std::vector<unsigned int> result;
    std::vector<unsigned int> dividend_vec;
    std::vector<unsigned int> divisor_vec;

    big_integer rest(dividend);

    int count = divisor_copy.get_digits_count();
    for(int i = count - 1; i >= 0; --i)
    {
        if(!(divisor_vec.empty() && divisor_copy.get_digit(i) == 0))
        {
            divisor_vec.push_back(divisor_copy.get_digit(i));
        }
    }

    count = dividend.get_digits_count();
    for(int i = count - 1; i >= 0; --i)
    {
        if(compare_vectors(dividend_vec, divisor_vec) == -1)
        {
            if(!(dividend_vec.empty() && dividend.get_digit(i) == 0))
            {
                dividend_vec.push_back(dividend.get_digit(i));
            }
            result.push_back(0);
            if(compare_vectors(dividend_vec, divisor_vec) == -1) continue;
        }

        unsigned int divide_result = 1 << ((sizeof(unsigned int) << 3) - 1);
        unsigned int left = 1;
        unsigned int right = -1;

        while(true)
        {
            std::vector<unsigned int> multiplication_result = multiply_vector_on_int(divide_result, divisor_vec);

            int comparison_result = compare_vectors(multiplication_result, dividend_vec);

            if(comparison_result == 1)
            {
                swap_or_add_bits(divide_result, true, left, right);
            }
            else if(comparison_result == -1)
            {
                big_integer big_dividend("0");
                std::vector<unsigned int> dividend_vec_copy(dividend_vec);
                std::reverse(dividend_vec_copy.begin(), dividend_vec_copy.end());

                if ((dividend_vec_copy.back() & (1 << ((sizeof(unsigned int) << 3) - 1))) != 0)
                {
                    dividend_vec_copy.push_back(0);
                }

                big_dividend.initialize_from(dividend_vec_copy, dividend_vec_copy.size());

                std::reverse(multiplication_result.begin(), multiplication_result.end());
                big_integer big_mult_result("0");

                if ((multiplication_result.back() & (1 << ((sizeof(unsigned int) << 3) - 1))) != 0)
                {
                    multiplication_result.push_back(0);
                }
                big_mult_result.initialize_from(multiplication_result, multiplication_result.size());

                if (multiplication_result.back() == 0)
                {
                    multiplication_result.pop_back();
                }

                rest = big_dividend - big_mult_result;

                if (rest >= divisor_copy)
                {
                    swap_or_add_bits(divide_result, false, left, right);
                }
                else
                {
                    result.push_back(static_cast<int>(divide_result));
                    dividend_vec.clear();
                    if (!rest.is_equal_to_zero())
                    {
                        int count = rest.get_digits_count();
                        for (int j = count - 1; j >= 0; --j)
                        {
                            dividend_vec.push_back(rest.get_digit(j));
                        }
                    }

                    break;
                }
            }
            else
            {
                rest = big_integer("0");

                result.push_back(static_cast<int>(divide_result));
                std::reverse(dividend_vec.begin(), dividend_vec.end());
                std::reverse(multiplication_result.begin(), multiplication_result.end());

                big_integer number_1("0");
                number_1.initialize_from(dividend_vec, dividend_vec.size());

                big_integer number_2("0");
                number_2.initialize_from(multiplication_result, multiplication_result.size());

                number_1 -= number_2;

                dividend_vec.clear();

                if(!number_1.is_equal_to_zero())
                {
                    int count_ = number_1.get_digits_count();
                    for(int j = count_ - 1; j >= 0; --j)
                    {
                        dividend_vec.push_back(number_1.get_digit(j));
                    }
                }
                break;
            }
        }
        dividend_vec.push_back(dividend.get_digit(i - 1));
    }

    dividend = rest;
    return dividend;
}

big_integer &big_integer::Newton_division::divide(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Newton_division::divide(big_integer &dividend, big_integer const &divisor, big_integer::multiplication_rule multiplication_rule) const", "not implemented");
}

big_integer &big_integer::Newton_division::modulo(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Newton_division::modulo(big_integer &dividend, big_integer const &divisor, big_integer::multiplication_rule multiplication_rule) const", "not implemented");
}

big_integer &big_integer::Burnikel_Ziegler_division::divide(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::divide(big_integer &dividend, big_integer const &divisor, big_integer::multiplication_rule multiplication_rule) const", "not implemented");
}

big_integer &big_integer::Burnikel_Ziegler_division::modulo(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::modulo(big_integer &dividend, big_integer const &divisor, big_integer::multiplication_rule multiplication_rule) const", "not implemented");
}