#include "Guid.h"

namespace Model{
	namespace Item{
		class CTagGroup;
		class CTagMgr
		{
		private:
			std::list<std::shared_ptr<CTagGroup>> m_ltGroup;
			std::map<CString, std::shared_ptr<CTagGroup>> m_mpGroup;
			std::map<CGuid, std::shared_ptr<CTagGroup>> m_mpGroup_ID;

		public:
			int GroupSize(){ return (int)m_ltGroup.size(); }
			auto GroupBegin()->decltype(m_mpGroup.begin()){ return m_mpGroup.begin(); }
			auto GroupEnd()->decltype(m_mpGroup.end()){ return m_mpGroup.end(); }
			auto GroupRBegin()->decltype(m_mpGroup.rbegin()){ return m_mpGroup.rbegin(); }
			auto GroupREnd()->decltype(m_mpGroup.rend()){ return m_mpGroup.rend(); }

		public:
			CTagMgr(void);
			~CTagMgr();

			bool GroupExist(const CGuid& id);
			bool GroupExist(const CString& name);
			bool AddGroup(const CString& name);
			bool RemoveGroup(const CString& name);
			bool RemoveGroup(const CGuid& id);

			std::shared_ptr<CTagGroup> GetGroup(const CGuid& id);
			std::shared_ptr<CTagGroup> GetGroup(const CString& name);
		};
	}
}