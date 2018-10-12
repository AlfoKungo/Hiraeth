#pragma once
#include "basic/updatable.h"
#include "skill_icon.h"
#include "UI/ui_skills.h"
#include "graphics/percented_sprite.h"
#include "top_right_icon.h"

namespace hiraeth {
	namespace skills {

		class SkillManager : public Updatable
		{
			std::map<unsigned int, std::tuple<SRL::SkillInfo*, SRL::AnimationMap*>> m_SkillsData;
		public:
			ui::UiSkills * m_UiSkills;
			graphics::Layer<TopRightIcon> m_Layer;
			SkillManager(ui::UiSkills * ui_skills)
				: m_UiSkills(ui_skills),
				m_Layer(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), false)
			{
				add_skill(0);
				add_skill(1);
				add_skill(2);

			}

			void update() override
			{
				m_Layer.update();
				size_t size_before{ m_Layer.m_Renderables.size() };
				m_Layer.m_Renderables.erase(
					std::remove_if(begin(m_Layer.m_Renderables), end(m_Layer.m_Renderables),
						[](graphics::Renderable* ts) {return ts->hasSpriteFinished(); }), end(m_Layer.m_Renderables));
				if (m_Layer.m_Renderables.size() != size_before)
					reconfigure_icons();
			}

			void draw() const
			{
				m_Layer.render();
			}

			void add_skill(unsigned int index)
			{
				SRL::SkillData skill_data = SRL::deserial<SRL::SkillData>("serialized/skills.data", index);
				m_SkillsData[index] = m_UiSkills->add_skill(skill_data, 0);
			}

			SRL::SkillInfo* get_skill(unsigned int skill_index)
			{
				if (m_SkillsData.find(skill_index) != m_SkillsData.end())
					return std::get<0>(m_SkillsData[skill_index]);
				return nullptr;
			}

			SRL::AnimationMap* getAnimationData(unsigned int skill_index)
			{
				if (m_SkillsData.find(skill_index) != m_SkillsData.end())
					return std::get<SRL::AnimationMap*>(m_SkillsData[skill_index]);
				return nullptr;
			}

			std::vector<unsigned int> get_available_skills()
			{
				std::vector<unsigned int> vints;
				vints.reserve(m_SkillsData.size());
				for (auto const& imap : m_SkillsData)
					vints.push_back(imap.first);
				return vints;
			}

			void add_icon(const std::string& name, unsigned int skill_index, float duration)
			{
				std::vector<TopRightIcon*>* rends = &m_Layer.m_Renderables;
				rends->erase(std::remove_if(rends->begin(), rends->end(), // remove icon if already exists
					[skill_index](const TopRightIcon* rend) {return rend->getSkillIndex() == skill_index; }), rends->end());

				SRL::AnimationData ad{ {},false };
				ad.frames_data.push_back({ {0,0,32,32},{16,0}, duration });
				m_Layer.add(new TopRightIcon{ skill_index, {float(768 - m_Layer.m_Renderables.size() * 32), 418 },
					graphics::TextureManager::Load(name), duration });
			}

			void reconfigure_icons()
			{
				for (int i = 0; i < m_Layer.m_Renderables.size(); ++i)
				{
					m_Layer.m_Renderables[i]->setPosition({ float(768 - i * 32), 418 });
				}
			}

		};
	}
}
