#ifndef SHARED_PTR
#define SHARED_PTR

template <typename T>
class SharedPtr {
public:
    // Default Constructor point to nullptr.
    SharedPtr();

    // Initial by a given point.
    explicit SharedPtr(T* pointer);

    // Copy constructor is forbidden.
    SharedPtr(const SharedPtr<T> &other);

    // Move constructor.
    SharedPtr(SharedPtr<T> &&other) noexcept;

    // Copy assignment constructor is forbidden.
    SharedPtr<T> &operator=(const SharedPtr<T> &other);

    // Move assignment constructor.
    SharedPtr<T> &operator=(SharedPtr<T> &&other)  noexcept;

    // Destructor;
    ~SharedPtr();

    // Return the count of how many instances available.
    int use_count() const;

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

    // Return if the instance has a valid pointer.
    explicit operator bool() const;

private:
    // The pointer.
    T* _p;
    // The count of the instances.
    int *c;

};

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args);

#include "shared_ptr.hpp"

#endif //SHARED_PTR
