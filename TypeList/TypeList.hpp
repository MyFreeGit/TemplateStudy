#include <utility>

// Definition for TypeList
template<typename... Types>
class TypeList;

/////////////////////////////   getHead and popHead   /////////////////////////////
template<typename List>
struct GetHeadT;

template<typename First, typename... Rest>
struct GetHeadT<TypeList<First, Rest...>>
{
    using Head = First;
    using Tails = TypeList<Rest...>;
};

// Specialization for empty TypeList. The Head is default void or otherwise
// compile error when use getHead or popFront
template<>
struct GetHeadT<TypeList<>>
{
    using Head = void;
    using Tails = TypeList<>;
};

template<typename List>
using getHead = typename GetHeadT<List>::Head;

template<typename List>
using popFront = typename GetHeadT<List>::Tails;

///////////////////////////    pushFront    /////////////////////////////////////

template<typename List, typename NewElements>
struct PushFrontT;

template<typename... Elements, typename NewElement>
struct PushFrontT<TypeList<Elements...>, NewElement>
{
    using Type = TypeList<NewElement, Elements...>;
};

template<typename List, typename NewElement>
using pushFront = typename PushFrontT<List, NewElement>::Type;

///////////////////////////    getAt    /////////////////////////////////////

using UINT = unsigned int;
template<typename List, UINT index>
struct GetAtT;

template<typename First, typename... Rest, UINT index>
struct GetAtT<TypeList<First, Rest...>, index>
{
    using Head = typename GetAtT<TypeList<Rest...>, index - 1>::Head;
};

template<typename First, typename... Rest>
struct GetAtT<TypeList<First, Rest...>, 0>
{
    using Head = First;
};

template<typename List, UINT index>
using getAt = typename GetAtT<List, index>::Head;

///////////////////////////    ifThenElse    /////////////////////////////////////

template<bool Condiction, typename CondictionIsTrue, typename CondictionIsFalse>
struct IfThenElseT;

template<typename CondictionIsTrue, typename CondictionIsFalse>
struct IfThenElseT<true, CondictionIsTrue, CondictionIsFalse>
{
    using Type = CondictionIsTrue;
};

template<typename CondictionIsTrue, typename CondictionIsFalse>
struct IfThenElseT<false, CondictionIsTrue, CondictionIsFalse>
{
    using Type = CondictionIsFalse;
};

template<bool Condiction, typename TypeWhenCondictionIsTrue, typename TypeWhenCondictionIsFalse>
using ifThenElse = typename IfThenElseT<Condiction, TypeWhenCondictionIsTrue, TypeWhenCondictionIsFalse>::Type;

///////////////////////////    getMaxSizeType    /////////////////////////////////////

template<typename List>
struct GetMaxSizeTypeT;

template<typename First, typename... Rest>
struct GetMaxSizeTypeT<TypeList<First, Rest...>>
{
    using MaxSize = ifThenElse<sizeof(First) >= sizeof(typename GetMaxSizeTypeT<TypeList<Rest...>>::MaxSize),
                               First, typename GetMaxSizeTypeT<TypeList<Rest...>>::MaxSize>;
};

template<>
struct GetMaxSizeTypeT<TypeList<>>
{
    using MaxSize = char;
};

template<typename List>
using getMaxSizeType = typename GetMaxSizeTypeT<List>::MaxSize;

///////////////////////////    isEmpty    /////////////////////////////////////

template<typename List>
class isEmpty : public std::false_type {};

template<>
class isEmpty<TypeList<>> : public std::true_type {};

///////////////////////////    pushBack    /////////////////////////////////////

template<typename List, typename NewElement>
struct PushBackT;

template<typename... Elements, typename NewElement>
struct PushBackT<TypeList<Elements...>, NewElement>
{
    using Type = TypeList<Elements..., NewElement>;
};

template<typename List, typename NewElement>
using pushBack = typename PushBackT<List, NewElement>::Type;

///////////////////////////    pushBack(Method Two)   /////////////////////////////////////

template<typename List, typename NewElement, bool = isEmpty<List>::value>
class PushBackV2;

template<typename List, typename NewElement>
class PushBackV2<List, NewElement, false>
{
    using Head = getHead<List>;
    using Tail = popFront<List>;
    using NewTail = typename PushBackV2<Tail, NewElement>::Type;
public:
    using Type = pushFront<NewTail, Head>;
};

template<typename List, typename NewElement>
class PushBackV2<List, NewElement, true>
{
public:
    using Type = pushFront<List, NewElement>;
};

template<typename List, typename NewElement>
using pushBackV2 = typename PushBackV2<List, NewElement>::Type;

///////////////////////////    transform   /////////////////////////////////////

template<typename List, template<typename T>class MetaFunc,
        bool = isEmpty<List>::value>
struct TransformT;

template<typename List, template<typename T>class MetaFunc>
struct TransformT<List, MetaFunc, false>
{
    using Type = pushFront<typename TransformT<popFront<List>, MetaFunc>::Type,
                           MetaFunc<getHead<List>>>;
};

