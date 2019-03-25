namespace MyStudy{
template<typename... Types>
class Tuple;

template<typename First, typename... Rest>
class Tuple<First, Rest...> : public Tuple<Rest...>
{
public:
    First getHead() const { return head; }
    Tuple<Rest...> getTails() const { return *this; }
    Tuple(const First& head, const Rest&... args) : head(head), Tuple<Rest...>(args...) {}
    Tuple(First&& head, Rest&&... args)
        : head(std::forward<First>(head)), Tuple<Rest...>(std::forward<Rest>(args)...) {}
    bool operator == (const Tuple<First, Rest...>& rval)
    {
        return head == rval.head && getTails() == rval.getTails();
    }
private:
    First head;
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