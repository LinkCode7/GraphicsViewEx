#if 1

    #include "kiwi.h"

kiwi::ByteBuffer::ByteBuffer() : _data(new uint8_t[INITIAL_CAPACITY]), _capacity(INITIAL_CAPACITY), _ownsData(true)
{
}

kiwi::ByteBuffer::ByteBuffer(uint8_t *data, size_t size) : _data(data), _size(size), _capacity(size)
{
}

kiwi::ByteBuffer::ByteBuffer(const uint8_t *data, size_t size)
    : _data(const_cast<uint8_t *>(data)), _size(size), _capacity(size), _isConst(true)
{
    (void)_isConst;
}

kiwi::ByteBuffer::~ByteBuffer()
{
    if (_ownsData)
    {
        delete[] _data;
    }
}

bool kiwi::ByteBuffer::readByte(bool &result)
{
    uint8_t value;
    if (!readByte(value))
    {
        result = false;
        return false;
    }

    result = value;
    return true;
}

bool kiwi::ByteBuffer::readByte(uint8_t &result)
{
    if (_index >= _size)
    {
        result = 0;
        return false;
    }

    result = _data[_index];
    _index++;
    return true;
}

bool kiwi::ByteBuffer::readVarFloat(float &result)
{
    uint8_t first;
    if (!readByte(first))
    {
        return false;
    }

    // Optimization: use a single byte to store zero
    if (first == 0)
    {
        result = 0;
        return true;
    }

    // Endian-independent 32-bit read
    if (_index + 3 > _size)
    {
        result = 0;
        return false;
    }
    uint32_t bits = first | (_data[_index] << 8) | (_data[_index + 1] << 16) | (_data[_index + 2] << 24);
    _index += 3;

    // Move the exponent back into place
    bits = (bits << 23) | (bits >> 9);

    // Reinterpret as a floating-point number
    memcpy(&result, &bits, 4);
    return true;
}

bool kiwi::ByteBuffer::readVarUint(uint32_t &result)
{
    uint8_t shift = 0;
    uint8_t byte;
    result = 0;

    do
    {
        if (!readByte(byte))
        {
            return false;
        }

        result |= (byte & 127) << shift;
        shift += 7;
    } while (byte & 128 && shift < 35);

    return true;
}

bool kiwi::ByteBuffer::readVarInt(int32_t &result)
{
    uint32_t value;
    if (!readVarUint(value))
    {
        result = 0;
        return false;
    }

    result = value & 1 ? ~(value >> 1) : value >> 1;
    return true;
}

bool kiwi::ByteBuffer::readString(const char *&result)
{
    result = reinterpret_cast<const char *>(_data) + _index;

    do
    {
        if (_index >= _size)
            return false;
    } while (_data[_index++] != '\0');

    return true;
}

bool kiwi::ByteBuffer::readString(String &result, MemoryPool &pool)
{
    uint32_t size = 0;
    result        = String();

    do
    {
        if (_index >= _size)
            return false;
    } while (_data[_index + size++] != '\0');

    result = pool.string(reinterpret_cast<char *>(_data + _index), size - 1);
    _index += size;
    return true;
}

void kiwi::ByteBuffer::writeByte(uint8_t value)
{
    assert(!_isConst);
    size_t index = _size;
    _growBy(1);
    _data[index] = value;
}

void kiwi::ByteBuffer::writeVarFloat(float value)
{
    assert(!_isConst);

    // Reinterpret as an integer
    uint32_t bits;
    memcpy(&bits, &value, 4);

    // Move the exponent to the first 8 bits
    bits = (bits >> 23) | (bits << 9);

    // Optimization: use a single byte to store zero and denormals (check for an exponent of 0)
    if ((bits & 255) == 0)
    {
        writeByte(0);
        return;
    }

    // Endian-independent 32-bit write
    size_t index = _size;
    _growBy(4);
    _data[index]     = bits;
    _data[index + 1] = bits >> 8;
    _data[index + 2] = bits >> 16;
    _data[index + 3] = bits >> 24;
}

void kiwi::ByteBuffer::writeVarUint(uint32_t value)
{
    assert(!_isConst);
    do
    {
        uint8_t byte = value & 127;
        value >>= 7;
        writeByte(value ? byte | 128 : byte);
    } while (value);
}

void kiwi::ByteBuffer::writeVarInt(int32_t value)
{
    assert(!_isConst);
    writeVarUint((value << 1) ^ (value >> 31));
}

void kiwi::ByteBuffer::writeString(const char *value)
{
    assert(!_isConst);
    uint32_t count = strlen(value) + 1;
    size_t   index = _size;
    _growBy(count);
    memcpy(_data + index, value, count);
}