template<typename List, template<typename T>class MetaFunc>
struct TransformT<List, MetaFunc, true>
{
    using Type = List;
};

template<typename List, template<typename T>class MetaFunc>
using transform = typename TransformT<List, MetaFunc>::Type;

///////////////////////////    accumulate   /////////////////////////////////////

template<typename List, template<typename T1, typename T2> class MetaFunc,
         typename InitType, bool = isEmpty<List>::value>
struct AccumulateT;

template<typename List, template<typename T1, typename T2> class MetaFunc, typename InitType>
struct AccumulateT<List, MetaFunc, InitType, false>
{
    using Type = typename AccumulateT<popFront<List>, MetaFunc,
                                      MetaFunc<InitType, getHead<List>>>::Type;
};

template<typename List, template<typename T1, typename T2> class MetaFunc, typename InitType>
struct AccumulateT<List, MetaFunc, InitType, true>
{
    using Type = InitType;
};

template<typename List, template<typename T1, typename T2> class MetaFunc, typename InitType = getHead<List>>
using accumulate = typename AccumulateT<List, MetaFunc, InitType>::Type;

///////////////////////////    insertSort   /////////////////////////////////////
// Avoid Intantiate the T in ifThenElse when if evaluation is false. Due to template instantiation is
// different with if statement, all template parameter are instantiated when template is instantiated!
template<typename T>
struct DoNotInstantiateInIfThenElse
{
    using Type = T;
};

// Define a metafunction to insert a item into a sorted list.
template<typename List, typename NewItem, template<typename T1, typename T2> class Comp,
         bool = isEmpty<List>::value>
class InsertSortT;

template<typename List, typename NewItem, template<typename T1, typename T2> class Comp>
class InsertSortT<List, NewItem, Comp, false>
{
    using NewTail = typename ifThenElse<Comp<NewItem, getHead<List>>::Value,
                                        DoNotInstantiateInIfThenElse<List>,  // Tail is List, Head is NewItem
                                        InsertSortT<popFront<List>, NewItem, Comp> // Insert the NewItem into tail and head is List's head
                                       >::Type;
    using NewHead = ifThenElse<Comp<NewItem, getHead<List>>::Value,
                               NewItem,  // Tail is List, Head is NewItem
                               getHead<List> // Insert the NewItem into tail and head is List's head
                              >;
public:
    using Type = pushFront<NewTail, NewHead>;
};

template<typename List, typename NewItem, template<typename T1, typename T2> class Comp>
class InsertSortT<List, NewItem, Comp, true>
{
public:
    using Type = pushFront<List, NewItem>;
};

template<typename List, typename NewItem, template<typename T1, typename T2> class Comp>
using insertSort = typename InsertSortT<List, NewItem, Comp>::Type;

// Define a Metafunction that Insert the head item into the tail list by insertSort metaFunction.
template<typename List, template<typename T1, typename T2> class Comp, bool = isEmpty<List>::value>
struct SortListT;

template<typename List, template<typename T1, typename T2> class Comp>
struct SortListT<List, Comp, false>
{
    using Type = insertSort<typename SortListT<popFront<List>, Comp>::Type, getHead<List>, Comp>;
};

template<typename List, template<typename T1, typename T2> class Comp>
struct SortListT<List, Comp, true>
{
    using Type = List;
};
    // We can use the public to insted of using template alias in the following way:
    // template<typename List, template<typename T1, typename T2> class Comp>
    // struct SortListT<List, Comp, false>
    //   : public InsertSortT<typename SortListT<popFront<List>, Comp>::Type, getHead<List>, Comp>
    // {
    // };

    // template<typename List, template<typename T1, typename T2> class Comp>
    // struct SortListT<List, Comp, true>
    // {
    //     using Type = List;
    // };
template<typename List, template<typename T1, typename T2> class Comp>
using sortList = typename SortListT<List, Comp>::Type;

///////////////////////////    getFirstIndexOf   /////////////////////////////////////

template<typename T, typename List, int Index=0, bool = isEmpty<List>::value>
struct GetFirstIndexOfT {};

template<typename T, typename List, int Index>
struct GetFirstIndexOfT<T, List, Index, false>
{
    constexpr static unsigned value = ifThenElse<std::is_same<T, getHead<List>>::value,
                                                 std::integral_constant<unsigned, Index>,
                                                 GetFirstIndexOfT<T, popFront<List>, Index + 1>
                                                >::value;
};

template<typename T, typename List, int Index>
struct GetFirstIndexOfT<T, List, Index, true>
{
    // We don't need to add a default value here to indicate the desired type isn't in typelist.
    // Because when instantiation the empty typelist here, means the type isn't in typelist. The
    // compilation will failed due to no value is defined. So we don't need to add a default value
    // like runtime programming here to indicate the error. The compilation will guarantee the clinet
    // code to get the correct value or compilation failure.
    // So below code isn't needed here:
    // constexpr static int value = -1
};

template<typename T, typename List>
constexpr static unsigned getFirstIndexOf = GetFirstIndexOfT<T, List>::value;

