#include "score.h"


CScore::CScore()
:m_Best(0),m_Good(0),m_Nomal(0),m_Bad(0),
m_Pool(0),m_Miss(0) ,m_Score(0)
{
}

CScore::~CScore()
{
}

int CScore::Judgement(float postion)
{
	
	
	if(postion <= 400)
	{	if(postion <= 400 && postion >= 219)
		{
			
			m_Pool++;
			
			if(m_LastMaxCombo < m_Combo)
			{
				m_LastMaxCombo= m_Combo;
			}
			return 4;
		}
		else if(postion <= 220 && postion >= 189)
		{
			m_Nomal++;
			m_Combo++;
			
			if(m_LastMaxCombo < m_Combo)
			{
				m_LastMaxCombo= m_Combo;
			}
			return 3;
		}

		else if(postion <= 190 && postion >= 179)
		{
			m_Good++;
			m_Combo++;
			
			if(m_LastMaxCombo < m_Combo)
			{
				m_LastMaxCombo= m_Combo;
			}
			return 2;
		}

		else if(postion <= 180 && postion >= 170)
		{
			m_Best++;
			m_Combo++;
			
			if(m_LastMaxCombo < m_Combo)
			{
				m_LastMaxCombo= m_Combo;
			}
			return 1;

		}

		else if(postion <= 169 && postion >= 163)
		{
			m_Bad++;
			
			if(m_LastMaxCombo < m_Combo)
			{
				m_LastMaxCombo= m_Combo;
			}
			m_Combo=0;
			return 5;
		}

		else {
			m_Miss++;
			
			if(m_LastMaxCombo < m_Combo)
			{
				m_LastMaxCombo= m_Combo;
			}
			m_Combo=0;
			return 6;
		}
		
	}
	

	return 0;
}

void CScore::InitScore()
{
	m_Nomal= 0;
	m_Good= 0;
	m_Best= 0;
	m_Bad= 0;
	m_Pool= 0;
	m_Miss= 0;
	m_Score =0;
	m_Combo =0;
	m_LastMaxCombo =0;

}