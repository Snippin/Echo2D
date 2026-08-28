#pragma once

#include "../Essentials/Vertex.h"

#include <memory>
#include <vector>

namespace ECHO_RENDERING
{
    constexpr size_t NUM_SPRITE_VERTICES = 4;
    constexpr size_t NUM_SPRITE_INDICES = 6;
    constexpr size_t MAX_SPRITES = 10000;
    constexpr size_t MAX_INDICES = MAX_SPRITES * NUM_SPRITE_INDICES;
    constexpr size_t MAX_VERTICES = MAX_SPRITES * NUM_SPRITE_VERTICES;

    template <typename TBatch, typename TGlyph>
    class Batcher
    {
    public:
        Batcher();
        Batcher(bool use_EBO);
        virtual ~Batcher();

        void Begin();
        virtual void End();
        virtual void Render() = 0;

    protected:

        // `stride` - Specifies byte offset between consecutive attributes
        // `offset` - Specifies the offset of the first component

        void SetVertexAttribute(GLuint layout_position, GLuint num_components,
            GLenum type, GLsizeiptr stride, void *offset,
            GLboolean normalised = GL_FALSE) const;

        inline GLuint GetVBO() const { return VBO; }
        inline GLuint GetEBO() const { return EBO; }
        inline void EnableVAO() const { glBindVertexArray(VAO); }
        inline void DisableVAO() const { glBindVertexArray(0); }

        virtual void GenerateBatches() = 0;

        std::vector<std::shared_ptr<TGlyph>> glyphs;
        std::vector<std::shared_ptr<TBatch>> batches;

    private:
        void Initialise();

        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        bool use_EBO;
    };

    template<typename TBatch, typename TGlyph>
    inline Batcher<TBatch, TGlyph>::Batcher() :
        Batcher(true)
    {
    }

    template<typename TBatch, typename TGlyph>
    inline Batcher<TBatch, TGlyph>::Batcher(bool use_EBO) :
        VAO{0}, VBO{0}, EBO{0}, glyphs{}, batches{}, use_EBO{use_EBO}
    {
        Initialise();
    }

    template<typename TBatch, typename TGlyph>
    inline Batcher<TBatch, TGlyph>::~Batcher()
    {
        if (VAO)
        {
            glDeleteVertexArrays(1, &VAO);
        }

        if (VBO)
        {
            glDeleteBuffers(1, &VBO);
        }

        if (EBO)
        {
            glDeleteBuffers(1, &EBO);
        }
    }

    template<typename TBatch, typename TGlyph>
    inline void Batcher<TBatch, TGlyph>::Begin()
    {
        glyphs.clear();
        batches.clear();
    }

    template<typename TBatch, typename TGlyph>
    inline void Batcher<TBatch, TGlyph>::End()
    {
        if (glyphs.empty())
        {
            return;
        }

        GenerateBatches();
    }

    template<typename TBatch, typename TGlyph>
    inline void Batcher<TBatch, TGlyph>::SetVertexAttribute(
        GLuint layout_position, GLuint num_components, GLenum type,
        GLsizeiptr stride, void *offset, GLboolean normalised) const
    {
        glBindVertexArray(VAO);
        glVertexAttribPointer(layout_position, num_components, type,
            normalised, stride, offset);
        glEnableVertexAttribArray(layout_position);
        glBindVertexArray(0);
    }

    template<typename TBatch, typename TGlyph>
    inline void Batcher<TBatch, TGlyph>::Initialise()
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr,
            GL_DYNAMIC_DRAW);

        if (!use_EBO)
        {
            glBindVertexArray(0);
            return;
        }

        // Generate index buffer
        GLuint offset{0};
        GLuint indices[NUM_SPRITE_INDICES]{0, 1, 2, 2, 3, 0};

        auto indices_array = std::make_unique<GLuint[]>(MAX_INDICES);

        // Loop through the index array in chunks of `NUM_SPRITE_INDICES` (6)
        for (size_t i = 0; i < MAX_INDICES; i += NUM_SPRITE_INDICES)
        {
            // Copy base triangle pattern into the current chunk of the array
            // shifting the vertex references by the current `offset`
            for (size_t j = 0; j < NUM_SPRITE_INDICES; j++)
            {
                indices_array[i + j] = indices[j] + offset;
            }

            // Advance the offset by the number of vertices in a sprite (4)
            offset += NUM_SPRITE_VERTICES;
        }

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * MAX_INDICES,
            indices_array.get(), GL_DYNAMIC_DRAW);

        glBindVertexArray(0);
    }
}
