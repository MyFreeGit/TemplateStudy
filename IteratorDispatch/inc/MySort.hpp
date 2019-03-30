#include <algorithm>
#include <iterator>
#include <functional>
#include <type_traits>
#include <iterator>

template <typename Iter>
using isRandomAccessIterator = std::is_same<typename std::iterator_traits<Iter>::iterator_category, typename std::random_access_iterator_tag>;

template <class Iterator, class Less>
void bubbleSort(Iterator first, Iterator last, Less less);

template <typename Iter, bool>
struct MySorter {
    static void sort(Iter begin, Iter end) {
        bubbleSort(begin, end, std::less<decltype(*begin)>());
    }
};

template <typename Iter>
struct MySorter <Iter, true>{
    static void sort(Iter begin, Iter end) {
        std::sort(begin, end);
    }
};


template <typename Iter>
void my_sort(Iter begin, Iter end)
{
    MySorter<Iter, isRandomAccessIterator<Iter>::value>::sort(begin, end);
}

template <class Iterator, class Less>
void bubbleSort(Iterator first, Iterator last, Less less)
{
    if (first == last)
        return;
    // usually "last" points beyond the last element
    // now it points directly to that last element
    --last;
    // only one element => it's sorted
    if (first == last)
        return;

    bool swapped;
    do
    {
        // reset swapped flag
        swapped = false;
        Iterator current = first;
        while (current != last)
        {
            // bubble up
            Iterator next = current;
            ++next;
            // two neighbors in wrong order ? swap them !
            if (less(*next, *current))
            {
                std::iter_swap(current, next);
                swapped = true;
            }
            ++current;
        }
        // last element is already sorted now
        --last; // remove this line if you only have a forward Iter

        // last will move closer towards first
    } while (swapped && first != last);
}



