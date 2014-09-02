using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CLIq3;

namespace Example {
	public class Addon : sAPIAddon {
		public Addon() {
		}

		public override void Load() {
		}

		public override void Unload() {
		}

		public override void EntityCreated(Entity E) {
			sAPI.Print(sAPI.ConColor.Blue + "Entity " + E.GetClassname() + " created!\n");

			Entity.DieFunc DFunc = E.GetDieFunc();

			E.SetDieFunc((Self, Inf, Att, Dmg, Mod) => {
				sAPI.Print(sAPI.ConColor.Cyan + "Entity " + E.GetClassname() + " died!\n");
				DFunc(Self, Inf, Att, Dmg, Mod);
			});
		}
	}
}