#include <fraction.h>

fraction::fraction(
    big_integer &&numerator,
    big_integer &&denominator)
    : _numerator(std::forward<big_integer>(numerator)),
    _denominator(std::forward<big_integer>(denominator))
{
    if(_denominator.is_equal_to_zero())
    {
        throw std::logic_error("Division by zero will not be tolerated");
    }

    big_integer gcd = big_integer::euclidean(_numerator, _denominator);

    this->_numerator /= gcd;
    this->_denominator /= gcd;
}

fraction::fraction(
    fraction const &other):
    _numerator(other._numerator),
    _denominator(other._denominator)
{

}

fraction::fraction(
        const big_integer &numerator,
        const big_integer &denominator)
        : _numerator(numerator),
        _denominator(denominator)
{

}

fraction &fraction::operator=(
    fraction const &other)
{
    if(*this != other)
    {
        this->_numerator = other._numerator;
        this->_denominator = other._denominator;
    }

    return *this;
}

fraction::fraction(
    fraction &&other) noexcept:
        _numerator(std::move(other._numerator)),
        _denominator(std::move(other._denominator))
{

}

fraction &fraction::operator=(
    fraction &&other) noexcept
{
    if(*this != other)
    {
        this->_numerator = std::move(other._numerator);
        this->_denominator = std::move(other._denominator);
    }

    return *this;

}

fraction &fraction::operator+=(
    fraction const &other)
{
    this->_numerator = this->_numerator * other._denominator + other._numerator * this->_denominator;
    this->_denominator *= other._denominator;

    if(this->_numerator.sign() == -1 && this->_denominator.sign() == -1
       || this->_numerator.sign() == -1 && this->_denominator.sign() == 1)
    {
        this->_numerator.change_sign();
        this->_denominator.change_sign();
    }

    big_integer gcd = big_integer::euclidean(this->_numerator, this->_denominator);
    this->_numerator /= gcd;
    this->_denominator /= gcd;

    return *this;
}

fraction fraction::operator+(
    fraction const &other) const
{
    return fraction(*this) += other;
}

fraction &fraction::operator-=(
    fraction const &other)
{
    this->_numerator = this->_numerator * other._denominator - other._numerator * this->_denominator;
    this->_denominator *= other._denominator;

    if(this->_denominator.sign() == -1 && this->_numerator.sign() == -1 || this->_denominator.sign() == 1 && this->_numerator.sign() == -1)
    {
        this->_numerator.change_sign();
        this->_denominator.change_sign();
    }

    big_integer gcd = big_integer::euclidean(this->_numerator, this->_denominator);
    this->_numerator /= gcd;
    this->_denominator /= gcd;

    return *this;
}

fraction fraction::operator-(
    fraction const &other) const
{
    return fraction(*this) -= other;
}

fraction &fraction::operator*=(
    fraction const &other)
{
    this->_numerator *= other._numerator;
    this->_denominator *= other._denominator;

    if(this->_denominator.sign() == -1 && this->_numerator.sign() == -1 || this->_denominator.sign() == 1 && this->_numerator.sign() == -1)
    {
        this->_numerator.change_sign();
        this->_denominator.change_sign();
    }

    big_integer gcd = big_integer::euclidean(this->_numerator, this->_denominator);
    this->_numerator /= gcd;
    this->_denominator /= gcd;

    return *this;
}

fraction fraction::operator*(
    fraction const &other) const
{
    return fraction(*this) *= other;
}

fraction &fraction::operator/=(
    fraction const &other)
{
    if(this->_denominator.is_equal_to_zero() || other._numerator.is_equal_to_zero())
    {
        throw std::logic_error("Division by zero will not be tolerated");
    }
    this->_numerator *= other._denominator;
    this->_denominator *= other._numerator;

    if(this->_denominator.sign() == -1 && this->_numerator.sign() == -1 || this->_denominator.sign() == 1 && this->_numerator.sign() == -1)
    {
        this->_numerator.change_sign();
        this->_denominator.change_sign();
    }

    big_integer gcd = big_integer::euclidean(this->_numerator, this->_denominator);
    this->_numerator /= gcd;
    this->_denominator /= gcd;

    return *this;
}

