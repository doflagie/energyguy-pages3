// ============================================================
//  rollup_spool.scad — Roll-up Wire Storage Spool with Trap Clips
//  TM-PNL-ENC-003 Rev A
//  Pool Noodle Magnetic Loop Antenna Project
//  Print: PLA, 0.3mm layer, 20% gyroid infill (lightweight)
//  Bed footprint: 180 x 180 x 92mm — fits 200x200 bed
//  Print orientation: hub vertical (Z axis), flanges horizontal
// ============================================================

// ---- Parameters ----
HUB_D        = 52;    // hub outer diameter (fits over noodle end OD=47)
HUB_L        = 80;    // hub usable winding length (between flanges)
FLANGE_D     = 178;   // flange outer diameter
FLANGE_T     = 8;     // flange thickness
WALL         = 3.0;   // wall thickness
N_FINS       = 4;     // anti-twist fins on hub
FIN_H        = 7;     // fin height above hub surface
FIN_W        = 3;     // fin base width
N_CLIPS      = 6;     // trap clip positions on one flange
CLIP_R       = 68;    // clip position radius from hub center
CLIP_OW      = 24;    // clip outer width
CLIP_OD      = 14;    // clip outer depth (radial)
CLIP_WELL_H  = 16;    // clip well depth (Z)
CLIP_WALL    = 2.0;   // clip wall thickness
CLIP_LIP     = 3.0;   // retaining lip width at top of clip
STRAP_W      = 24;    // carry strap slot width
STRAP_H      = 12;    // carry strap slot height
CENTER_BORE  = 8;     // center pass-through (hanging peg / M5 bolt)
$fn          = 80;

// ---- Hub with anti-twist fins ----
module hub() {
    union() {
        // Main cylinder
        cylinder(h=HUB_L, d=HUB_D, center=true);
        // 4 fins equally spaced
        for (a = [0 : 360/N_FINS : 360 - 1])
            rotate([0, 0, a])
                translate([HUB_D/2, -FIN_W/2, 0])
                    cube([FIN_H, FIN_W, HUB_L], center=true);
    }
}

// ---- Plain flange (bottom) ----
module flange_plain() {
    difference() {
        cylinder(h=FLANGE_T, d=FLANGE_D);
        // Hub bore
        translate([0, 0, -0.1])
            cylinder(h=FLANGE_T + 0.2, d=HUB_D + 0.4);
        // Carry strap slot (at outer edge, one side)
        translate([FLANGE_D/2 - STRAP_H - 3, -STRAP_W/2, -0.1])
            cube([STRAP_H + 4, STRAP_W, FLANGE_T + 0.2]);
        // Center pass-through
        translate([0, 0, -0.1])
            cylinder(h=FLANGE_T + 0.2, d=CENTER_BORE);
        // Weight-reduction holes (6x)
        for (a = [0 : 60 : 300])
            rotate([0, 0, a + 30])
                translate([FLANGE_D/4, 0, -0.1])
                    cylinder(h=FLANGE_T + 0.2, d=FLANGE_D/8);
    }
}

// ---- Trap clip (individual, added to top flange face) ----
module trap_clip() {
    // U-shaped cradle: fits T50 toroid core assembly (~20mm wide, 12mm tall)
    difference() {
        cube([CLIP_OW, CLIP_OD, CLIP_WELL_H]);
        // Well interior
        translate([CLIP_WALL, CLIP_WALL, CLIP_WALL])
            cube([CLIP_OW - 2*CLIP_WALL, CLIP_OD - CLIP_WALL, CLIP_WELL_H]);
        // Front opening (access to place/remove trap)
        translate([-0.1, -0.1, CLIP_WALL])
            cube([CLIP_OW + 0.2, CLIP_WALL + 0.2, CLIP_WELL_H]);
    }
    // Retaining lip at top opening (prevents trap falling out)
    translate([0, CLIP_OD - CLIP_WALL - CLIP_LIP, CLIP_WELL_H - CLIP_WALL])
        cube([CLIP_OW, CLIP_LIP, CLIP_WALL]);
}

// ---- Top flange with trap clips ----
module flange_with_clips() {
    union() {
        // Base flange
        difference() {
            cylinder(h=FLANGE_T, d=FLANGE_D);
            translate([0, 0, -0.1])
                cylinder(h=FLANGE_T + 0.2, d=HUB_D + 0.4);
            translate([0, 0, -0.1])
                cylinder(h=FLANGE_T + 0.2, d=CENTER_BORE);
            // Weight holes
            for (a = [0 : 60 : 300])
                rotate([0, 0, a + 30])
                    translate([FLANGE_D/4, 0, -0.1])
                        cylinder(h=FLANGE_T + 0.2, d=FLANGE_D/8);
        }
        // 6 trap clips on top face of this flange
        for (a = [0 : 360/N_CLIPS : 360 - 1])
            rotate([0, 0, a])
                translate([CLIP_R - CLIP_OD/2, -CLIP_OW/2, FLANGE_T])
                    trap_clip();
    }
}

// ---- Raised winding instruction text ----
module hub_label() {
    translate([0, 0, -HUB_L/2 + 2])
    rotate([90, 0, 0])
    translate([0, 0, -HUB_D/2 - 1.2])
    linear_extrude(1.5)
        text("WIND CW FROM CENTER", size=4.5, halign="center", valign="center",
             font="Liberation Sans:style=Bold");
}

module coupling_label() {
    translate([0, 0, HUB_L/2 - 8])
    rotate([90, 0, 0])
    translate([0, 0, -HUB_D/2 - 1.2])
    linear_extrude(1.5)
        text("COUPLING LOOP LAST", size=4.0, halign="center", valign="center",
             font="Liberation Sans:style=Bold");
}

// ---- Full assembly ----
// Hub
color("Ivory", 0.9) hub();

// Bottom flange at -HUB_L/2 - FLANGE_T
translate([0, 0, -HUB_L/2 - FLANGE_T])
    color("DodgerBlue", 0.85) flange_plain();

// Top flange with trap clips
translate([0, 0, HUB_L/2])
    color("DodgerBlue", 0.85) flange_with_clips();

// Labels (raised text on hub exterior)
color("Gold")
    hub_label();
color("Gold")
    coupling_label();

// ---- Notes ----
// PRINT INSTRUCTIONS:
// - Print hub+flanges as single body (no supports needed if hub vertical).
// - Alternatively, print hub and flanges separately and glue.
// - 6 trap clips on top flange face: T50 toroid + cap assembly drops into each well.
// - Center bore (8mm): accepts M5 bolt or 8mm hanging peg for vertical deployment.
// - Carry strap slot (24x12mm): fits 1" webbing.
// - Wind 50m of #14 AWG wire between flanges; coupling loop on outermost layer.
// - Trap assemblies clip into the 6 cradles after winding.
