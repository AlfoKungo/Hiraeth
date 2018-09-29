#pragma once
#include "basic/updatable.h"
#include "skill_icon.h"
#include "UI/ui_skills.h"

namespace hiraeth {
	namespace skills {

		class SkillManager : public Updatable
		{
			std::map<unsigned int, SRL::SkillInfo > m_SkillsInfo;
			std::map<unsigned int, std::tuple<SRL::SkillInfo*, SRL::AnimationMap*>> m_SkillsData;
			ui::UiSkills * m_UiSkills;
		public:
			graphics::Layer<graphics::Renderable> m_Layer;
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
						[](graphics::Renderable* ts) {return ts->is_sprite_finished(); }), end(m_Layer.m_Renderables));
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
				m_SkillsInfo[index] = skill_data.skill_info;
			}

			SRL::SkillInfo* get_skill(unsigned int skill_index)
			{
				if (m_SkillsInfo.find(skill_index) != m_SkillsInfo.end())
					return &m_SkillsInfo[skill_index];
				return nullptr;
			}

			SRL::AnimationMap* getAnimationData(unsigned int skill_index)
			{
				if (m_SkillsInfo.find(skill_index) != m_SkillsInfo.end())
					return std::get<SRL::AnimationMap*>(m_SkillsData[skill_index]);
				return nullptr;
			}

			std::vector<unsigned int> get_available_skills()
			{
				std::vector<unsigned int> vints;
				vints.reserve(m_SkillsInfo.size());
				for (auto const& imap : m_SkillsInfo)
					vints.push_back(imap.first);
				return vints;
			}

			void add_icon(const std::string& name, float duration)
			{
				SRL::AnimationData ad{ {},false };
				ad.frames_data.push_back({ {0,0,32,32},{16,0}, duration });
				m_Layer.add(new graphics::SpritedRenderable{ {float(752 - m_Layer.m_Renderables.size() * 32), 418 }, ad,
					graphics::TextureManager::Load(name), true });
			}

			void reconfigure_icons()
			{
				for (int i = 0; i < m_Layer.m_Renderables.size(); ++i)
				{
					m_Layer.m_Renderables[i]->setPosition({ float(752 - i * 32), 418 });
				}
			}
		};
	}
}
