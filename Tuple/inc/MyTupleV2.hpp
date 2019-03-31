/*
The version Tuple use more techincal to improve the performance of the tuple.
*/
namespace MyStudy{
template<typename... Types>
class Tuple;

// Use TupleImpl as base class for Tuple to wrapper the data value. We 
// can use dynamic_cast from Tuple to TupleImpl to fetch the data value. The template parameter
// N is for make the all TupleImpl distinct (N is in descendant order) even the wrapped data's
// type is same.
template<unsigned N, typename T>
class TupleImpl
{
public:
    TupleImpl(const T& v):value{v} {}
    TupleImpl(T&& v):value{std::forward<T>(v)} {}
    const T& getValue() const { return value; }
private:
    T value;
};

template<typename Head, typename... Tails>
class Tuple<Head, Tails...> : public TupleImpl<sizeof...(Tails), Head>,  public Tuple<Tails...>
{
public:
    Tuple<Tails...> getTails() const { return *this; }

    // Constructor for LValue reference
    Tuple(const Head& head, const Tails&... tails) : HEAD(head), Tuple<Tails...>(tails...) {}
    // Constructor for RValue reference
    Tuple(Head&& head, Tails&&... tails)
        : HEAD(std::forward<Head>(head)), Tuple<Tails...>(std::forward<Tails>(tails)...) {}

    bool operator == (const Tuple<Head, Tails...>& rval)
    {
        return getHead() == rval.getHead() && getTails() == rval.getTails();
    }
private:
    using HEAD = TupleImpl<sizeof...(Tails), Head>;
    Head getHead() const { return dynamic_cast<const HEAD *>(this)->getValue(); }
    Head getHead() { return dynamic_cast<HEAD *>(this)->getValue(); }
};

template<>
class Tuple<>
{
public:
    // Empty tuple is equal to eachother
    bool operator == (const Tuple<>& rval)
    {
        return true;
    }
};

// getValueFromTupleImpl is helper function shall only be called by getValue.
// The only purpose of this function cause cast from Tuple to one of its base class TupleImpl.
// The tricky here is: When Pos is given, the compiler can deduce the Type of T, because the Pos is distinct
// among the TupleImpl classes. When the cast is successful, we can get value from the TupleImpl classes.
template<unsigned Pos, typename T>
auto getValueFromTupleImpl(TupleImpl<Pos, T> t)
{
    return t.getValue();
}

// This version of getValue is constant time for compiler to get correct value, no recussion is needed.
template<unsigned Index, typename... Types>
auto getValue(const Tuple<Types...>& tuple)
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