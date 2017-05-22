#include "batchrenderer2d.h"


//#define SHOW_ATLAS

namespace hiraeth {
	namespace graphics {

		BatchRenderer2D::BatchRenderer2D()
			: m_IndexCount(0)
		{
			init();
		}

		BatchRenderer2D::~BatchRenderer2D()
		{
			delete m_IBO;
			glDeleteBuffers(1, &m_VBO);
		}

		void BatchRenderer2D::init()
		{
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);

			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
			glEnableVertexAttribArray(SHADER_UV_INDEX);
			glEnableVertexAttribArray(SHADER_TID_INDEX);
			glEnableVertexAttribArray(SHADER_COLOR_INDEX);

			glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)0);
			glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::uv)));
			glVertexAttribPointer(SHADER_TID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::tid)));
			glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::color)));

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			GLuint* indices = new GLuint[RENDERER_INDICES_SIZE];

			int offset = 0;
			for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
			{
				indices[i] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}
			m_IBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);

			glBindVertexArray(0);


#ifdef SHOW_ATLAS
			std::string nig = "24sdasdjkh7";
			for (int c = 0; c < nig.length(); c++)
			{
				const char *d = &nig[c];
				texture_font_get_glyph(m_FTFont, d);
			}
#endif
		}

		void BatchRenderer2D::begin()
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			m_Buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		}

		void  BatchRenderer2D::submit(const Renderable2D* renderable, unsigned int blendColor)
		{
			const maths::vec3& position = renderable->getPosition();
			const maths::vec2& size = renderable->getSize();
			const unsigned int color = renderable->getColor() & blendColor;
			const std::vector<maths::vec2>& uv = renderable->getUV();
			const GLuint tid = renderable->getTID();

			float ts = 0.0f;
			if (tid > 0)
			{
				bool found = false;
				for (int i = 0; i < m_TextureSlots.size(); i++)
				{
					if (m_TextureSlots[i] == tid)
					{
						ts = (float)(i + 1);
						found = true;
						break;
					}
				}
				if (!found)
				{
					if (m_TextureSlots.size() >= RENDERER_MAX_TEXTURES)
					{
						end();
						flush();
						begin();
					}
					m_TextureSlots.push_back(tid);
					ts = (float)(m_TextureSlots.size());
				}
			}

			m_Buffer->vertex = *m_TransformationBack * position;
			m_Buffer->uv = uv[0];
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x, position.y + size.y, position.z);
			m_Buffer->uv = uv[1];
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y + size.y, position.z);
			m_Buffer->uv = uv[2];
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y, position.z);
			m_Buffer->uv = uv[3];
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_IndexCount += 6;
		}

		void BatchRenderer2D::drawString(const Font& font, const std::string& text, const maths::vec3& position, unsigned int color)
		{
			using namespace ftgl;

			float ts = 0.0f;
			bool found = false;
			for (int i = 0; i < m_TextureSlots.size(); i++)
			{
				if (m_TextureSlots[i] == font.getID())
				{
					ts = (float)(i + 1);
					found = true;
					break;
				}
			}
			if (!found)
			{
				if (m_TextureSlots.size() >= RENDERER_MAX_TEXTURES)
				{
					end();
					flush();
					begin();
				}
				m_TextureSlots.push_back(font.getID());
				ts = (float)(m_TextureSlots.size());
			}


			font.submitFont();

			texture_font_t* ft_font = font.getFTFont();
#ifndef SHOW_ATLAS
			float x = position.x;
			for (int i = 0; i < text.length(); i++)
			{
				//char c = text.at(i);
				const char *c = &text[i];
				texture_glyph_t* glyph = texture_font_get_glyph(ft_font, c);

				if (glyph != NULL)
				{
					if (i > 0)
					{
						float kerning = texture_glyph_get_kerning(glyph, c);
						x += kerning;
					}
					float x0 = x + glyph->offset_x;
					float y0 = position.y + glyph->offset_y;
					float x1 = x0 + glyph->width;
					float y1 = y0 - glyph->height;

					float u0 = glyph->s0;
					float v0 = glyph->t0;
					float u1 = glyph->s1;
					float v1 = glyph->t1;


					m_Buffer->vertex = *m_TransformationBack * maths::vec3(x0, y0, 0);
					m_Buffer->uv = maths::vec2(u0, v0);
					m_Buffer->tid = ts;
					m_Buffer->color = color;
					m_Buffer++;

					m_Buffer->vertex = *m_TransformationBack * maths::vec3(x0, y1, 0);
					m_Buffer->uv = maths::vec2(u0, v1);
					m_Buffer->tid = ts;
					m_Buffer->color = color;
					m_Buffer++;

					m_Buffer->vertex = *m_TransformationBack * maths::vec3(x1, y1, 0);
					m_Buffer->uv = maths::vec2(u1-0.0005, v1);
					m_Buffer->tid = ts;
					m_Buffer->color = color;
					m_Buffer++;

					m_Buffer->vertex = *m_TransformationBack * maths::vec3(x1, y0, 0);
					m_Buffer->uv = maths::vec2(u1-0.0005, v0);
					m_Buffer->tid = ts;
					m_Buffer->color = color;
					m_Buffer++;

					m_IndexCount += 6;

					x += glyph->advance_x;
				}
			}
#else
			m_Buffer->vertex = maths::vec3(-200, -200, 0);
			m_Buffer->uv = maths::vec2(0, 1);
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = maths::vec3(-200, 200, 0);
			m_Buffer->uv = maths::vec2(0, 0);
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = maths::vec3(200, 200, 0);
			m_Buffer->uv = maths::vec2(1, 0);
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_Buffer->vertex = maths::vec3(200, -200, 0);
			m_Buffer->uv = maths::vec2(1, 1);
			m_Buffer->tid = ts;
			m_Buffer->color = color;
			m_Buffer++;

			m_IndexCount += 6;
#endif
		}

		void BatchRenderer2D::end()
		{
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void BatchRenderer2D::flush()
		{
			for (int i = 0; i < m_TextureSlots.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_TextureSlots[i]);
			}


			glBindVertexArray(m_VAO);
			m_IBO->bind();

			glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);

			m_IBO->unbind();
			glBindVertexArray(0);
			m_IndexCount = 0;

			m_TextureSlots.clear();
		}
	}
}