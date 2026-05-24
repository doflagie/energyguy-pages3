// ============================================================
//  yagi_element_clip.scad — Yagi Element Clip for Pool Noodle Boom
//  TM-PNL2-ENC-004 Rev A
//  Clips wire Yagi elements (perpendicular) to pool noodle boom.
//  Snap-fit: no tools required for field assembly / disassembly.
//  Parametric: adjust ELEMENT_WIRE_D for #14, #16, #18 AWG.
//  Print bed: 200×200×200mm  (print 10–20 per session)
// ============================================================

// ---- Parameters ----
BOOM_OD        = 47.0;  // pool noodle boom OD (mm)
BOOM_CLR       = 1.2;   // snap-fit clearance (mm)
ELEMENT_WIRE_D_14 = 1.70;  // #14 AWG wire diameter (mm) + clearance
ELEMENT_WIRE_D_16 = 1.40;  // #16 AWG
ELEMENT_WIRE_D_18 = 1.10;  // #18 AWG
ELEMENT_WIRE_D  = ELEMENT_WIRE_D_14;  // select gauge here

BODY_H         = 22.0;  // clip body height (axis of element)
BODY_W         = BOOM_OD + BOOM_CLR * 2 + 10;  // clip width
WALL           = 2.8;   // minimum wall thickness
SNAP_W         = 8.0;   // snap-arm width
SNAP_DEPTH     = 2.5;   // snap tooth engagement depth
SNAP_T         = 1.8;   // snap arm thickness
WIRE_GROOVE_D  = 1.2;   // depth of wire-capture groove
SPACING_MARK_W = 1.0;   // reference mark groove width
N_ARRAY        = 10;    // number to print in array

$fn = 64;

BOOM_ID = BOOM_OD + BOOM_CLR * 2;

module single_clip() {
    difference() {
        union() {
            // Main body
            hull() {
                cylinder(d = BOOM_ID + WALL * 2, h = BODY_H, center = true);
                translate([0, (BOOM_ID + WALL * 2)/2, 0])
                    cube([BODY_W - 4, 4, BODY_H], center = true);
            }
            // Snap arms — extend down, hook under boom
            for (side = [-1, 1])
                translate([side * (BOOM_ID/2 + WALL/2 - 0.5), -2, 0])
                    difference() {
                        cube([SNAP_T, SNAP_W, BODY_H + SNAP_DEPTH * 2], center = true);
                        // Snap tooth cutout
                        translate([side * SNAP_DEPTH/2, 0, -(BODY_H/2 + SNAP_DEPTH)])
                            cube([SNAP_DEPTH + 0.5, SNAP_W + 0.1, SNAP_DEPTH * 2], center = true);
                    }
        }

        // Boom bore (partial — open at one side for snap-on)
        cylinder(d = BOOM_ID, h = BODY_H + 0.2, center = true);

        // Boom opening slot (snap-on access)
        translate([0, -(BOOM_ID/2 + WALL/2), 0])
            cube([BOOM_ID - SNAP_DEPTH * 4, WALL + 2, BODY_H + 0.2], center = true);

        // Element wire groove (through-hole, perpendicular to boom axis)
        rotate([90, 0, 0])
            cylinder(d = ELEMENT_WIRE_D, h = BODY_W + 4, center = true, $fn = 16);

        // Wire groove capture lip (prevents wire from lifting)
        rotate([90, 0, 0])
            cylinder(d = ELEMENT_WIRE_D + WIRE_GROOVE_D * 2, h = WALL, center = true, $fn = 16);

        // Center reference groove (aligns element at boom centerline)
        translate([0, BOOM_ID/2 + WALL, 0])
            cube([SPACING_MARK_W, 1.5, BODY_H + 0.2], center = true);

        // Label
        translate([0, BOOM_ID/2 + WALL + 1, BODY_H/2 - 1])
            rotate([90, 0, 0])
                linear_extrude(1)
                    text("TM-PNL2-ENC-004", size = 2.5, halign = "center",
                         font = "Liberation Sans");
    }
}

// ---- Print array ----
// Print N_ARRAY clips in a row
for (i = [0 : N_ARRAY - 1]) {
    translate([(BODY_W + 3) * i, 0, BODY_H/2])
        single_clip();
}

// ---- Notes ----
// To assemble:
//   a. Thread element wire through wire groove in clip before mounting.
//   b. Snap clip onto pool noodle boom (boom snaps into bore).
//   c. Slide clip to correct spacing position.
//   d. Element wire is captured by groove; no separate fastener needed.
//   e. For permanent installs: add drop of cyanoacrylate to wire groove.
//
// Element spacing accuracy:
//   Print a spacing jig or mark boom with permanent marker at each
//   element position from the calculations CSV.
//
// Wire gauge:
//   Change ELEMENT_WIRE_D parameter to match wire gauge being used:
//   #14 AWG = 1.70mm, #16 AWG = 1.40mm, #18 AWG = 1.10mm
//
// Boom clearance:
//   BOOM_CLR = 1.2mm allows for pool noodle OD variation.
//   If fit is too loose: reduce BOOM_CLR to 0.8mm.
//   If clip will not snap on: increase SNAP_DEPTH by 0.5mm.
