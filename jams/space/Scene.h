
#pragma once

#include <space/Forward.h>

using namespace mud;
using namespace toy;

void paint_star(Gnode& parent, Star& star);
void paint_scan_star(Gnode& parent, Star& star);
void paint_scan_fleet(Gnode& parent, Fleet& fleet);
void paint_galaxy(Gnode& parent, Galaxy& galaxy);
void paint_combat(Gnode& parent, SpatialCombat& combat);
void paint_scene(Gnode& parent);
void paint_viewer(Viewer& viewer);
