#pragma once

// The game's weather reported to the runtime's NUMOS sky.
//
// Each frame the sky's current and outgoing TESWeather and the transition
// fraction are read (structs.hpp, Weather) and pushed as
//   rtx.weather.gameWeather          "<form id>, fnv.<class>"
//   rtx.weather.gameWeatherOutgoing  the same for the outgoing weather
//   rtx.weather.gameWeatherBlend     the engine's transition fraction
// The class comes from the record's own data (classification flags, thunder
// frequency, day fog distance, wind) with the thresholds in [Weather], so a
// weather a mod adds is classed like any other. The runtime shows the preset
// bound to the first id it knows: a preset bound to a form ID wins over the
// class binding.
//
// Outside an engine transition the blend is -1, so a cut (console `fw`, which
// the engine applies with no blend) fades over the preset's transition time.
// For the first frames after a sky appears (a load, a worldspace change) the
// blend is 1 with outgoing = current, so the sky snaps to the weather.
// In an interior (nvse_link facts) the sky is not on screen, and the runtime
// re-bakes its sky tables on every frame a weather change lasts: there the
// current weather is reported the same way, so a cut and an engine
// transition alike are in place at once.
// Without a sky or a current weather (menus, interiors) or when the reads
// fail, the ids are empty and NUMOS shows the dev menu's preset or the
// settings alone.
//
// The kind of cell is pushed apart from the weather, whenever the plugin's
// facts are ready:
//   rtx.weather.gameInterior         True in an interior cell
// There the runtime skips the cloud layer on screen and the primary surface's
// own sun and moon shadow ray; the sky still lights the cell.

namespace comp::game::weather
{
	// Writes each class id into the weather.bindings line of the preset
	// [Weather] maps it to, and removes it from every other preset file.
	// Call once at startup, before the runtime reads the preset folder.
	void bind_presets();

	// Once per frame from the sky drive, after the bridge's warm-up.
	void update();

	// The weather last reported, for the session status line: "<form id> <class>",
	// "none", or empty before the first report.
	const std::string& current();
}
