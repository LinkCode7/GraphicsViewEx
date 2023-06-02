#ifndef KIWI_H
#define KIWI_H

#include <assert.h>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <initializer_list>

namespace kiwi
{
class String;
class MemoryPool;

class ByteBuffer
{
public:
    ByteBuffer();
    ByteBuffer(uint8_t *data, size_t size);
    ByteBuffer(const uint8_t *data, size_t size);
    ~ByteBuffer();
    ByteBuffer(const ByteBuffer &) = delete;
    ByteBuffer &operator=(const ByteBuffer &) = delete;

    uint8_t *data() const { return _data; }
    size_t   size() const { return _size; }
    size_t   index() const { return _index; }

    bool readByte(bool &result);
    bool readByte(uint8_t &result);
    bool readVarFloat(float &result);
    bool readVarUint(uint32_t &result);
    bool readVarInt(int32_t &result);
    bool readString(const char *&result);
    bool readString(String &result, MemoryPool &pool);

    void writeByte(uint8_t value);
    void writeVarFloat(float value);
    void writeVarUint(uint32_t value);
    void writeVarInt(int32_t value);
    void writeString(const char *value);

private:
    void _growBy(size_t amount);

    enum
    {
        INITIAL_CAPACITY = 256
    };
    uint8_t *_data     = nullptr;
    size_t   _size     = 0;
    size_t   _capacity = 0;
    size_t   _index    = 0;
    bool     _ownsData = false;
    bool     _isConst  = false;
};

////////////////////////////////////////////////////////////////////////////////

class String
{
public:
    String() {}
    explicit String(const char *c_str) : _c_str(c_str) {}

    const char *c_str() const { return _c_str; }

private:
    const char *_c_str = nullptr;
};

inline bool operator==(const String &a, const String &b)
{
    return !strcmp(a.c_str(), b.c_str());
}
inline bool operator!=(const String &a, const String &b)
{
    return !(a == b);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Array
{
public:
    Array() {}
    Array(T *data, uint32_t size) : _data(data), _size(size) {}

    T *      data() { return _data; }
    T *      begin() { return _data; }
    T *      end() { return _data + _size; }
    uint32_t size() const { return _size; }
    T &      operator[](uint32_t index)
    {
        assert(index < _size);
        return _data[index];
    }
    void set(const T *data, size_t size)
    {
        assert(size == _size);
        memcpy(_data, data, (size < _size ? size : _size) * sizeof(T));
    }
    void set(const std::initializer_list<T> &data) { set(data.begin(), data.size()); }

    const T *data() const { return _data; }
    const T *begin() const { return _data; }
    const T *end() const { return _data + _size; }
    const T &operator[](uint32_t index) const
    {
        assert(index < _size);
        return _data[index];
    }

private:
    T *      _data = nullptr;
    uint32_t _size = 0;
};

////////////////////////////////////////////////////////////////////////////////

class MemoryPool
{
public:
    MemoryPool() {}
    ~MemoryPool() { clear(); }
    MemoryPool(const MemoryPool &) = delete;
    MemoryPool &operator=(const MemoryPool &) = delete;

    void clear();

    template <typename T>
    T *allocate(uint32_t count = 1);

    template <typename T>
    Array<T> array(uint32_t size)
    {
        return Array<T>(allocate<T>(size), size);
    }

    String string(const char *data, uint32_t count);
    String string(const char *c_str) { return string(c_str, strlen(c_str)); }

private:
    enum
    {
        INITIAL_CAPACITY = 1 << 12
    };

    struct Chunk
    {
        uint8_t *data     = nullptr;
        uint32_t capacity = 0;
        uint32_t used     = 0;
        Chunk *  next     = nullptr;
    };

    Chunk *_first = nullptr;
    Chunk *_last  = nullptr;
};

////////////////////////////////////////////////////////////////////////////////

class BinarySchema
{
public:
    bool parse(ByteBuffer &bb);
    bool findDefinition(const char *definition, uint32_t &index) const;
    bool skipField(ByteBuffer &bb, uint32_t definition, uint32_t field) const;

private:
    enum
    {
        TYPE_BOOL   = -1,
        TYPE_BYTE   = -2,
        TYPE_INT    = -3,
        TYPE_UINT   = -4,
        TYPE_FLOAT  = -5,
        TYPE_STRING = -6,
    };

    struct Field
    {
        String   name;
        int32_t  type    = 0;
        bool     isArray = false;
        uint32_t value   = 0;
    };

    enum
    {
        KIND_ENUM    = 0,
        KIND_STRUCT  = 1,
        KIND_MESSAGE = 2,
    };

    struct Definition
    {
        String       name;
        uint8_t      kind = 0;
        Array<Field> fields;
    };

    bool _skipField(ByteBuffer &bb, const Field &field) const;

    MemoryPool        _pool;
    Array<Definition> _definitions;
};
} // namespace kiwi

////////////////////////////////////////////////////////////////////////////////

template <typename T>
T *kiwi::MemoryPool::allocate(uint32_t count)
{
    Chunk *  chunk = _last;
    uint32_t size  = count * sizeof(T);
    uint32_t index = (chunk ? chunk->used : 0) + alignof(T) - 1;
    index -= index % alignof(T);

    if (chunk && index + size >= index && index + size <= chunk->capacity)
    {
        chunk->used = index + size;
        return reinterpret_cast<T *>(chunk->data + index);
    }

    chunk           = new Chunk;
    chunk->capacity = size > INITIAL_CAPACITY ? size : INITIAL_CAPACITY;
    chunk->data     = new uint8_t[chunk->capacity](); // "()" means zero-initialized
    chunk->used     = size;

    if (_last)
        _last->next = chunk;
    else
        _first = chunk;
    _last = chunk;

    return reinterpret_cast<T *>(chunk->data);
}

#endif
