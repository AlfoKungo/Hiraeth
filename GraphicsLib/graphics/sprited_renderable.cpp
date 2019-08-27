#include "sprited_renderable.h"
#include "texture_manager.h"


namespace hiraeth
{
	namespace graphics
	{
		SpritedRenderable::SpritedRenderable(maths::vec3 position, SRL::AnimationData animation_data,
			Texture* ptex, bool is_one_time )
			: Renderable(position, maths::vec2(ptex->getWidth() / float(animation_data.frames_data.size()),
				ptex->getHeight()), 0xffffffff, animation_data.frames_data[0].origin),
			m_FramesAmount(animation_data.frames_data.size()), //this
			m_FrameWidth(ptex->getWidth() / float(animation_data.frames_data.size())),
			m_IsLoop(animation_data.is_loop),
			m_AniData(animation_data),
			m_IsOneTime(is_one_time)

		{
			m_Texture = ptex;
			update_frame();
			m_AnimationTimer.reSet(m_AniData.frames_data[m_FrameIndex].delay);
		}

		SpritedRenderable::SpritedRenderable(maths::vec3 position, const std::string& tex_name,
			SRL::FullAnimationData animation_data, bool is_one_time )
			: SpritedRenderable(position, animation_data.animation_data, 
			//: SpritedRenderable(position,SRL::AnimationData{},
				//TextureManager::Load(tex_name), is_one_time)
				TextureManager::Load(tex_name, animation_data.animation_texture ), is_one_time)
		{
		}

		SpritedRenderable::SpritedRenderable(maths::vec3 position, unsigned int frames_amount,
		                                     float delay, bool is_loop, Texture* ptex)
			: SpritedRenderable(position, generate_animation_data(frames_amount, delay, is_loop, ptex->getSize()), ptex)
		{
		}

		void SpritedRenderable::update()
		{
			if (m_AnimationTimer.hasExpired())
			{
				if (m_IsOneTime && m_FrameIndex + 1 == m_FramesAmount)
				{
					m_IsFinished = true;
					//m_Texture = nullptr;
					return;
				}
				m_FrameIndex += m_LoopDirection;
				if (!m_IsLoop || m_FramesAmount == 1)
				{
					m_FrameIndex %= m_FramesAmount;
				}
				else
				{
					if ((m_FrameIndex == 0) || (m_FrameIndex == m_FramesAmount - 1))
					{
						m_LoopDirection *= -1;
					}
				}
				update_frame();
				m_AnimationTimer.reSet(m_AniData.frames_data[m_FrameIndex].delay);
			}
		}

		void SpritedRenderable::resetState()
		{
			m_FrameIndex = 0;
			m_LoopDirection = 1;
			m_AnimationTimer.reSet(m_AniData.frames_data[m_FrameIndex].delay);
			update_frame();
		}

		void SpritedRenderable::update_frame()
		{
			m_UV = update_uv();
			m_Org = m_AniData.frames_data[m_FrameIndex].origin;
		}

		std::vector<maths::vec2> SpritedRenderable::update_uv() const
		{
			auto&& tex_size = m_Texture->getSize();
			maths::vec2 pos(m_AniData.frames_data[m_FrameIndex].frame_box.position);
			maths::vec2 size(m_AniData.frames_data[m_FrameIndex].frame_box.size);
			std::vector<maths::vec2> uv;
			uv.push_back((pos) / tex_size);
			uv.push_back((pos + maths::vec2(0, size.y)) / tex_size);
			uv.push_back((pos + maths::vec2(size.x, size.y)) / tex_size);
			uv.push_back((pos + maths::vec2(size.x, 0)) / tex_size);
			return uv;
		}
	}
}
