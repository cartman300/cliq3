#pragma once

#ifndef SERVER
#error Must compile under SERVER
#endif

namespace CLIq3 {
	public ref class Pointer {
	public:
		void* Ptr;

		Pointer();
		Pointer(void* Ptr);

		static Pointer^ FromIntPtr(IntPtr Ptr);
		static Pointer^ Zero = Pointer::FromIntPtr(IntPtr::Zero);
	};

	public ref class EntPtr : Pointer {
	public:
		EntPtr(void* Ent);

		static EntPtr^ FromIntPtr(IntPtr Ptr);
		static EntPtr^ Zero = gcnew EntPtr(0);
	};

	public ref class ClientPtr : Pointer {
	public:
		ClientPtr(void* Client);

		static ClientPtr^ FromIntPtr(IntPtr Ptr);
		static ClientPtr^ Zero = gcnew ClientPtr(0);
	};
}