fraction fraction::operator/(
    fraction const &other) const
{
    return fraction(*this) /= other;
}

bool fraction::operator==(
    fraction const &other) const
{
    return(this->_numerator == other._numerator && this->_denominator == other._denominator);
}

bool fraction::operator!=(
    fraction const &other) const
{
    return !(*this == other);
}

bool fraction::operator>=(
    fraction const &other) const
{

    big_integer lhs_numerator = (this->_denominator.sign() == -1) ? -this->_numerator : this->_numerator;
    big_integer lhs_denominator = (this->_denominator.sign() == -1) ? -this->_denominator : this->_denominator;

    big_integer rhs_numerator = (other._denominator.sign() == -1) ? -other._numerator : other._numerator;
    big_integer rhs_denominator = (other._denominator.sign() == -1) ? -other._denominator : other._denominator;

    big_integer lhs = lhs_numerator * rhs_denominator;
    big_integer rhs = rhs_numerator * lhs_denominator;

    return lhs >= rhs;
}

bool fraction::operator>(
    fraction const &other) const
{
    //TODO: желаю продуктивно потратить сэкономленные 4 секунды и 10 строчек
    return (*this != other && *this >= other);
}

bool fraction::operator<=(
    fraction const &other) const
{
    return !(*this > other);
}

bool fraction::operator<(
    fraction const &other) const
{
    return (*this != other && *this <= other);
}

std::ostream &operator<<(
    std::ostream &stream,
    fraction const &obj)
{
    stream << obj._numerator << "/" /*<< (obj._denominator.sign() == -1 ? "-" : "+") */<< obj._denominator;

    return stream;
}

std::istream &operator>>(
    std::istream &stream,
    fraction &obj)
{
    char sign;
    std::string numerator;
    std::string denominator;

    stream >> numerator;
    stream.ignore(1, '/');
    stream >> sign >> denominator;

    if(sign == '-')
    {
        denominator = sign + denominator;
    }

    obj = fraction(big_integer(numerator), big_integer(denominator));

    return stream;
}

fraction fraction::abs() const
{
    return
    {
        this->_numerator,
        (this->_denominator.sign() == -1) ? -this->_denominator : this->_denominator
    };
}

fraction fraction::factorial(
        size_t number)
{
    fraction result(big_integer("1"), big_integer("1"));
    for(size_t i = 2; i <= number; ++i)
    {
        big_integer fac(std::to_string(i));
        result._numerator *= fac;
    }

    return result;
}

fraction fraction::sin(
    fraction const &epsilon) const
{
    auto sin_row = [](size_t n, const fraction &current) -> fraction
    {
        fraction term = current.pow(2 * n + 1) / factorial(2 * n + 1);
        if(n & 1)
        {
            term._denominator.change_sign();
        }

        return term;
    };

    fraction sum(big_integer("0"), big_integer("1"));
    fraction current(big_integer("0"), big_integer("1"));

    size_t n = 0;
    do
    {
        current = sin_row(n, *this);
        sum += current;
        ++n;
    }
    while(current.abs() >= epsilon && n <= 100);

    return sum;
}

fraction fraction::cos(
    fraction const &epsilon) const
{
    auto cos_row = [](size_t n, const fraction &current) -> fraction
    {
        fraction term = current.pow(2 * n) / factorial( 2 * n);
        if(n & 1)
        {
            term._denominator.change_sign();
        }
        return term;
    };

    fraction sum(big_integer("0"), big_integer("1"));
    fraction current(big_integer("0"), big_integer("1"));
    size_t n = 0;
    do
    {
        current = cos_row(n, *this);
        sum += current;
        ++n;
    }
    while(current.abs() >= epsilon && n <= 100);


    return sum;
}

fraction fraction::tg(
    fraction const &epsilon) const
{
    return this->sin(epsilon) / this->cos(epsilon);
}

fraction fraction::ctg(
    fraction const &epsilon) const
{
    return this->cos(epsilon) / this->sin(epsilon);
}

