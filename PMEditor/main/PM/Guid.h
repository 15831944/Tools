#pragma once
class CGuid
{
private:
	GUID m_val;

public:
	CGuid(){ memset(&m_val, 0, sizeof(m_val)); }
	static CGuid NewGuid(){ CGuid guid; ::CoCreateGuid(&guid.m_val); return guid; }

	CGuid& operator = (const CGuid& guid){ memcpy(&m_val, &guid.m_val, sizeof(m_val)); return *this; }
	bool IsEmpty(){ CGuid temp;	return temp == *this; }
	bool operator == (const CGuid& guid) const { return memcmp(&m_val, &guid.m_val, sizeof(m_val)) == 0; }
	bool operator != (const CGuid& guid) const { return memcmp(&m_val, &guid.m_val, sizeof(m_val)) != 0; }
	bool operator >(const CGuid& guid) const { return memcmp(&m_val, &guid.m_val, sizeof(m_val)) > 0; }
	bool operator <(const CGuid& guid) const { return memcmp(&m_val, &guid.m_val, sizeof(m_val)) < 0; }
	bool operator >=(const CGuid& guid) const { return memcmp(&m_val, &guid.m_val, sizeof(m_val)) >= 0; }
	bool operator <=(const CGuid& guid) const { return memcmp(&m_val, &guid.m_val, sizeof(m_val)) <= 0; }

	bool operator == (CGuid& guid){ return memcmp(&m_val, &guid.m_val, sizeof(m_val)) == 0; }
	bool operator != (CGuid& guid){ return memcmp(&m_val, &guid.m_val, sizeof(m_val)) != 0; }
	bool operator >(CGuid& guid){ return memcmp(&m_val, &guid.m_val, sizeof(m_val)) > 0; }
	bool operator <(CGuid& guid){ return memcmp(&m_val, &guid.m_val, sizeof(m_val)) < 0; }
	bool operator >=(CGuid& guid){ return memcmp(&m_val, &guid.m_val, sizeof(m_val)) >= 0; }
	bool operator <=(CGuid& guid){ return memcmp(&m_val, &guid.m_val, sizeof(m_val)) <= 0; }

	CString ToString()
	{
		CString str;
		str.Format(_T("{%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X}"),
			m_val.Data1, m_val.Data2, m_val.Data3,
			m_val.Data4[0], m_val.Data4[1],
			m_val.Data4[2], m_val.Data4[3],
			m_val.Data4[4], m_val.Data4[5],
			m_val.Data4[6], m_val.Data4[7]);
		return str;
	}
};

