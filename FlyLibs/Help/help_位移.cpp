
#define SHARE_CRYSTAL  0X01
#define SHARE_FISHTICKET 0x0010

struct a {
	int freeShare;
}m_jsonPlayerInfo;

void setFreeShare(int type)
{
	if (type == 1)
	{
		m_jsonPlayerInfo.freeShare |= SHARE_CRYSTAL;
	}
	else  if (type == 2)
	{
		m_jsonPlayerInfo.freeShare |= SHARE_FISHTICKET;
	}
}
bool getFreeShare(int type)
{
	if (type == 1)
	{
		if (m_jsonPlayerInfo.freeShare & SHARE_CRYSTAL)
		{
			m_jsonPlayerInfo.freeShare &= ~SHARE_CRYSTAL;
			return true;
		}
	}
	else  if (type == 2)
	{
		if (m_jsonPlayerInfo.freeShare & SHARE_FISHTICKET)
		{
			m_jsonPlayerInfo.freeShare &= ~SHARE_FISHTICKET;
			return true;
		}
	}
	return false;
}
