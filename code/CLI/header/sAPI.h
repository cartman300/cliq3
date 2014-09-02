
namespace CLIq3 {
	public ref class sAPIAddon {
	public:
		virtual void Load() {}
		virtual void Unload() {}
	};

	public ref class sAPI {
	private:
		static List<sAPIAddon^>^ Instances = gcnew List<sAPIAddon^>();

	public:
		static void Print(String^ S);

	internal:
		static void LoadPlugins();
		static void LoadPlugin(Assembly^ Asm);

		static void UnloadPlugins();
	};
}