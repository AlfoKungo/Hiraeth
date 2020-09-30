#include "hrth_pch.h"
#include "maprenderer.h"
#include "maprenderable.h"

namespace hiraeth {
	namespace map {

		MapRenderer::MapRenderer()
			//: m_Tex(tex)
		{
			m_TransformationStack.push_back(maths::mat4::Identity());
			m_TransformationBack = &m_TransformationStack.back();
			init();
			
			GLuint result;
			glGenTextures(1, &result);
			glBindTexture(GL_TEXTURE_2D, result);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_TTD.texture_data.pic);
			glBindTexture(GL_TEXTURE_2D, 0);

			//delete[] m_TTD.pic;
			m_Ttid = result;
		}

		MapRenderer::~MapRenderer() 
		{
			delete m_IBO;
			glDeleteBuffers(1, &m_VBO);
		}

		void MapRenderer::init()
		{
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);

			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, MAP_RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(MAP_SHADER_VERTEX_INDEX);
			glEnableVertexAttribArray(MAP_SHADER_UV_INDEX);
			glEnableVertexAttribArray(MAP_SHADER_COLOR_INDEX);

			glVertexAttribPointer(MAP_SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, MAP_RENDERER_VERTEX_SIZE, (const GLvoid*) 0);
			glVertexAttribPointer(MAP_SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, MAP_RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::uv)));
			glVertexAttribPointer(MAP_SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, MAP_RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::color)));

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			GLuint* indices = new GLuint[MAP_RENDERER_INDICES_SIZE];

			int offset = 0;
			for (int i = 0; i < MAP_RENDERER_INDICES_SIZE; i += 6)
			{
				indices[  i  ] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}
			m_IBO = new graphics::IndexBuffer(indices, MAP_RENDERER_INDICES_SIZE);

			glBindVertexArray(0);
		}

		void MapRenderer::begin()
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			m_Buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		}

		void  MapRenderer::submit(const MapRenderable* renderable)
		{
			const maths::vec3& position = renderable->getPosition();
			const unsigned int type = renderable->getType();
			const SRL::TileUv tile_uv = m_TTD.info[type];
			const maths::vec2& size = tile_uv.UvSize;
			const unsigned int color = renderable->getColor();
			const std::vector<maths::vec2>& uv = create_uv_by_pos_size(tile_uv.UvPos, tile_uv.UvSize, 
				m_TexSize);


			m_Buffer->vertex = *m_TransformationBack * position;
			m_Buffer->uv     = uv[0];
			m_Buffer->color  = color;
			m_Buffer++;

			m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x, position.y + size.y, 0);
			m_Buffer->uv = uv[1];
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y + size.y, 0);
			m_Buffer->uv = uv[2];
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y, 0);
			m_Buffer->uv = uv[3];
			m_Buffer->color = color;
			m_Buffer++;

			m_IndexCount += 6;
		}

		void MapRenderer::end()
		{
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void MapRenderer::flush()
		{
			glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, m_Tex->getID());
			glBindTexture(GL_TEXTURE_2D, m_Ttid);
			glBindVertexArray(m_VAO);
			m_IBO->bind();
			
			glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);

			m_IBO->unbind();
			glBindVertexArray(0);
			m_IndexCount = 0;
		}

		void MapRenderer::changeTexture(int tile_index)
		{
			reloadTTD(tile_index - 1);
			update_texture();
			m_TexSize = maths::vec2{ float(m_TTD.texture_data.width), float(m_TTD.texture_data.height)};
			delete[] m_TTD.texture_data.pic;
		}

		void MapRenderer::reloadTTD(int tile_index)
		{
			m_TTD = SRL::deserial<SRL::TileTextureData>("map", (200 + tile_index));
		}

		void MapRenderer::update_texture()
		{
			glBindTexture(GL_TEXTURE_2D, m_Ttid);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_TTD.texture_data.width, m_TTD.texture_data.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_TTD.texture_data.pic);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		std::vector<maths::vec2> MapRenderer::create_uv_by_pos_size(maths::vec2 pos, maths::vec2 size, maths::vec2 tex_size)
		{
			std::vector<maths::vec2> uv;
			uv.push_back((pos) / tex_size);
			uv.push_back((pos + maths::vec2(0, size.y)) / tex_size);
			uv.push_back((pos + maths::vec2(size.x, size.y)) / tex_size);
			uv.push_back((pos + maths::vec2(size.x, 0)) / tex_size);
			return uv;
		}
	}
}
