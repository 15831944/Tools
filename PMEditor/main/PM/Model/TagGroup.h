#include "Guid.h"

namespace Model{
	namespace Item{
		class CTag;
		class CTagGroup
		{
		private:
			CGuid m_id;
			CString m_name;

			std::list<std::shared_ptr<CTagGroup>> m_ltGroup;
			std::map<CString, std::shared_ptr<CTagGroup>> m_mpGroup;
			std::map<CGuid, std::shared_ptr<CTagGroup>> m_mpGroup_ID;

			std::map<CString, std::shared_ptr<CTag>> m_mpTag;
			std::map<CGuid, std::shared_ptr<CTag>> m_mpTag_ID;

		public:
			CGuid& ID(){ return m_id; }
			CString& Name(){ return m_name; }

			int GroupSize(){ return (int)m_ltGroup.size(); }
			auto GroupBegin()->decltype(m_mpGroup.begin()){ return m_mpGroup.begin(); }
			auto GroupEnd()->decltype(m_mpGroup.end()){ return m_mpGroup.end(); }
			auto GroupRBegin()->decltype(m_mpGroup.rbegin()){ return m_mpGroup.rbegin(); }
			auto GroupREnd()->decltype(m_mpGroup.rend()){ return m_mpGroup.rend(); }

			int TagSize(){ return (int)m_ltGroup.size(); }
			auto TagBegin()->decltype(m_mpTag.begin()){ return m_mpTag.begin(); }
			auto TagEnd()->decltype(m_mpTag.end()){ return m_mpTag.end(); }
			auto TagRBegin()->decltype(m_mpTag.rbegin()){ return m_mpTag.rbegin(); }
			auto TagREnd()->decltype(m_mpTag.rend()){ return m_mpTag.rend(); }

		public:
			CTagGroup(const CString& name);
			~CTagGroup();

			bool GroupExist(const CGuid& id);
			bool GroupExist(const CString& name);
			bool AddGroup(const CString& name);
			bool RemoveGroup(const CString& name);
			bool RemoveGroup(const CGuid& id);

			std::shared_ptr<CTagGroup> GetGroup(const CGuid& id);
			std::shared_ptr<CTagGroup> GetGroup(const CString& name);

			bool TagExist(const CGuid& id);
			bool TagExist(const CString& name);
			bool AddTag(const CString& name);
			bool RemoveTag(const CString& name);
			bool RemoveTag(const CGuid& id);

			std::shared_ptr<CTag> GetTag(const CGuid& id);
			std::shared_ptr<CTag> GetTag(const CString& name);

		};
	}
}