﻿using System;
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
			try {
				R = new Random();

				H = new CommandHost();
				H.RegisterDefaultCommands();
			} catch (Exception E) {
				sAPI.PrintError(E.ToString() + "\n");
			}
		}

		public override void Command(string S) {
			try {
				EvaluationResult R = H.Evaluate(S);
				if (!string.IsNullOrWhiteSpace(R.Output)) {
					string Clr = sAPI.ConColor.Green;
					if (!R.TruthValue) {
						Clr = sAPI.ConColor.Red;
						sAPI.Print(Clr + R.Status + ": ");
					}
					sAPI.Print(Clr + R.Output + "\n");
				} else if (!R.TruthValue)
					sAPI.Print(sAPI.ConColor.Red + R.Status + "\n");
			} catch (Exception E) {
				sAPI.PrintError(E.ToString() + "\n");
			}
		}

		public override bool FireWeapon(EntPtr Ply, bool AltFire, Vec3 Muzzle, Vec3 Forward) {
			try {
				if (AltFire)
					return AltFireWeapon(Ply, Muzzle, Forward);
				else
					return PrimaryFireWeapon(Ply, Muzzle, Forward);
			} catch (Exception E) {
				sAPI.PrintError(E.ToString() + "\n");
			}
			return false;
		}

		bool PrimaryFireWeapon(EntPtr Ply, Vec3 Muzzle, Vec3 Forward) {
			return false;
		}

		bool AltFireWeapon(EntPtr Ply, Vec3 Muzzle, Vec3 Forward) {
			Weapon W = Ply.GetWeapon();

			if (W == Weapon.WP_MACHINEGUN) {
				sQuake.Fire_bullet(Ply, 200 * 2, 14 * 2, MeansOfDeath.MOD_MACHINEGUN, Muzzle, Forward);
				return true;
			} else if (W == Weapon.WP_ROCKET_LAUNCHER) {
				EntPtr R = sQuake.Fire_rocket(Ply, Muzzle, Forward);
				Trajectory T = R.GetTrajectory();
				T.Type = TrajectoryType.TR_GRAVITY;
				R.SetTrajectory(T);
				return true;
			} else if (W == Weapon.WP_GRENADE_LAUNCHER) {
				Mods.FireFancyGrenades(Ply, Muzzle, Forward, 3);
				return true;
			} else if (W == Weapon.WP_PLASMAGUN) {
				Mods.FireFrancyPlasma(Ply, Muzzle, Forward);
				return true;
			}

			return true;
		}

		public override void EntityCreated(EntPtr E) {
			try {
				if (E.GetClassname() == "plasma" || E.GetClassname() == "bfg") {
					Trajectory T = E.GetTrajectory();
					T.Delta = T.Delta.Randomize(25);
					E.SetTrajectory(T);
				}
			} catch (Exception Ex) {
				sAPI.PrintError(Ex.ToString() + "\n");
			}
		}
	}
}