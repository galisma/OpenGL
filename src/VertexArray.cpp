// VertexArray.cpp
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray() { GLCall(glGenVertexArrays(1, &m_RendererID)); }

VertexArray::~VertexArray() {
  GLCall(glDeleteVertexArrays(1, &m_RendererID));  // Antes era glGenVertexArrays
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
  Bind();
  vb.Bind();
  // Corregir GetElement() -> GetElements()
  const auto& elements = layout.GetElements(); // <- Aquí el cambio
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto& element = elements[i];
    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(
        i, element.count, element.type, element.normalized, layout.GetStride(),
        (const void*)(static_cast<uintptr_t>(offset)) // <- Cast modificado
    ));
    offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
  }
}

void VertexArray::Bind() const {
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const { GLCall(glBindVertexArray(0)); }