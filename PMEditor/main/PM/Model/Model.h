
namespace Model
{
	namespace Item{ class CTagGroup; }
	class CModel
	{
	private:
		std::shared_ptr<Item::CTagGroup> m_Root;

	public:
		CModel(void);
		void OnCreateModel();
	};
}