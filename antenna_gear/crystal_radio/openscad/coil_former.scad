/*
 * coil_former.scad
 * Crystal Radio Coil Forms and Ferrite Rod Hardware
 * TM-CR-001 Rev A
 * Material: PETG (dimensional stability; PLA may warp near warm circuits)
 * Print bed: 200 × 200 mm
 *
 * Parts:
 *   1. Ferrite rod sleeve — slides on 9.5 mm rod; holds winding
 *   2. Coupling slider — adjustable secondary position on rod
 *   3. Air-core form — 160M coil, 50mm diameter
 *   4. Rod end cap — holds rod in enclosure
 *   5. Coil spacer — maintains separation between coils on rod
 *   6. Winding shuttle — assists winding fine-wire coils on core
 */

$fn = 48;

// ── Global parameters ───────────────────────────────────────────────────────
ROD_D        = 9.5;   // Ferrite rod OD (nominal 9.5 mm; measure actual rod)
ROD_FIT      = 0.3;   // Clearance for sliding fit on rod
WALL         = 1.6;   // Shell wall thickness
COIL_L       = 55;    // Active winding length (110 turns, close-wound #28 AWG)
FLANGE_H     = 3;     // End flange height
FLANGE_OD    = ROD_D + 2*WALL + 6; // Outer diameter of flanges

// ── Utility ─────────────────────────────────────────────────────────────────
module sleeve_blank(inner_d, outer_d, length) {
    difference() {
        cylinder(d=outer_d, h=length);
        translate([0,0,-0.1]) cylinder(d=inner_d, h=length+0.2);
    }
}

// ── 1. Ferrite Rod Coil Sleeve ───────────────────────────────────────────────
/*
 * Sliding coil form for 110-turn main winding.
 * Fits over 9.5 mm ferrite rod with 0.3 mm running clearance.
 * Split lengthwise for installation (print in two halves, snap-fit).
 * Flanges prevent coil from shifting.
 * Wire exit slots at each end for winding leads and taps.
 */

SLEEVE_ID    = ROD_D + ROD_FIT;       // 9.8 mm
SLEEVE_OD    = SLEEVE_ID + 2*WALL;    // 13.0 mm

module coil_sleeve_half() {
    difference() {
        union() {
            // Main sleeve body
            sleeve_blank(SLEEVE_ID, SLEEVE_OD, COIL_L);
            // Flanges
            translate([0,0,-FLANGE_H])
                cylinder(d=FLANGE_OD, h=FLANGE_H);
            translate([0,0,COIL_L])
                cylinder(d=FLANGE_OD, h=FLANGE_H);
            // Alignment ridges (pair, for two-half assembly)
            translate([SLEEVE_OD/2-0.1, 0, COIL_L/2])
                cube([0.8, SLEEVE_OD+2, COIL_L-4], center=true);
        }
        // Split cut — lower half only
        translate([-FLANGE_OD/2-0.1, -FLANGE_OD-0.1, -FLANGE_H-0.1])
            cube([FLANGE_OD+0.2, FLANGE_OD/2+0.1, COIL_L+2*FLANGE_H+0.2]);
        // Wire exit slots (top flange: 3 slots for tap wires)
        for (i=[-1, 0, 1])
            translate([i*4, 0, COIL_L-0.1])
                cube([1.5, SLEEVE_OD+1, FLANGE_H+0.2], center=true);
        // Wire exit slots (bottom flange)
        translate([0, 0, -FLANGE_H-0.1])
            cube([1.5, SLEEVE_OD+1, FLANGE_H+0.2], center=true);
        // Snap groove on alignment ridge for mating half
        translate([SLEEVE_OD/2+0.1, 0, COIL_L/2])
            cube([1.2, 1.2, COIL_L-8], center=true);
    }
}

// Print two instances side by side:
// coil_sleeve_half();
// translate([FLANGE_OD+3, 0, 0]) coil_sleeve_half();


// ── 2. Coupling Slider (adjustable secondary position) ───────────────────────
/*
 * Shorter sleeve for secondary/tickler coil; slides along rod.
 * Set screw M2 (or friction fit) holds position.
 * Used in regenerative and double-tuned designs to adjust coupling.
 * Scale marks on outer surface (1 mm increments) for repeatable positioning.
 */

