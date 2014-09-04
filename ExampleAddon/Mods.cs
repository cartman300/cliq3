using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CLIq3;

namespace Example {
	class Mods {
		public static void Randomize(IntPtr _) {
			try {
				EntPtr R = EntPtr.FromIntPtr(_);

				Trajectory T = R.GetTrajectory();
				R.SetOrigin(T.Base = sAPI.EvaluateTrajectory(T, sAPI.GetTime()));
				T.Delta = T.Delta.Randomize(80);
				T.Time = sAPI.GetTime() - 50;
				R.SetTrajectory(T);

				R.SetNextThink(sAPI.GetTime() + 100);
			} catch (Exception E) {
				sAPI.PrintError(E.Message + "\n");
			}
		}

		public static EntPtr FireGrenadeLancher(EntPtr Ply, Vec3 Muzzle, Vec3 Forward) {
			EntPtr R = sQuake.Fire_grenade(Ply, Muzzle, Forward);

			Trajectory T = R.GetTrajectory();
			T.Type = TrajectoryType.TR_LINEAR;
			R.SetTrajectory(T);

			ThinkFunc F = R.GetThinkFunc();
			int ShotTime = sAPI.GetTime();
			R.SetThinkFunc((_) => {
				Mods.Randomize(_);
				if (ShotTime < sAPI.GetTime() - 2500)
					F(_);
			});
			R.SetNextThink(sAPI.GetTime() + 200);

			return R;
		}
	}
}