namespace MyStudy{
template<typename... Types>
class Tuple;

template<typename Head, typename... Tails>
class Tuple<Head, Tails...> : public Tuple<Tails...>
{
public:
    Head getHead() const { return head; }
    Tuple<Tails...> getTails() const { return *this; }
    Tuple(const Head& head, const Tails&... tails) : head(head), Tuple<Tails...>(tails...) {}
    Tuple(Head&& head, Tails&&... tails)
        : head(std::forward<Head>(head)), Tuple<Tails...>(std::forward<Tails>(tails)...) {}
    bool operator == (const Tuple<Head, Tails...>& rval)
    {
        return head == rval.head && getTails() == rval.getTails();
    }
private:
    Head head;
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

template<int Index>
struct GetAtT
{
    template<typename... Types>
    static auto value(const Tuple<Types...>& tuple)
    {
        return GetAtT<Index - 1>::value(tuple.getTails());
    }
};

template<>
struct GetAtT<0>
{
    template<typename... Types>
    static auto value(const Tuple<Types...>& tuple)
    {
        return tuple.getHead();
    }
};

template<int Index, typename... Types>
auto getValue(const Tuple<Types...>& tuple)
{
    return GetAtT<Index>::value(tuple);
}

template<typename... Types>
auto makeTuple(Types&&... elements)
{
    return Tuple<std::decay_t<Types>...>(std::forward<Types>(elements)...);
}

}//namespace MyStudy