SLIDER_L  = 25;  // 20-turn secondary winding length
SLIDER_ID = SLEEVE_ID;
SLIDER_OD = SLEEVE_OD;

module coupling_slider() {
    difference() {
        union() {
            sleeve_blank(SLIDER_ID, SLIDER_OD, SLIDER_L);
            // Small flanges
            translate([0,0,-2]) cylinder(d=FLANGE_OD-2, h=2);
            translate([0,0,SLIDER_L]) cylinder(d=FLANGE_OD-2, h=2);
            // Position indicator fin (1 mm wide, 4 mm tall)
            translate([SLIDER_OD/2-0.1, 0, SLIDER_L/2])
                cube([4, 1, SLIDER_L], center=true);
        }
        // Set screw boss hole (M2, perpendicular to rod)
        translate([0, 0, SLIDER_L/2])
            rotate([90,0,0])
            cylinder(d=2.2, h=SLIDER_OD+0.2, center=true);
        // Wire exit slot
        translate([0, 0, SLIDER_L-0.1])
            cube([1.5, SLIDER_OD+1, 3], center=true);
        translate([0, 0, -2-0.1])
            cube([1.5, SLIDER_OD+1, 3], center=true);
    }
}

// ── 3. Air-Core Form (160M coil, 50mm diameter) ─────────────────────────────
/*
 * Hollow cylinder for 160M air-core coil.
 * 30 turns #24 AWG → L ≈ 78 µH at 50 mm diameter, 50 mm long.
 * No ferrite; used for 1.8–2.0 MHz where ferrite losses increase.
 * Print vertically; two half-flanges allow post-print wire installation.
 * Mounting: screw to baseboard through center tube.
 */

AIRCORE_D   = 50;   // Coil form diameter
AIRCORE_L   = 52;   // Winding length (30 turns, 1.6 mm/turn + flange)
AIRCORE_WALL = 2;
AIRCORE_FLANGE_OD = AIRCORE_D + 8;

module aircore_form() {
    difference() {
        union() {
            // Main cylinder
            sleeve_blank(AIRCORE_D, AIRCORE_D+2*AIRCORE_WALL, AIRCORE_L);
            // Flanges
            translate([0,0,-FLANGE_H])
                cylinder(d=AIRCORE_FLANGE_OD, h=FLANGE_H);
            translate([0,0,AIRCORE_L])
                cylinder(d=AIRCORE_FLANGE_OD, h=FLANGE_H);
        }
        // Mounting hole through center
        translate([0,0,-FLANGE_H-0.1])
            cylinder(d=5, h=AIRCORE_L+2*FLANGE_H+0.2);
        // Wire grooves (start/end + tap)
        for (angle=[0, 120, 240])
            rotate([0,0,angle])
            translate([AIRCORE_D/2+AIRCORE_WALL/2, 0, AIRCORE_L+0.1])
                cylinder(d=2.5, h=FLANGE_H+0.2);
        // Wire routing channel along flange face
        translate([0,0,-FLANGE_H-0.1])
            rotate_extrude(angle=360)
            translate([(AIRCORE_D+AIRCORE_WALL)/2, 0, 0])
            square([AIRCORE_WALL+0.2, 1.8], center=true);
    }
    // Winding guide ring (optional; improves uniform spacing)
    // Each ring marks one turn position for 1.6 mm pitch wire
    for (i=[0:29])
        translate([0,0,1 + i*1.6])
        difference() {
            cylinder(d=AIRCORE_D+2*AIRCORE_WALL+0.8, h=0.4);
            translate([0,0,-0.1]) cylinder(d=AIRCORE_D+2*AIRCORE_WALL-0.4, h=0.6);
        }
}

// ── 4. Rod End Cap ────────────────────────────────────────────────────────────
/*
 * Snaps onto end of ferrite rod; positions rod within enclosure.
 * Two versions: open-end (for use with C1 shaft side) and
 *               closed-end (for magnetic-quiet end of rod).
 * O-ring groove for rod retention without adhesive.
 */

