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

bool dnum::operator<(dnum d) const
{
    return this->val < d.val;
}

dnum pow(const dnum &d, double n)
{
    return dnum(pow(d.val, n), n * pow(d.val, n - 1) * d.der_val);
}

dnum sqrt(const dnum &d)
{
    return pow(d, 0.5);
}

std::ostream &operator<<(std::ostream &os, const dnum &d)
{
    os << "Value: " << d.val << " Derivate: " << d.der_val;
    return os;
}

dnum std::max(const dnum &d1, const dnum &d2)
{
    return d2;
    return d1.der_val > d2.der_val ? d1 : d2;
}
