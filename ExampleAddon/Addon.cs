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
			sAPI.Print("Loading example addon!\n");
		}

		public override void Unload() {
			sAPI.Print("Unloading example addon!\n");
		}
	}
}