CAP_L  = 12;
CAP_ID = ROD_D + 0.2;  // Slightly tighter; press fit

module rod_end_cap(closed=false) {
    difference() {
        union() {
            cylinder(d=CAP_ID+4, h=CAP_L);
            // Mounting flange
            translate([0,0,CAP_L-1]) cylinder(d=CAP_ID+12, h=1);
        }
        // Rod channel
        translate([0,0,-0.1]) cylinder(d=CAP_ID, h=closed ? CAP_L-2 : CAP_L+0.2);
        // O-ring groove (1.5 mm wide × 1.2 mm deep)
        translate([0,0,4])
            rotate_extrude(angle=360)
            translate([CAP_ID/2+1, 0, 0])
            square([2, 1.5], center=true);
        // Screw holes in flange for panel mounting M2
        for (a=[0,120,240])
            rotate([0,0,a])
            translate([CAP_ID/2+5, 0, CAP_L-0.1])
            cylinder(d=2.2, h=1.2);
    }
}

// ── 5. Coil Spacer ────────────────────────────────────────────────────────────
/*
 * Thin ring that slides on rod between coils.
 * Maintains fixed separation between L1, L2 (or L1, L2, L3 in TRF).
 * Available in 10, 15, 20, 25 mm widths.
 * Width = gap between coil forms, which determines coupling k.
 */

module coil_spacer(width=20) {
    difference() {
        cylinder(d=SLEEVE_OD+4, h=width);
        translate([0,0,-0.1]) cylinder(d=SLEEVE_ID, h=width+0.2);
        // Marking cuts for identification (one cut = 10mm, two cuts = 20mm, etc.)
        count = round(width/10);
        for (i=[0:count-1])
            translate([0, SLEEVE_OD/2+1.5, i*3+3])
            rotate([90,0,0]) cylinder(d=1.2, h=SLEEVE_OD+4);
    }
}

// ── 6. Winding Shuttle ────────────────────────────────────────────────────────
/*
 * Flat spool used to hold fine wire during winding.
 * Allows controlled feeding of #36–#28 AWG wire onto toroid or coil form.
 * Load wire by wrapping around shuttle; unwind as needed.
 * Thin profile fits through toroid center hole.
 */

SHUTTLE_L     = 55;
SHUTTLE_W     = 8;
SHUTTLE_T     = 2;
SHUTTLE_HOLE  = 10;  // Passes through T-94A-77 center (ID=14mm; this clears it)

module winding_shuttle() {
    difference() {
        union() {
            // Central flat body
            cube([SHUTTLE_L, SHUTTLE_W, SHUTTLE_T], center=true);
            // End retainer flanges
            for (x=[-SHUTTLE_L/2+2, SHUTTLE_L/2-2])
                translate([x, 0, 0])
                cube([4, SHUTTLE_W+4, SHUTTLE_T], center=true);
        }
        // Wire notch on each end (thread wire out of shuttle)
        for (x=[-SHUTTLE_L/2, SHUTTLE_L/2])
            translate([x, 0, 0])
            cylinder(d=1.8, h=SHUTTLE_T+0.2, center=true);
    }
}


// ── Assembly View / Render ────────────────────────────────────────────────────
// Uncomment to render individual parts for printing:

// MAIN COIL SLEEVE (print ×2, snap together on rod):
coil_sleeve_half();
translate([FLANGE_OD+4, 0, 0]) coil_sleeve_half();

// COUPLING SLIDER:
// translate([0, FLANGE_OD+4, 0]) coupling_slider();

// AIR-CORE FORM (160M):
// translate([0, AIRCORE_FLANGE_OD+4, 0]) aircore_form();

// ROD END CAPS (×2 per radio):
// translate([0, CAP_ID+20, 0]) rod_end_cap(closed=false);
// translate([25, CAP_ID+20, 0]) rod_end_cap(closed=true);

// COIL SPACERS (print multiple widths):
// translate([0, 0, 50]) coil_spacer(10);
// translate([30, 0, 50]) coil_spacer(20);
// translate([60, 0, 50]) coil_spacer(25);

// WINDING SHUTTLE:
// translate([0, SHUTTLE_W+20, 0]) winding_shuttle();
