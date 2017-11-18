#pragma once

#include <vector>
#include <GL/glew.h>
#include <cereal/types/vector.hpp>
#include "maths/maths.h"

#include <cstddef>
#include "graphics/texture.h"
#include "graphics/buffers/indexbuffer.h"


namespace hiraeth {
	namespace map {

		struct VertexData
		{
			maths::vec3 vertex;
			maths::vec2 uv;
			unsigned int color;
		};

		struct TileUv
		{
			maths::vec2 UvPos;
			maths::vec2 UvSize;
			template<class Archive>
			void serialize(Archive & ar)
			{
				ar(UvPos, UvSize);
			}
		};

		struct TileTextureData
		{
			int width, height, texture_data_size;
			std::vector<TileUv> TilesUV;
			BYTE* pic;
			template<class Archive>
			void serialize(Archive & ar)
			{
				ar(width, height, texture_data_size, TilesUV);
				pic = new BYTE[texture_data_size];
				ar(cereal::binary_data(pic, texture_data_size));
			}
		};

#define MAP_RENDERER_MAX_SPRITES    60000
#define MAP_RENDERER_VERTEX_SIZE    sizeof(VertexData)
#define MAP_RENDERER_SPRITE_SIZE    MAP_RENDERER_VERTEX_SIZE * 4
#define MAP_RENDERER_BUFFER_SIZE    MAP_RENDERER_SPRITE_SIZE * MAP_RENDERER_MAX_SPRITES
#define MAP_RENDERER_INDICES_SIZE   MAP_RENDERER_MAX_SPRITES * 6

#define MAP_SHADER_VERTEX_INDEX		0
#define MAP_SHADER_UV_INDEX			1
#define MAP_SHADER_COLOR_INDEX		2
		
		class MapRenderable;
		class MapRenderer
		{
		//Renderer
		private:
			std::vector<maths::mat4> m_TransformationStack;
			const maths::mat4* m_TransformationBack;
		public:
			void push(const maths::mat4& matrix, bool override = false)
			{
				if (override)
					m_TransformationStack.push_back(matrix);
				else
					m_TransformationStack.push_back(m_TransformationStack.back() * matrix);
				m_TransformationBack = &m_TransformationStack.back();
			}
			void pop()
			{
				if (m_TransformationStack.size() > 1)
					m_TransformationStack.pop_back();

				m_TransformationBack = &m_TransformationStack.back();
			}

		//Map Renderer
		private:
			GLuint m_VAO;
			GLuint m_VBO;
			graphics::IndexBuffer* m_IBO;
			GLsizei m_IndexCount;
			VertexData* m_Buffer;
			unsigned int m_Ttid;
			TileTextureData m_TTD;

		public:
			explicit MapRenderer(graphics::Texture* tex);
			~MapRenderer();
			void begin();
			void submit(const MapRenderable* renderable);
			void end();
			void flush();
			void setTexture(int tile_index);
		private:
			void init();
			std::vector<maths::vec2> create_uv_by_pos_size(maths::vec2 pos, maths::vec2 size, maths::vec2 tex_size);
		};
	}
}
