#ifndef U_REF_COUNT_CACHE_HPP
#define U_REF_COUNT_CACHE_HPP

#include "Api.hpp"

#include "MemoryU.hpp"

#include <unordered_set>
#include <unordered_map>

#include <memory>

namespace U {

// NOTE: None of this is fully thread safe for different reasons.
// A couple of simple scenarios may be safe.

// TODO: Need to benchmark the two implementations.

namespace Impl {

template<typename T, typename THash = std::hash<T>, typename TEqual = std::equal_to<T>>
class RefCountCacheImmediate : U::CannotCopy {
public:
    RefCountCacheImmediate() : _map(std::make_shared<TMap>()) {
    }

    template<typename TConcrete, typename... TArgs>
    std::shared_ptr<T> make(TArgs&&... args) {
        // If the element is not inserted, 'obj' will not be returned and will call its deleter in
        // the end.
        // For now, creating it with a deleter that does not erase the element.
        std::shared_ptr<T> obj(new TConcrete(std::forward<TArgs>(args)...), Deleter{});
        auto result = _map->emplace(obj.get(), obj);
        if (!result.second)
            return result.first->second.lock();
        // Here, we inserted the element and will return 'obj'.
        // Now, its deleter should erase the element.
        std::get_deleter<Deleter>(obj)->map = _map;
        return obj;
    }

private:
    struct PTHash {
        size_t operator()(T *pt) const {
            THash hash;
            return hash(*pt);
        }
    };

    struct PTEqual {
        bool operator()(T *pt1, T *pt2) const {
            TEqual equal;
            return equal(*pt1, *pt2);
        }
    };

    // We have to have a map from T* and not a set, since a deleter will only have access to T*
    // and will need to erase an element.
    using TMap = std::unordered_map <T*, std::weak_ptr<T>, PTHash, PTEqual>;

    struct Deleter {
        Deleter() {
        }

        Deleter(const std::shared_ptr<TMap> &map) : map(map) {
        }

        void operator()(T *pt) const {
            if (map)
                map->erase(pt);
            delete pt;
        }

        std::shared_ptr<TMap> map;
    };

    // _map should be accessible via a shared_ptr, so that if *this gets destroyed, _map
    // is still live, because it it referenced by the deleters.
    // To make this thread safe, can have a shared mutex that guards access to _map.
    std::shared_ptr<TMap> _map;
};


template<typename T, typename THash = std::hash<T>, typename TEqual = std::equal_to<T>>
class RefCountCacheGC : U::CannotCopy {
public:

    template<typename TConcrete, typename... TArgs>
    std::shared_ptr<T> make(TArgs&&... args) {
        std::shared_ptr<T> obj = std::make_shared<TConcrete>(std::forward<TArgs>(args)...);

        size_t bucketsBefore = _map.bucket_count();

        while (true) {
            auto emplaceResult = _map.emplace(Item(obj));
            if (!emplaceResult.second) {
                std::shared_ptr<T> result =  emplaceResult.first->ptr.lock();
                // If an equal element was found, but locking it returns empty pointer, means that
                // it has expired just after being found. Need to retry emplacing.
                if (!result)
                    continue;
                return result;
            }
            break;
        }

        // TODO: Figure out if this can be done before the resize.
        if (_map.bucket_count() > bucketsBefore)
            eraseExpired();

        return obj;
    }
    
private:
    struct Item {
        Item(std::shared_ptr<T> &ptr) : ptr(std::weak_ptr<T>(ptr)), hash(THash{}(*ptr)) {
        }

        std::weak_ptr<T> ptr;
        size_t hash;
    };

    struct ItemHash {
        size_t operator()(const Item &item) const {
            return item.hash;
        }
    };
    
    struct ItemEqual {
        bool operator()(const Item &item1, const Item &item2) const {
            std::shared_ptr<T> ptr1 = item1.ptr.lock();
            std::shared_ptr<T> ptr2 = item2.ptr.lock();
            if (!ptr1 || !ptr2)
                // TODO: Figure out if unordered_set will freak out when equality of elements changes.
                // This will be called on lookup/insert or rehash. For lookup/insert, at least
                // one ptr will be non-expired (based on what RefCountCacheGC) does.
                // For rehash, items that were in the set, were non-equal, so here we are returning false,
                // to be consistent with the state before the items expired.
                return false;
            return TEqual{}(*item1.ptr.lock(), *item2.ptr.lock());
        }
    };

    void eraseExpired() {
        // NOTE: This will only work correctly in C++14 and later.
        // In earlier standards, the items may get reordered, and some expired pointers will be missed.
        // TODO: Maybe give a warning if compiled with earlier standard.
        for(auto it = _map.begin(), end = _map.end(); it != end; ) {
            if(it->ptr.expired())
                it = _map.erase(it);
            else
                ++it;
        }
    }
    
    std::unordered_set <Item, ItemHash, ItemEqual> _map;
};

}

template<typename T, typename THash = std::hash<T>, typename TEqual = std::equal_to<T>>
using RefCountCache = Impl::RefCountCacheImmediate<T, THash, TEqual>;

}

#endif
