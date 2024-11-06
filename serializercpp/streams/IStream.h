#pragma once

class IStream {
public:
    virtual size_t Write(const void* data, size_t size, int timeoutMs = 0) = 0;
    virtual size_t Read(void* data, size_t size, int timeoutMs = 0) = 0;
};

