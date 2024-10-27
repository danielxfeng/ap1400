template<typename T>
UniquePtr<T>::UniquePtr() {_p = nullptr;}

template<typename T>
UniquePtr<T>::UniquePtr(T *pointer) : _p(pointer) {}

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr<T> &&other)  noexcept : _p(other._p) {*other.p = nullptr;}

template<typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr &&other)  noexcept {
    if (this != &other) {
        delete _p;
        _p = other._p;
        other._p = nullptr;
    }
    return *this;
}

template<typename T>
UniquePtr<T>::~UniquePtr() {delete _p;}

template<typename T>
T *UniquePtr<T>::get() const {return _p;}

template<typename T>
T &UniquePtr<T>::operator*() const {return *_p;}

template<typename T>
T *UniquePtr<T>::operator->() const {return _p;}

template<typename T>
void UniquePtr<T>::reset() {
    delete _p;
    _p = nullptr;
}

template<typename T>
void UniquePtr<T>::reset(T* pointer) {
    delete _p;
    _p = pointer;
}

template<typename T>
T *UniquePtr<T>::release() {
    T* temp = _p;
    _p = nullptr;
    return temp;
}

template<typename T>
UniquePtr<T>::operator bool() const {
    return _p != nullptr;
}

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {return UniquePtr{new T{std::forward<Args>(args)...}};}
