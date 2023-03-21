#include <math.h>
#include "dual_num.h"

dnum::dnum(double _val, double _der_val) : val(_val), der_val(_der_val)
{
}

dnum dnum::operator+(dnum d) const
{
    return dnum(val + d.val, der_val + d.der_val);
}

dnum dnum::operator-(dnum d) const
{
    return dnum(val - d.val, der_val - d.der_val);
}

dnum dnum::operator*(dnum d) const
{
    return dnum(val * d.val, val * d.der_val + d.val * der_val);
}

dnum dnum::operator/(dnum d) const
{
    return dnum(val / d.val, (der_val * d.val - val * d.der_val) / (d.val * d.val));
}

dnum pow(const dnum &d, double n)
{
    return dnum(pow(d.val, n), n * pow(d.val, n - 1) * d.der_val);
}

std::ostream &operator<<(std::ostream &os, const dnum &d)
{
    os << "Value: " << d.val << " Derivate: " << d.der_val;
    return os;
}