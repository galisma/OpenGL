// VertexBufferLayout.h
#pragma once
#include <GL/glew.h>
#include "Renderer.h"
#include <stdexcept>
#include <vector>

// Estructura que describe cada elemento del buffer.
struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT:         return 4;
            case GL_UNSIGNED_INT:  return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout() : m_Stride(0) {}

    // Declaramos la función plantilla sin definirla aquí.
    template<typename T>
    void Push(unsigned int count);

    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
};

// Utilidad para generar un static_assert dependiente de T.
template<typename T>
struct always_false : std::false_type {};

// Definición del template genérico: si se intenta usar un tipo no soportado se produce un error en tiempo de compilación.
template<typename T>
void VertexBufferLayout::Push(unsigned int /*count*/) {
    static_assert(always_false<T>::value, "Tipo no soportado en VertexBufferLayout::Push");
}

// Especialización para float
template<>
inline void VertexBufferLayout::Push<float>(unsigned int count) { // Agregar inline
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

// Especialización para unsigned int
template<>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count) { // Agregar inline
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

// Especialización para unsigned char
template<>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count) { // Agregar inline
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}