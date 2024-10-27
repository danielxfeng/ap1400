template<typename T>
SharedPtr<T>::SharedPtr() : _p(nullptr), c(new int(0)) {}

template<typename T>
SharedPtr<T>::SharedPtr(T *pointer) : _p(pointer), c(new int(1)) {}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T> &other) : _p(other._p), c(other.c) {
    if (c) (*c)++;
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T> &&other) noexcept : _p(other._p), c(other.c) {
    other._p = nullptr;
    other.c = nullptr;
}

template<typename T>
SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr<T> &other) {
    if (this != &other) {
        (*c)--;
        if (!*c) {
            delete _p;
        }
        _p = other._p;
        c = other.c;
        if (c) (*c)++;
    }
    return *this;
}

template<typename T>
SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr<T> &&other)  noexcept {
    if (this != &other) {
        (*c)--;
        if (!*c) {
            delete _p;
        }
        _p = other._p;
        c = other.c;
        other._p = nullptr;
        other.c = nullptr;
    }
    return *this;
}

template<typename T>
SharedPtr<T>::~SharedPtr() {
    (*c)--;
    if (!*c) {
        delete _p;
    }
    _p = nullptr;
}

template<typename T>
int SharedPtr<T>::use_count() const {return *c;}

template<typename T>
T *SharedPtr<T>::get() const {return _p;}

template<typename T>
T &SharedPtr<T>::operator*() const {return *_p;}

template<typename T>
T *SharedPtr<T>::operator->() const {return _p;}

template<typename T>
void SharedPtr<T>::reset() {
    (*c)--;
    if (!*c) {
        delete _p;
    }
    _p = nullptr;
}

template<typename T>
void SharedPtr<T>::reset(T* pointer) {
    (*c)--;
    if (!*c) {
        delete _p;
    }
    _p = pointer;
    *c = 1;
}

template<typename T>
SharedPtr<T>::operator bool() const {
    return _p != nullptr;
}

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {return SharedPtr{new T{std::forward<Args>(args)...}};}
