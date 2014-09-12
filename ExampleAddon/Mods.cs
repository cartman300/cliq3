using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CLIq3;

namespace Example {
	class Mods {
		public static void Randomize(EntPtr R, int Amp) {
			Trajectory T = R.GetTrajectory();
			R.SetOrigin(T.Base = sAPI.EvaluateTrajectory(T, sAPI.GetTime()));
			T.Delta = T.Delta.Randomize(Amp);
			T.Time = sAPI.GetTime() - 50;
			R.SetTrajectory(T);
		}

		public static EntPtr FireStraightGrenade(EntPtr Ply, Vec3 Muzzle, Vec3 Forward) {
			EntPtr R = sQuake.Fire_grenade(Ply, Muzzle, Forward);
			R.SetNextThink(sAPI.GetTime() + 2000 + svAddon.R.Next(-350, 350));

			Trajectory T = R.GetTrajectory();
			T.Type = TrajectoryType.TR_LINEAR;
			R.SetTrajectory(T);

			Randomize(R, 60);
			return R;
		}

		public static void FireFancyGrenades(EntPtr Ply, Vec3 Muzzle, Vec3 Forward, int Grenades) {
			for (int i = 0; i < Grenades; i++) {
				EntPtr G = Mods.FireStraightGrenade(Ply, Muzzle, Forward);
				G.SetDamage(G.GetDamage() / (Grenades / 2));
				G.SetSplashDamage(G.GetSplashDamage() / (Grenades / 2));
			}
		}

		public static EntPtr FireFrancyPlasma(EntPtr Ply, Vec3 Muzzle, Vec3 Forward) {
			EntPtr P = sQuake.Fire_plasma(Ply, Muzzle, Forward);
			Trajectory Tr = P.GetTrajectory();
			Tr.Delta.Normalize();
			Tr.Delta = Forward.Mult(1000);
			P.SetTrajectory(Tr);

			int ThinkTime = 50;

			ThinkFunc TF = P.GetThinkFunc();
			int OldThinkTime = P.GetNextThink();

			P.SetNextThink(ThinkTime);
			P.SetThinkFunc((_) => {
				try {
					EntPtr PE = EntPtr.FromIntPtr(_);

					Trajectory T = PE.GetTrajectory();
					Vec3 Pos = sAPI.EvaluateTrajectory(T, sAPI.GetTime());

					EntPtr PE2 = sQuake.Fire_plasma(Ply, Pos, Vector.RandomizeDir());
					PE2.SetNextThink(sAPI.GetTime() + 200);
					PE2.SetThinkFunc((__) => {
						EntPtr.FromIntPtr(__).Free();
					});
					T = PE2.GetTrajectory();
					T.Delta = T.Delta.Norm().Mult(100);
					PE2.SetTrajectory(T);

					if (sAPI.GetTime() > OldThinkTime)
						TF(_);

					PE.SetNextThink(ThinkTime);
				} catch (Exception Ex) {
					sAPI.PrintError(Ex.ToString() + "\n");
				}
			});

			return P;
		}
	}
}