///////////////////////////    getLengthOf   /////////////////////////////////////

template<typename List>
struct GetLengthOfT{};

template<typename...Types>
struct GetLengthOfT<TypeList<Types...>>
{
    constexpr static std::size_t length = sizeof...(Types);
};

template<typename List>
constexpr static std::size_t getLengthOf = GetLengthOfT<List>::length;

///////////////////////////    getIntegralList   /////////////////////////////////////

template<typename T, int Start=0, int Step=0, unsigned Count=1,
         typename = std::enable_if<(Count >= 0)>>
struct GetIntegralListT
{
    using Type = pushFront<typename GetIntegralListT<T, Start + Step, Step, Count - 1>::Type,
                                   std::integral_constant<T, Start>>;
};

template<typename T, int Start, int Step>
struct GetIntegralListT<T, Start, Step, 0>
{
    using Type = TypeList<>;
};

template<typename T, int Start, int Step, int Count>
using getIntegralList = typename GetIntegralListT<T, Start, Step, Count>::Type;

///////////////////////////    reverse   /////////////////////////////////////

template<typename List, bool = isEmpty<List>::value>
class ReverseT;

template<typename List>
class ReverseT<List, false>
{
    using Head = getHead<List>;
public:
    using Type = pushBack<typename ReverseT<popFront<List>>::Type, Head>;
                      //   ^- Don't forget this because metafunction's pushBack parameter is a type
};

template<typename List>
class ReverseT<List, true>
{
public:
    using Type = List;
};

template<typename List>
using reverse = typename ReverseT<List>::Type;

///////////////////////////    reverseV2   /////////////////////////////////////
// You can compare those two implementation of reverse. They use different trick.
// In this version, we generate a list that contains the integral value list with a descendent order indicating
// the reversed TypeList's item's index. Then use the metafuction getAt to get each item from the list and generate
// the new List. It isn't use recussion such as previous implementation.
template<typename List, typename ConstList>
struct GetTypeListWithIndexT{};

// Here is the trick for force the compiler to performs the parameter argument deduction by template specialization
// In the general template definition, we define the number of template parameter of GetTypeListWithIndexT. In the
// below the template specialization, we define another set of template parameters which need to be deduced from 
// paramter argument that user given. We can use this kind of trick to extract designed information from specified
// template parameters from another template. Here is we extract the int list which encapsulate in the list which
// metafunction "getIntegralList" generated!
template<typename List, unsigned... Indexes>
struct GetTypeListWithIndexT<List, TypeList<std::integral_constant<unsigned, Indexes>...>>
{
    using Type = TypeList<getAt<List, Indexes>...>;
};

template<typename List>
struct ReverseTypeListT{};

template<typename... Types>
struct ReverseTypeListT<TypeList<Types...>>
{
    using List = TypeList<Types...>;
    using Indexes = getIntegralList<unsigned, sizeof...(Types) - 1, -1, sizeof...(Types)>;
    using Type = typename GetTypeListWithIndexT<List, Indexes>::Type;
};

template<typename List>
using reverseV2 = typename ReverseTypeListT<List>::Type;

///////////////////////////    findCompatibleType   /////////////////////////////////////
// To find a type in the first type which the argument T is same with or can be converted to
// If the type can be converted to and the same are all found, the same type's index is prefered.
// If no type can be found, the compilation is stopped by static assert.
template<int Index>
struct FindConvertableWrapperT
{
    constexpr static int findConvertable = Index;
};

template<typename T, typename List, int Index=0, bool = isEmpty<List>::value>
struct FindCompatibleTypeT;

template<typename T, typename List, int Index>
struct FindCompatibleTypeT<T, List, Index, false>
{
    // Two same type are always convertable. So check the TypeList contains same type first.
    constexpr static int value = ifThenElse<std::is_same<T, getHead<List>>::value,
                                            std::integral_constant<int, Index>,
                                            FindCompatibleTypeT<T, popFront<List>, Index + 1>
                                           >::value;
    // Only to find the convertable type when the same type isn't found
    constexpr static int findConvertable = 
                    ifThenElse<std::is_convertible<T, getHead<List>>::value && value == -1,
                               FindConvertableWrapperT<Index>,
                               FindCompatibleTypeT<T, popFront<List>, Index + 1>
                              >::findConvertable;
};

template<typename T, typename List, int Index>
struct FindCompatibleTypeT<T, List, Index, true>
{
    constexpr static int value = -1;
    constexpr static int findConvertable = -1;
};

template<typename T, typename List>
constexpr static int _find_same = FindCompatibleTypeT<T, List>::value;

template<typename T, typename List>
constexpr static int _find_convertable = FindCompatibleTypeT<T, List>::findConvertable;

template<typename T, typename List>
constexpr int findCompatibleType() 
{
    constexpr int s = _find_same<T, List>;
    constexpr int c = _find_convertable<T, List>;
    static_assert(s != -1 || c != -1, "Cannot find compatible Type!");
    return (s == -1) ? c : s; 
};