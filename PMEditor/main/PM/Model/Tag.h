#include "Guid.h"

namespace Model{
	namespace Item{
		class CTag
		{
		private:
			CGuid m_id;
			CString m_name;

		public:
			CGuid& ID(){ return m_id; }
			CString& Name(){ return m_name; }

		public:
			CTag(const CString& name);
			~CTag();
		};
	}
}