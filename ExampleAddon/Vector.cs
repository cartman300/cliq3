using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CLIq3;

namespace Example {
	static class Vector {
		public static Vec3 Randomize(this Vec3 T, int Amp) {
			/*Vec3 Norm = T;
			Norm.Normalize();*/

			T.X += Addon.R.Next(-Amp, Amp);
			T.Y += Addon.R.Next(-Amp, Amp);
			T.Z += Addon.R.Next(-Amp, Amp);

			return T;
		}

		public static Vec3 RandomizeDir(this Vec3 V) {
			V.X *= (float)Addon.R.Next(-100, 100) / 100;
			V.Y *= (float)Addon.R.Next(-100, 100) / 100;
			V.Z *= (float)Addon.R.Next(-100, 100) / 100;
			return V;
		}

		public static Vec3 Add(this Vec3 A, Vec3 B) {
			A.X += B.X;
			A.Y += B.Y;
			A.Z += B.Z;
			return A;
		}

		public static Vec3 Sub(this Vec3 A, Vec3 B) {
			A.X -= B.X;
			A.Y -= B.Y;
			A.Z -= B.Z;
			return A;
		}

		public static Vec3 Mult(this Vec3 A, float F) {
			A.X *= F;
			A.Y *= F;
			A.Z *= F;
			return A;
		}
	}
}
