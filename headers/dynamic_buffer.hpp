#include <cstdlib>

template <class T>
class DynamicBuffer {
public:
    DynamicBuffer();

    DynamicBuffer(size_t size);

    ~DynamicBuffer();
    
    DynamicBuffer(const DynamicBuffer& other);

    DynamicBuffer& operator=(const DynamicBuffer& other);

    void CleanContents() {
        for (size_t i = 0; i < GetCurrSize(); ++i) {
            free(buffer[i]);
            buffer[i] = nullptr;
        }
    }

    void Add(T value);

    T operator[](size_t i) const;

    size_t GetCurrSize() const;

private:
    void Increase();

    T* buffer;

    size_t curr_size;
    size_t max_size;
};

template <class T>
DynamicBuffer<T>::DynamicBuffer() {
    buffer = nullptr;

    curr_size = 0;
    max_size = 0;
}

template <class T>
DynamicBuffer<T>::DynamicBuffer(size_t size) {
    buffer = (T*) calloc(size, sizeof(T));

    max_size = size;
}

template <class T>
DynamicBuffer<T>::~DynamicBuffer() {
    if (buffer) {
        free(buffer);
        buffer = nullptr;
    }
}

template <class T>
DynamicBuffer<T>::DynamicBuffer(const DynamicBuffer<T>& other) {
    curr_size = 0;
    max_size = other.max_size;

    buffer = (T*) calloc(max_size, sizeof(T));

    for (size_t i = 0; i < other.curr_size; ++i) {
        buffer[i] = other.buffer[i];

        ++curr_size;
    }

    assert(curr_size == other.curr_size);
}

template <class T>
DynamicBuffer<T>& DynamicBuffer<T>::operator=(const DynamicBuffer<T>& other) {
    if (this == &other) {
        return *this;
    }

    curr_size = 0;
    max_size = other.max_size;

    buffer = (T*) calloc(max_size, sizeof(T));

    for (size_t i = 0; i < other.curr_size; ++i) {
        buffer[i] = other.buffer[i];

        ++curr_size;
    }

    assert (curr_size == other.curr_size);

    return *this;
}

template <class T>
void DynamicBuffer<T>::Add(T value) {
    assert(buffer);

    if (curr_size == max_size) {
        Increase();
    }

    buffer[curr_size] = value;

    ++curr_size;
}

template <class T>
T DynamicBuffer<T>::operator[](size_t i) const {
    assert(i < max_size);

    return buffer[i];
}

template <class T>
size_t DynamicBuffer<T>::GetCurrSize() const {
    return curr_size;
}

template <class T>
void DynamicBuffer<T>::Increase() {
    assert(buffer);

    T* temp_buffer = (T*) calloc(max_size * 2, sizeof(T));
    assert(temp_buffer);

    for (size_t i = 0; i < curr_size; ++i) {
        temp_buffer[i] = buffer[i];
    }

    assert(buffer);
    free(buffer);
    buffer = nullptr;

    buffer = temp_buffer;
    max_size *= 2;
}
