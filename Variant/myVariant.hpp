#include <utility>
#include <type_traits>
#include <cstring>
#include <sstream>
#include <exception>
#include "../TypeList/TypeList.hpp"

namespace MyStudy{
constexpr int VARIANT_INVALID_INDEX = -1;
class DataNotInBuffer : public std::invalid_argument
{
public:
    DataNotInBuffer(const std::string& errMsg) : std::invalid_argument(errMsg) {}
};

template<typename... Types>
class Variant;

// Store the value of the Variant. Allocate the buffer for the largest type (same as union type).
// One Variant Object has one VariantBuffer as base class
template<typename... Types>
class VariantBuffer
{
public:
    template<typename T>
    T* getValue() { return reinterpret_cast<T*>(buffer); }
    template<typename T>
    T const* getValue() const { return reinterpret_cast<const T*>(buffer); }
    template<typename T>
    void setValue(const T& value) { std::memcpy(buffer, &value, sizeof(T)); }
    inline void setActiveTypeIndex(int n) { activeTypeIndex = n; }
    inline const int getActiveTypeIndex() const { return activeTypeIndex; }
    void* getRawBuffer() { return buffer; }
private:
    using TypeWithMaxSize = getMaxSizeType<TypeList<Types...>>;
    alignas(Types...) unsigned char buffer[sizeof(TypeWithMaxSize)];
    int activeTypeIndex = VARIANT_INVALID_INDEX;
};

// Record the relationship of each Type in Variant and all types. It is used for call corresponding
// destructor which keeped in Variant. Due to in runtime, we cannot deduce the correct type name from
// a none constexpr value. We can only call all VariantSelector's destory function (because VariantSelector
// keep the typename T in its template paramenter, so we can call ~T() in VariantSelector, but we cannot call
// it in Variant class)
template<typename T, typename... Types>
class VariantSelector
{
public:
    bool destory() 
    {
        if(typeIndex == getDerived()->getActiveTypeIndex())
        {
            // Need to call the destructor of the object which allocated in Variant.
            getDerived()->template getValue<T>().~T();
            return true;
        }
        return false;
    }
    inline const int getTypeIndex() const { return typeIndex; }
    constexpr static int typeIndex = getFirstIndexOf<T, TypeList<Types...>>; // Type T's Index in Types
private:
    using Derived = Variant<Types...>;
    // Get Pointer to Variant class, only static_cast can do downcast.
    Derived* getDerived() { return static_cast<Derived*>(this);};
};

template<typename... Types>                               // v: unpack the variadic template parameter to a series of VariantSelector class
class Variant : private VariantBuffer<Types...>, private VariantSelector<Types, Types...>...
{
public:
    using Buffer =  VariantBuffer<Types...>;
    using Self = Variant<Types...>;

    template<typename T>
    constexpr explicit Variant(const T& v) : Buffer{} 
    {
        this->setActiveValue(v);
    }

    template<typename T>
    constexpr explicit Variant(T&& v) : Buffer{}
    {
        this->setActiveValue(std::forward<std::decay_t<T>>(v));
    }

    template<typename T>
    constexpr Self& operator = (const T& v)
    {
        destroy(); // Call object's destructor in buffer
        this->setActiveValue(v);
        return *this;
    }

    template<typename T>
    constexpr Self& operator = (T&& v)
    {
        destroy(); // Call object's destructor in buffer
        this->setActiveValue(std::forward<std::decay_t<T>>(v));
        return *this;
    }

    void destroy()
    {
        // Using initialize list to unpack the variadic paramter to call all the VariantSelector's destory function
        // This is only use for demonstrate the template programming. This will instance all the VariantSelector's destory
        // function, which will cause code bloat.
        // C++17 can directory unpack all destory like this: (VariantSelector<Types, Types...>::destory(), ...);
        bool result[] = { VariantSelector<Types, Types...>::destory()... };
        this->setActiveTypeIndex(VARIANT_INVALID_INDEX);
    }

    // This kind of compare operator isn't possible due to we cannot get current Active type from runtime
    // value. As stl implementation, it depends on constexpr value to get Type. If we use constexpr to define
    // The active type index, that cause the variant can only be set to one kind of type in construction. My
    // design allow variant can be modifid in runtime so the operator == is left being unimplemented.
    // bool operator == (const Self& v) {}

    ~Variant() 
    {

    }
private:
    template<typename T, typename... Args>
    friend class VariantSelector;

    template<int Index, typename... Args, typename T>
    friend const T& getValue(const Variant<Args...>& v);

    template<typename T, typename... Args>
    friend const T& getValue(const Variant<Args...>& v);

    template<typename T>
    void setActiveValue(const T& v)
    {
        this->setActiveTypeIndex(getFirstIndexOf<T, TypeList<Types...>>);
        // Call the T's constructor on internal buffer
        new(this->getRawBuffer())T(v);
    }

    template<typename T>
    void setActiveValue(T&& v)
    {
        this->setActiveTypeIndex(getFirstIndexOf<T, TypeList<Types...>>);
        // Call the T's move constructor on internal buffer
        new(this->getRawBuffer())T(std::move(v));
    }

    template<typename T>
    const T& getValue() const
    {
        using Selector = VariantSelector<T, Types...>;
        const Selector& s = dynamic_cast<const Selector&>(*this);
        if(Selector::typeIndex != this->getActiveTypeIndex())
        {
            std::stringstream ss;
            ss << "typeIndex = " << Selector::typeIndex << "; activeTypeIndex = " << this->getActiveTypeIndex();
            throw DataNotInBuffer(ss.str());
        }
        return *(dynamic_cast<const Buffer*>(this)-> template getValue<T>());
    }

    template<typename T>
    T& getValue()
    {
        using Selector = VariantSelector<T, Types...>;
        Selector& s = dynamic_cast<Selector&>(*this);
        if(Selector::typeIndex != this->getActiveTypeIndex())
        {
            std::stringstream ss;
            ss << "typeIndex = " << Selector::typeIndex << "; activeTypeIndex = " << this->getActiveTypeIndex();
            throw DataNotInBuffer(ss.str());
        }
        return *(dynamic_cast<Buffer*>(this)-> template getValue<T>());
    }

};

template<int Index, typename... Types, typename T = getAt<Index, TypeList<Types...>>>
const T& getValue(const Variant<Types...>& v)
{
    return v.template getValue<T>();
}

template<typename T, typename... Types>
const T& getValue(const Variant<Types...>& v)
{
    return v.template getValue<T>();
}
}// namespace MyStudy