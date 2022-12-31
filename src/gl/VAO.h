#pragma once

#include "VBO.h"

#include <memory>

class VAO
{
public:
    VAO();

    // delete non move constructor and assignment operator to prevent
    // unintentional resource leaks
    VAO(const VAO&) = delete;
    VAO& operator= (const VAO&) = delete;
    
    VAO(VAO&&);
    VAO& operator= (VAO&&);

    ~VAO();


    template<typename Attribute>
    void defineAttribute(const std::vector<Attribute>& data);

    void defineIBO(const std::vector<GLuint>& indices);

    void bind() { glBindVertexArray(handle); }

    // getters
    GLuint getHandle() const        { return handle;            }
    GLuint getNumIndices() const    { return indexBuffer->size; }

private:
	GLuint handle;

    std::vector<VBO> vbos;

    struct IBO
    {
        IBO(const std::vector<GLuint>& indices)
            :
            buffer{ indices, GL_ELEMENT_ARRAY_BUFFER },
            size{ indices.size() }
        {}

        const VBO buffer;
        const unsigned size{ 0 };
    };

    std::unique_ptr<IBO> indexBuffer;

private:
    void reset();
    void destroy();
};

template<typename Attribute>
void VAO::defineAttribute(const std::vector<Attribute>& attributeData)
{
    bind();

    vbos.emplace_back(VBO{ attributeData, GL_ARRAY_BUFFER });

    glVertexAttribPointer(
        Attribute::META.location,            // attribute location
        Attribute::META.numElements,         // elements per attribute
        Attribute::META.elementDataType,     // element data type
        Attribute::META.shouldNormalize,     // normalized
        sizeof(Attribute),                   // stride (in bytes)
        0                                    // offset (in bytes)
    );

    // enable attribute (no reason not to if we defined an attribute)
    glEnableVertexAttribArray(Attribute::META.location);
}