void kiwi::ByteBuffer::_growBy(size_t amount)
{
    assert(!_isConst);

    if (_size + amount > _capacity)
    {
        size_t   capacity = (_size + amount) * 2;
        uint8_t *data     = new uint8_t[capacity];
        memcpy(data, _data, _size);

        if (_ownsData)
        {
            delete[] _data;
        }

        _data     = data;
        _capacity = capacity;
        _ownsData = true;
    }

    _size += amount;
}

////////////////////////////////////////////////////////////////////////////////

void kiwi::MemoryPool::clear()
{
    for (Chunk *chunk = _first, *next; chunk; chunk = next)
    {
        next = chunk->next;
        delete[] chunk->data;
        delete chunk;
    }

    _first = _last = nullptr;
}

kiwi::String kiwi::MemoryPool::string(const char *text, uint32_t count)
{
    char *c_str = allocate<char>(count + 1);
    memcpy(c_str, text, count);
    return String(c_str);
}

////////////////////////////////////////////////////////////////////////////////

bool kiwi::BinarySchema::parse(ByteBuffer &bb)
{
    uint32_t definitionCount = 0;

    _definitions = {};
    _pool.clear();

    if (!bb.readVarUint(definitionCount))
    {
        return false;
    }

    _definitions = _pool.array<Definition>(definitionCount);

    for (auto &definition : _definitions)
    {
        uint32_t fieldCount = 0;

        if (!bb.readString(definition.name, _pool) || !bb.readByte(definition.kind) || !bb.readVarUint(fieldCount) ||
            (definition.kind != KIND_ENUM && definition.kind != KIND_STRUCT && definition.kind != KIND_MESSAGE))
        {
            return false;
        }

        definition.fields = _pool.array<Field>(fieldCount);

        for (auto &field : definition.fields)
        {
            if (!bb.readString(field.name, _pool) || !bb.readVarInt(field.type) || !bb.readByte(field.isArray) ||
                !bb.readVarUint(field.value) || field.type < TYPE_STRING || field.type >= (int32_t)definitionCount)
            {
                return false;
            }
        }
    }

    return true;
}

bool kiwi::BinarySchema::findDefinition(const char *definition, uint32_t &index) const
{
    for (uint32_t i = 0; i < _definitions.size(); i++)
    {
        auto &item = _definitions[i];
        if (item.name == String(definition))
        {
            index = i;
            return true;
        }
    }

    // Ignore fields we're looking for in an old schema
    index = -1;
    return false;
}

bool kiwi::BinarySchema::skipField(ByteBuffer &bb, uint32_t definition, uint32_t field) const
{
    if (definition < _definitions.size())
    {
        for (auto &item : _definitions[definition].fields)
        {
            if (item.value == field)
            {
                return _skipField(bb, item);
            }
        }
    }

    return false;
}

bool kiwi::BinarySchema::_skipField(ByteBuffer &bb, const Field &field) const
{
    uint32_t count = 1;

    if (field.isArray && !bb.readVarUint(count))
    {
        return false;
    }

    while (count-- > 0)
    {
        switch (field.type)
        {
            case TYPE_BOOL:
            case TYPE_BYTE:
            {
                uint8_t dummy = 0;
                if (!bb.readByte(dummy))
                    return false;
                break;
            }

            case TYPE_INT:
            case TYPE_UINT:
            {
                uint32_t dummy = 0;
                if (!bb.readVarUint(dummy))
                    return false;
                break;
            }

            case TYPE_FLOAT:
            {
                float dummy = 0;
                if (!bb.readVarFloat(dummy))
                    return false;
                break;
            }

            case TYPE_STRING:
            {
                uint8_t value = 0;
                do
                {
                    if (!bb.readByte(value))
                        return false;
                } while (value);
                break;
            }

            default:
            {
                assert(field.type >= 0 && (uint32_t)field.type < _definitions.size());
                auto &definition = _definitions[field.type];

                switch (definition.kind)
                {
                    case KIND_ENUM:
                    {
                        uint32_t dummy;
                        if (!bb.readVarUint(dummy))
                            return false;
                        break;
                    }

                    case KIND_STRUCT:
                    {
                        for (auto &item : definition.fields)
                        {
                            if (!_skipField(bb, item))
                                return false;
                        }
                        break;
                    }

                    case KIND_MESSAGE:
                    {
                        uint32_t id = 0;
                        while (true)
                        {
                            if (!bb.readVarUint(id))
                                return false;
                            if (!id)
                                break;
                            if (!skipField(bb, field.type, id))
                                return false;
                        }
                        break;
                    }

                    default:
                    {
                        assert(false);
                        break;
                    }
                }
            }
        }
    }

    return true;
}

#endif