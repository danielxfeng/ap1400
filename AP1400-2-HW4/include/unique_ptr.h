#ifndef UNIQUE_PTR
#define UNIQUE_PTR

template <typename T>
class UniquePtr {
public:
    // Default Constructor point to nullptr.
    UniquePtr();

    // Initial by a given point.
    explicit UniquePtr(T* pointer);

    // Copy constructor is forbidden.
    UniquePtr(const UniquePtr<T> &other) = delete;

    // Move constructor.
    UniquePtr(UniquePtr<T> &&other) noexcept;

    // Copy assignment is forbidden..
    UniquePtr& operator=(const UniquePtr &other) = delete;

    // Move assignment constructor.
    UniquePtr& operator=(UniquePtr &&other)  noexcept ;

    // Destructor;
    ~UniquePtr();

    // Return the raw pointer.
    T* get() const;

    // Return *ptr.
    T& operator*() const;

    // Return ptr->.
    T* operator->() const;

    // Delete the pointer.
    void reset();

    // Delete the pointer and assign to a new pointer.
    void reset(T* pointer);

    // Return the raw pointer and delete the smart point itself.
    T* release();

    // Return if the instance has a valid pointer.
    explicit operator bool() const;

private:
    // The pointer.
    T* _p;
};

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args);

#include "unique_ptr.hpp"
#endif //UNIQUE_PTR