fraction fraction::sec(
    fraction const &epsilon) const
{
    fraction result = this->cos(epsilon);


    std::swap(result._numerator, result._denominator);

    if(result._numerator.sign() == -1 && result._denominator.sign() == -1 || result._numerator.sign() == -1 && result._denominator.sign() == 1)
    {
        result._numerator.change_sign();
        result._denominator.change_sign();
    }

    return result;
}

fraction fraction::cosec(
    fraction const &epsilon) const
{
    fraction result = this->sin(epsilon);

    std::swap(result._numerator, result._denominator);

    if(result._numerator.sign() == -1 && result._denominator.sign() == -1 || result._numerator.sign() == -1 && result._denominator.sign() == 1)
    {
        result._numerator.change_sign();
        result._denominator.change_sign();
    }

    return result;
}

fraction fraction::arcsin(
    fraction const &epsilon) const
{
    if(this->abs() > fraction(big_integer("1"),big_integer("1")))
    {
        throw std::logic_error("arcsin is defined on [-1; 1]");
    }

    auto arcsin_row = [](size_t n, const fraction &current) -> fraction
    {
        big_integer numerator("1");
        big_integer denominator("1");

        n = 2 * n + 1;
        for(size_t i = 2; i <= n; ++i)
        {
            big_integer multiplier(std::to_string(i));
            if(i & 1)
            {
                numerator *= multiplier;
            }
            else
            {
                denominator *= multiplier;
            }
        }

        fraction one(std::move(numerator), std::move(denominator));

        fraction two = current.pow(n) / fraction(big_integer(std::to_string(n)), big_integer("1")).pow(2);

        return one * two;
    };

    fraction sum = arcsin_row(0, *this);
    fraction current = arcsin_row(1, *this);
    fraction result = sum + current;

    for(size_t i = 2; current.abs() >= epsilon && i <= 100; i++)
    {
        sum = result;
        current = arcsin_row(i, *this);
        result += current;
    }

    return result;
}

fraction fraction::arccos(
    fraction const &epsilon) const
{
    if(this->abs() > fraction(big_integer("1"),big_integer("1")))
    {
        throw std::logic_error("arccos is defined on [-1; 1]");
    }

    return fraction(big_integer("1"), big_integer("1")).arcsin(epsilon) - this->arcsin(epsilon);
}

fraction fraction::arctg(
    fraction const &epsilon) const
{
    auto arctg_row = [](size_t n, const fraction &current) -> fraction
    {
        fraction result = current.pow(2 * n + 1) / fraction(big_integer(std::to_string(2 * n + 1)), big_integer("1"));
        if( n & 1)
        {
            result._denominator.change_sign();
        }

        return result;
    };

    fraction sum = arctg_row(0, *this);
    fraction current = arctg_row(1, *this);
    fraction result = sum + current;

    for(size_t i = 2; current.abs() > epsilon && i <= 100; ++i)
    {
        sum = result;
        current = arctg_row(i, *this);
        result += current;
    }

    return result;
}

fraction fraction::arcctg(
    fraction const &epsilon) const
{
    return fraction(big_integer("2"), big_integer("1")) * fraction(big_integer("1"), big_integer("1")).arctg(epsilon) - this->arctg(epsilon);
}

fraction fraction::arcsec(
    fraction const &epsilon) const
{//arcsec(x) = arccos(1/x)
    if(this->abs() < fraction(big_integer("1"), big_integer("1")))
    {
        throw std::logic_error("arcsecant is defined on R \\(-1; 1)");
    }

    big_integer numerator = this->_numerator;
    big_integer denominator = this->_denominator;

    if(this->_denominator.sign() == -1)
    {
        numerator.change_sign();
        denominator.change_sign();
    }

    return fraction(std::move(denominator), std::move(numerator)).arccos(epsilon);
}

