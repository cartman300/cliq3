using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CLIq3;

namespace Example {
	static class Vector {
		public static Vec3 Randomize(this Vec3 T, int Amp) {
			T.X += svAddon.R.Next(-Amp, Amp);
			T.Y += svAddon.R.Next(-Amp, Amp);
			T.Z += svAddon.R.Next(-Amp, Amp);
			return T;
		}

		public static Vec3 RandomizeDir() {
			Vec3 V = new Vec3();
			V.X = (float)(svAddon.R.NextDouble() + svAddon.R.NextDouble() - 1);
			V.Y = (float)(svAddon.R.NextDouble() + svAddon.R.NextDouble() - 1);
			V.Z = (float)(svAddon.R.NextDouble() + svAddon.R.NextDouble() - 1);
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

		public static Vec3 Norm(this Vec3 V) {
			V.Normalize();
			return V;
		}
	}
}
