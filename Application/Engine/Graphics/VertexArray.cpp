#include "pch.h"
#include "VertexArray.h"

bool sn::VertexArray::Create(const std::string& name, void* null)
{
    glGenVertexArrays(1, &m_vao); 
    glBindVertexArray(m_vao);

    return true;
}

void sn::VertexArray::Destroy()
{
    glDeleteVertexArrays(1, &m_vao); 
    glDeleteBuffers(m_vbos.size(), m_vbos.data());
}

void sn::VertexArray::CreateBuffer(GLsizei size, GLsizei count, void* data)
{
    m_count = count;

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    m_vbos.push_back(vbo);
}

void sn::VertexArray::SetAttribute(int index, GLint size, GLsizei stride, size_t offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
}

void sn::VertexArray::Draw(GLenum primitiveType)
{
    glBindVertexArray(m_vao);
    glDrawArrays(primitiveType, 0, m_count);
}
