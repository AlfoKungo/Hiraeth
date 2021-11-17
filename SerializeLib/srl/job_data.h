#pragma once


namespace SRL {

	const unsigned int
		JobBeginner = 0,
		JobBerserker = 1,
		JobCrusaderKnight = 2,
		JobWizard = 3,
		JobRogue = 4,
		JobArcher = 5,
		JobForestFighter = 6;
	//JobBerserker = 1000,
	//JobCrusaderKnight = 2000,
	//JobWizard = 3000,
	//JobRogue = 4000,
	//JobArcher = 5000,
	//JobForestFighter = 6000;

	using SkillsList = std::vector<unsigned int>;
	//struct JobData
	//{
	//	std::vector<unsigned int> job_skills;
	//	template<class A> void serialize(A& ar) {
	//		ar(CEREAL_NVP(job_skills));
	//	}
	//};

	struct FirstJobTree
	{
		std::vector<unsigned int> first_job_skills;
		std::map<unsigned int, std::vector<unsigned int>> second_job_skills;
	};

	enum JobsTypes
	{
		Novice,
		Berserker,
		CrusaderKnight,
		Wizard,
		Rogue,
		Archer,
		ForestFighter,
	};

	struct AllJobsData
	{
		SkillsList zero_jobs;
		std::map<JobsTypes, FirstJobTree> first_jobs;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(zero_jobs), CEREAL_NVP(first_jobs));
		}
	};

	const SRL::AllJobsData JobInfo{
				{{0,1,2}},
		{{ SRL::Berserker, {{5,6,7,8,9}}},
				{ SRL::CrusaderKnight, {{10,11}}},
				{ SRL::Wizard, {{3,4}, {}}},
				{ SRL::Rogue, {{3,4}}},
				{ SRL::Archer, {{3,4}}},
				{ SRL::ForestFighter, {{1,2,3}}},
					} };

}
