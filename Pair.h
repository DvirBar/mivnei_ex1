#ifndef PAIR_H_
#define PAIR_H_

template <class T, class K>
class Pair
{
public:
    Pair(const T &value, const K &key);
    Pair(const Pair &pair) = default;
    Pair &operator=(const Pair &pair) = default;

    K &key() const;
    T &value() const;

private:
    K m_key;
    T m_value;
};

template <class T, class K>
Pair<T, K>::Pair(const T &value, const K &key) : m_key(key),
                                                 m_value(value)
{
}

template <class T, class K>
K &Pair<T, K>::key() const
{
    return m_key;
}

template <class T, class K>
T &Pair<T, K>::value() const
{
    return m_value;
}

#endif // PAIR_H_