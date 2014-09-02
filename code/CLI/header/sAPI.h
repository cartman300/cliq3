
namespace CLIq3 {
	public ref class Entity {
	internal:
		IntPtr Ent;

	public:
		delegate void ThinkFunc(IntPtr self);
		delegate void ReachedFunc(IntPtr self);
		delegate void BlockedFunc(IntPtr self, IntPtr other);
		delegate void TouchFunc(IntPtr self, IntPtr other, IntPtr trace);
		delegate void UseFunc(IntPtr self, IntPtr other, IntPtr activator);
		delegate void PainFunc(IntPtr self, IntPtr attacker, int damage);
		delegate void DieFunc(IntPtr self, IntPtr inflictor, IntPtr attacker, int damage, int mod);

		Entity(IntPtr E);

		String^ GetClassname();
		void SetClassname(String^ S);

		DieFunc^ GetDieFunc();
		void SetDieFunc(DieFunc^ F);
	};

	public ref class sAPIAddon {
	public:
		virtual void Load(){}
		virtual void Unload(){}

		virtual void EntityCreated(Entity^ E){}
	};

	public ref class sAPI {
	private:
		static List<sAPIAddon^>^ Instances = gcnew List<sAPIAddon^>();

	public:
		ref class ConColor {
		public:
			literal String^ Black = "^0";
			literal String^ Red = "^1";
			literal String^ Green = "^2";
			literal String^ Yellow = "^3";
			literal String^ Blue = "^4";
			literal String^ Cyan = "^5";
			literal String^ Magenta = "^6";
			literal String^ White = "^7";
		};

		static void Print(String^ S);
		static void PrintError(String^ S);

	internal:
		static void LoadPlugins();
		static void LoadPlugin(Assembly^ Asm);
		static void UnloadPlugins();

		static void OnEntityCreated(Entity^ E);
	};
}