#ifndef COMPARABLE_H
#define COMPARABLE_H

enum CompareResult
//enum class CompareResult
{
    LT = -1,
    EQ = 0,
    GT = 1
};

template <typename T1, typename T2 = T1>
CompareResult compare(const T1 & obj1, const T2 & obj2)
{
    return (obj1==obj2) ? EQ : ( (obj1<obj2) ? LT : GT );
}
template <typename T1, typename T2 = T1>
CompareResult compare(const T1 && obj1, const T2 && obj2)
{
    return (obj1==obj2) ? EQ : ( (obj1<obj2) ? LT : GT );
}

class Comparable
{
public:
    virtual ~Comparable() {}
    virtual CompareResult compare(const Comparable &obj) const = 0;

    ///////////////////////////////////////////////////////////
    virtual bool equals(const Comparable &obj) const
    {
        return compare(obj) == EQ;
    }
    virtual bool operator==(const Comparable &obj) const
    {
        return compare(obj) == EQ;
    }
    virtual bool operator!=(const Comparable &obj) const
    {
        return compare(obj) == EQ;
    }
    virtual bool operator<(const Comparable &obj) const
    {
        return compare(obj) == LT;
    }
    virtual bool operator<=(const Comparable &obj) const
    {
        return compare(obj) <= EQ;
    }
    virtual bool operator>(const Comparable &obj) const
    {
        return compare(obj) == GT;
    }
    virtual bool operator>=(const Comparable &obj) const
    {
        return compare(obj) >= EQ;
    }
};

CompareResult compare(const Comparable & obj1, const Comparable & obj2)
{
    return obj1.compare(obj2);
}
CompareResult compare(const Comparable && obj1, const Comparable && obj2)
{
    return obj1.compare(obj2);
}

#endif
