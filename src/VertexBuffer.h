// VertexBuffer.h

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
#pragma once

class VertexBuffer
{
    private:
        unsigned int m_RendererID;
    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
};

#endif // VERTEXBUFFER_H