fraction fraction::arccosec(
    fraction const &epsilon) const
{//arccosec(x) = arcsin(1/x)

    if(this->abs() < fraction(big_integer("1"), big_integer("1")))
    {
        throw std::logic_error("arccosecant is defined on R \\(-1; 1)");
    }

    big_integer numerator = this->_numerator;
    big_integer denominator = this->_denominator;

    if(this->_denominator.sign() == -1)
    {
        numerator = -numerator;
        denominator = -denominator;
    }

    return fraction(std::move(denominator), std::move(numerator)).arcsin(epsilon);
}

fraction fraction::pow(
    size_t degree) const
{
    fraction copy(*this);
    fraction result(big_integer("1"), big_integer("1"));

    while(degree)
    {
        if(degree & 1)
        {
            result *= copy;
        }
        copy *= copy;
        degree >>= 1;
    }

    return result;
}

fraction fraction::root(
    size_t degree,
    fraction const &epsilon) const
{
    if(this->_denominator.sign() == -1 && !(degree & 1))
    {
        throw std::logic_error("computing of even root of negative value will not be tolerated");
    }

    fraction copy(*this);
    bool swapped = false;
    if(copy._numerator > copy._denominator)
    {
        std::swap(copy._numerator, copy._denominator);
        swapped = true;
    }

    fraction alpha = fraction(big_integer("1"), big_integer(std::to_string(degree)));
    copy -= fraction(big_integer("1"), big_integer("1"));

    fraction sum(big_integer("1"), big_integer("1"));
    fraction prev = fraction(big_integer("2"), big_integer("1")) * epsilon;

    fraction precompute(alpha);

    for(size_t i = 1; prev.abs() > epsilon && i <= 100; ++i)
    {
        prev = precompute;
        prev *= copy.pow(i);
        prev /= factorial(i);
        sum += prev;

        precompute *= alpha - fraction(big_integer(std::to_string(i)), big_integer("1"));
    }

    if(swapped)
    {
        std::swap(sum._numerator, sum._denominator);
    }

    return sum;
}

fraction fraction::log2(
    fraction const &epsilon) const
{
    if(this->_denominator.sign() == -1)
    {
        throw std::logic_error("computing of logarithm of negative value will not be tolerated");
    }

    if(this->_numerator == this->_denominator)
    {
        return fraction(big_integer("0"), big_integer("1"));
    }

    return ln(epsilon) / fraction(big_integer("2"), big_integer("1")).ln(epsilon);
}

fraction fraction::ln(
    fraction const &epsilon) const
{
    if(this->_denominator.sign() == -1)
    {
        throw std::logic_error("computing of logarithm of negative value will not be tolerated");
    }
    if(this->_numerator == big_integer("0"))
    {
        throw std::logic_error("logarithm of zero is undefined");
    }

    if(this->_numerator == this->_denominator)
    {
        return fraction(big_integer("0"), big_integer("1"));
    }

    fraction x(*this);
    if(x > fraction(big_integer("1"), big_integer("1")))
    {
        std::swap(x._numerator, x._denominator);
    }
    fraction current_other = x - fraction(big_integer("1"), big_integer("1"));

    auto ln_row = [](size_t n, const fraction &current_local) -> fraction
    {
        fraction sum_local = current_local.pow(n) / fraction(big_integer(std::to_string(n)), big_integer("1"));

        if((n - 1) & 1)
        {
            sum_local._denominator.change_sign();
        }
        return sum_local;
    };

    fraction sum(big_integer("0"), big_integer("1"));
    fraction current(big_integer("1"), big_integer("1"));

    size_t n = 1;
    do
    {
        current = ln_row(n, current_other);
        sum += current;
        n++;
    }
    while(current.abs() >= epsilon && n <= 100);


    if(*this > fraction(big_integer("1"), big_integer("1")))
    {
        sum._denominator.change_sign();
    }

    return sum;
}

fraction fraction::lg(
    fraction const &epsilon) const
{
    if(this->_denominator.sign() == -1)
    {
        throw std::logic_error("computing of logarithm of negative value will not be tolerated");
    }

    if(this->_numerator == this->_denominator)
    {
        return fraction(big_integer("0"), big_integer("1"));
    }

    return ln(epsilon) / fraction(big_integer("10"), big_integer("1")).ln(epsilon);
}