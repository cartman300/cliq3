using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CLIq3;
using vCommands;
using vCommands.Commands;
using vCommands.Parsing;
using vCommands.Variables;

namespace Example {
	public class Addon : sAPIAddon {
		public static Random R;
		CommandHost H;

		public Addon() {
		}

		public override void Load() {
			R = new Random();

			H = new CommandHost();
			H.RegisterDefaultCommands();
		}

		public override void Unload() {
		}

		public override bool Command(string S) {
			try {
				EvaluationResult R = H.Evaluate(S);
				if (!string.IsNullOrWhiteSpace(R.Output)) {
					if (!R.TruthValue)
						return false;
					sAPI.Print(sAPI.ConColor.Green + R.Output + "\n");
					return true;
				}
			} catch (Exception E) {
				sAPI.PrintError(E.Message + "\n");
			}
			return false;
		}

		public override bool FireWeapon(EntPtr Ply, Vec3 Muzzle, Vec3 Forward) {
			if (Ply.GetWeapon() == Weapon.WP_GRENADE_LAUNCHER) {
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
				return true;
			}

			return false;
		}

		public override void EntityCreated(EntPtr E) {
			if (E.GetClassname() == "plasma" || E.GetClassname() == "rocket" || E.GetClassname() == "bfg") {
				Trajectory T = E.GetTrajectory();
				T.Delta = T.Delta.Randomize(25);
				E.SetTrajectory(T);
			}
		}
	}
}