namespace MyStudy{
template<typename... Types>
class Tuple;

template<typename Head, typename... Tails>
class Tuple<Head, Tails...> : public Tuple<Tails...>
{
public:
    const Head& getHead() const { return head; }
    Head& getHead() { return head; }

    const Tuple<Tails...>& getTails() const { return *this; }
    Tuple<Tails...>& getTails() { return *this; }

    Tuple(const Head& head, const Tails&... tails) : head(head), Tuple<Tails...>(tails...) {}
    Tuple(Head&& head, Tails&&... tails)
        : head(std::forward<Head>(head)), Tuple<Tails...>(std::forward<Tails>(tails)...) {}
    bool operator == (const Tuple<Head, Tails...>& rval)
    {
        return head == rval.head && getTails() == rval.getTails();
    }
private:
    // Using std::decay_t to remove the reference or char[] to char*
    std::decay_t<Head> head;
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
    static const auto& value(const Tuple<Types...>& tuple)
    {
        return GetAtT<Index - 1>::value(tuple.getTails());
    }

    template<typename... Types>
    static auto& value(Tuple<Types...>& tuple)
    {
        return GetAtT<Index - 1>::value(tuple.getTails());
    }};

template<>
struct GetAtT<0>
{
    template<typename... Types>
    static auto value(const Tuple<Types...>& tuple)
    {
        return tuple.getHead();
    }

    template<typename... Types>
    static auto& value(Tuple<Types...>& tuple)
    {
        return tuple.getHead();
    }
};

template<int Index, typename... Types>
const auto& getValue(const Tuple<Types...>& tuple)
{
    return GetAtT<Index>::value(tuple);
}

template<int Index, typename... Types>
auto& getValue(Tuple<Types...>& tuple)
{
    return GetAtT<Index>::value(tuple);
}

template<typename... Types>
auto makeTuple(Types&&... elements)
{
    return Tuple<std::decay_t<Types>...>(std::forward<Types>(elements)...);
}

}//namespace MyStudy