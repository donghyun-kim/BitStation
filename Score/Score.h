#pragma once


class CScore
{
public :
	CScore();
	~CScore();

	int Judgement(float postion);
	void InitScore();

//private:
	int m_Best;
	int m_Good;
	int m_Nomal;
	int m_Bad;
	int m_Pool;
	int m_Miss;
	int m_Score;
	int m_Combo;
	int m_LastMaxCombo;
};