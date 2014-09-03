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
		static int Mag = 80;
		Random R;
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
			EvaluationResult R;
			try {
				R = H.Evaluate(S);
				if (!string.IsNullOrWhiteSpace(R.Output)) {
					string Clr = sAPI.ConColor.Green;
					if (!R.TruthValue)
						sAPI.Print((Clr = sAPI.ConColor.Red) + R.Status + ": ");
					sAPI.Print(Clr + R.Output + "\n");
				} else if (!R.TruthValue)
					sAPI.Print(sAPI.ConColor.Red + R.Status + "\n");
			} catch (Exception E) {
				sAPI.PrintError(E.Message + "\n");
			}
			return true;
		}

		public override void EntityCreated(IntPtr E) {
			if (Entity.GetClassname(E) == "rocket")
				for (int i = 0; i < 4; i++) {
					var Ent = Entity.Spawn();
					Entity.SetClassname(Ent, "rocket");
					Entity.SetNextThink(Ent, Entity.GetNextThink(E));
					Entity.SetThinkFunc(Ent, Entity.GetThinkFunc(E));
					Entity.SetEType(Ent, EntType.ET_MISSILE);
					Entity.SetSVFlags(Ent, SVFlags.USE_CURRENT_ORIGIN);
					Entity.SetWeapon(Ent, Weapon.WP_ROCKET_LAUNCHER);
					Entity.SetOwnerNum(Ent, Entity.GetOwnerNum(E));
					Entity.SetParent(Ent, Entity.GetParent(E));
					Entity.SetDamage(Ent, Entity.GetDamage(E));
					Entity.SetSplashDamage(Ent, Entity.GetSplashDamage(E));
					Entity.SetSplashRadius(Ent, 120);
					Entity.SetMethodOfDeath(Ent, MeansOfDeath.MOD_ROCKET);
					Entity.SetSplashMethodOfDeath(Ent, MeansOfDeath.MOD_ROCKET_SPLASH);
					Entity.SetClipmask(Ent, Clipmask.SHOT);
					Entity.SetTargetEnt(Ent, IntPtr.Zero);

					Trajectory T = Entity.GetTrajectory(E);
					T.Delta.X += R.Next(-Mag, Mag);
					T.Delta.Y += R.Next(-Mag, Mag);
					T.Delta.Z += R.Next(-Mag, Mag);
					Entity.SetTrajectory(Ent, T);
					Entity.SetOrigin(Ent, Entity.GetOrigin(E));
				}
		}
	}
}