#include <type_traits>
/*
The version Tuple use more technical to improve the performance of the tuple.
*/
namespace MyStudy{
template<typename... Types>
class Tuple;

// Generic template
template<unsigned N, typename T, bool = std::is_class<T>::value && !std::is_final<T>::value && std::is_empty<T>::value>
class TupleImpl;

// Use TupleImpl as base class for Tuple to wrapper the data value. We
// can use dynamic_cast from Tuple to TupleImpl to fetch the data value. The template parameter
// N is for make the all TupleImpl distinct (N is in descendant order) even the wrapped data's
// type is same.
template<unsigned N, typename T>
class TupleImpl<N, T, false>
{
public:
    TupleImpl(const T& v):value{v} {}
    TupleImpl(T&& v):value{std::forward<T>(v)} {}
    const T& getValue() const { return value; }
    T& getValue() { return value; }
private:
    T value;
};

// This improvements is for reduce the size of the Tuple class with EBCO. When the T is class that
// has no member data, we use those class as base class, the base class will cost no space. But if
// we take those class as member, this will cost some space for the member data. When T is non-empty
// class, those two implementation has no effect.
template<unsigned N, typename T>
class TupleImpl<N, T, true> : public T
{
public:
    TupleImpl(const T& v):T{v} {}
    TupleImpl(T&& v):T{std::forward<T>(v)} {}
    const T& getValue() const { return *this; }
    T& getValue() { return *this; }
};

template<typename Head, typename... Tails>
class Tuple<Head, Tails...> : private TupleImpl<sizeof...(Tails), Head>,  private Tuple<Tails...>
{
public:
    // Constructor for LValue reference
    Tuple(const Head& head, const Tails&... tails) : HEAD(head), Tuple<Tails...>(tails...) {}
    // Constructor for RValue reference
    Tuple(Head&& head, Tails&&... tails)
        : HEAD(std::forward<std::decay_t<Head>>(head)), Tuple<Tails...>(std::forward<Tails>(tails)...) {}
    // Copy Contructor
    Tuple(const Tuple<Head, Tails...>& tuple) : HEAD(tuple.getHead()), TAILS(tuple.getTails()) {}
    // Move Contructor( Need the getHead and getTails return a mutable reference!)
    Tuple(Tuple<Head, Tails...>&& tuple) : HEAD(std::move(tuple.getHead())), 
                                           TAILS(std::move(tuple.getTails())) {}

    bool operator == (const Tuple<Head, Tails...>& rval) const
    {
        // Need the getHead and getTails return a constant reference in the const == operator function
        return getHead() == rval.getHead() && getTails() == rval.getTails();
    }

    // We need to define the friend here, due to the type cast from Tuple to TupleImpl need to access the
    // private base class TupleImpl
    template<unsigned Index, typename... Types>
    friend auto& getValue(const Tuple<Types...>& tuple);
    template<unsigned Index, typename... Types>
    friend auto& getValue(Tuple<Types...>& tuple);
private:
    using HEAD = TupleImpl<sizeof...(Tails), std::decay_t<Head>>;
    using TAILS = Tuple<Tails...>;

    // Need in == operator and getValue (constant version)
    const Head& getHead() const { return dynamic_cast<const HEAD *>(this)->getValue(); }
    // Need in move constructor and getValue (mutable version)
    Head& getHead() { return dynamic_cast<HEAD *>(this)->getValue(); }

    const TAILS& getTails() const { return dynamic_cast<const TAILS&>(*this); }
    TAILS& getTails() {return *this; }
};

template<>
class Tuple<>
{
public:
    // Empty tuple is equal to eachother
    bool operator == (const Tuple<>& rval) const
    {
        return true;
    }
};

// getValueFromTupleImpl is helper function shall only be called by getValue.
// The only purpose of this function cause cast from Tuple to one of its base class TupleImpl.
// The tricky here is: When Pos is given, the compiler can deduce the Type of T, because the Pos is distinct
// among the TupleImpl classes. When the cast is successful, we can get value from the TupleImpl classes.
template<unsigned Pos, typename T>
auto& getValueFromTupleImpl(const TupleImpl<Pos, T>& t)
{
    return t.getValue();
}

// This version of getValue is constant time for compiler to get correct value, no recussion is needed.
template<unsigned Index, typename... Types>
auto& getValue(const Tuple<Types...>& tuple)
{
    return getValueFromTupleImpl<sizeof...(Types) - Index - 1>(tuple);
}

template<unsigned Pos, typename T>
auto& getValueFromTupleImpl(TupleImpl<Pos, T>& t)
{
    return t.getValue();
}

// The overload version of non const reference of tuple is used for get the reference to tuple data.
// Through the reference, we can modify the tuple data.
template<unsigned Index, typename... Types>
auto& getValue(Tuple<Types...>& tuple)
{
    return getValueFromTupleImpl<sizeof...(Types) - Index - 1>(tuple);
}

template<typename... Types>
auto makeTuple(Types&&... elements)
{
    // Tuple only record pure data no reference.
    return Tuple<std::decay_t<Types>...>(std::forward<Types>(elements)...);
}

}//namespace MyStudy