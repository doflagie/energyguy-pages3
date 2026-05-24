// ============================================================
// CANTENNA DUMMY LOAD — LID, BASE ADAPTER, CONNECTOR MOUNT
// For 500W oil-cooled dummy load using 1-gallon paint can
//
// NOTE: The can body itself is a standard 1-gallon steel paint can
//       (153mm OD, 285mm tall). This file produces:
//         1. Base plate with N-connector mount + ground ring
//         2. Sealing lid with fill port + temp probe port + pressure relief
//         3. Connector standoff/insulator
//
// Print settings: ASA ONLY (mineral oil resistance required)
//   Infill: 40%, walls: 4 perimeters
//   Post-process: coat interior with epoxy or mineral-oil-safe sealant
//
// Hardware: 1-gallon paint can (153mm x 285mm), all steel
//           N-type panel mount connector (bottom center)
//           NTC thermistor probe (lid, 150mm insertion)
//           1/8" NPT pressure relief (lid)
// ============================================================

/* [Can Dimensions] */
can_od    = 153.0;   // 1-gallon paint can OD
can_id    = 149.0;   // can ID
can_h     = 285.0;   // can body height

/* [Plate Parameters] */
plate_t   = 6.0;     // base/lid plate thickness
lip_h     = 15.0;    // engagement lip height (fits inside can top/bottom)
lip_t     = 2.5;     // lip wall thickness

/* [Connector Parameters] */
// N-type panel mount hole diameter
n_hole_d  = 21.0;
// N-type mounting bolt pattern (4x #4-40, 25.4mm diagonal)
n_bolt_r  = 12.7;   // radius from center
n_bolt_d  = 2.9;    // #4-40 clearance

/* [Probe / Fill Ports] */
// Temperature probe: 1/4"-28 tapped hole, 50mm from center
probe_offset = 50.0;
probe_d      = 7.0;  // 1/4" drill

// Fill / check valve: 3/8" NPT tap, 40mm from center other side
fill_offset  = 40.0;
fill_d       = 10.0; // 3/8" drill

// Pressure relief: 1/8" NPT, 30mm from center
relief_offset = 30.0;
relief_d      = 8.6;  // 1/8" NPT tap drill

$fn = 96;

// ============================================================
// Engagement lip (fits inside can)
// ============================================================
module can_lip() {
    difference() {
        cylinder(d=can_id - 0.5, h=lip_h);
        translate([0, 0, -0.01])
            cylinder(d=can_id - 0.5 - 2*lip_t, h=lip_h + 0.02);
    }
}

// ============================================================
// Base plate (bottom of can, N-connector mount)
// ============================================================
module base_plate() {
    union() {
        difference() {
            // Main plate disc
            cylinder(d=can_od + 4, h=plate_t);
            // N-connector hole (center)
            translate([0, 0, -0.01])
                cylinder(d=n_hole_d, h=plate_t + 0.02);
            // N-connector bolt holes (4x)
            for (a=[45, 135, 225, 315]) {
                translate([n_bolt_r*cos(a), n_bolt_r*sin(a), -0.01])
                    cylinder(d=n_bolt_d, h=plate_t + 0.02);
            }
            // Coax ground strap notch (for braid connection)
            translate([25, 0, -0.01])
                cube([10, 5, plate_t + 0.02], center=true);
        }
        // Engagement lip (protrudes up into can)
        translate([0, 0, plate_t])
            can_lip();
        // Mounting feet (3x, 120-deg)
        for (a=[0, 120, 240]) {
            translate([(can_od/2 + 8)*cos(a), (can_od/2 + 8)*sin(a), 0])
                difference() {
                    cylinder(d=16, h=plate_t + 3);
                    translate([0, 0, -0.01]) cylinder(d=4.4, h=plate_t + 4);
                }
        }
    }
}

// ============================================================
// Sealing lid (top of can)
// ============================================================
module sealing_lid() {
    difference() {
        union() {
            // Main lid disc
            cylinder(d=can_od + 4, h=plate_t);
            // Engagement lip (protrudes down into can)
            translate([0, 0, -lip_h])
                can_lip();
            // Fill port boss
            translate([fill_offset, 0, plate_t])
                cylinder(d=fill_d + 8, h=10);
            // Temp probe boss
            translate([-probe_offset, 0, plate_t])
                cylinder(d=probe_d + 8, h=10);
            // Pressure relief boss
            translate([0, relief_offset, plate_t])
                cylinder(d=relief_d + 8, h=8);
            // Handle bosses
            for (x=[-40, 40]) {
                translate([x, can_od/2 - 15, plate_t])
                    cube([20, 20, 12], center=true);
            }
        }
        // Fill port hole (3/8" NPT tap = 10mm drill)
        translate([fill_offset, 0, -0.01])
            cylinder(d=fill_d, h=plate_t + 12);
        // Temp probe hole (1/4"-28 tap = 7mm drill)
        translate([-probe_offset, 0, -0.01])
            cylinder(d=probe_d, h=plate_t + 12);
        // Pressure relief hole (1/8" NPT = 8.6mm drill)
        translate([0, relief_offset, -0.01])
            cylinder(d=relief_d, h=plate_t + 10);
        // Handle cutouts
        for (x=[-40, 40]) {
            translate([x, can_od/2 - 15, plate_t + 1])
                cube([12, 22, 12], center=true);
        }
        // Gasket groove on bottom face of lid
        translate([0, 0, -0.01])
            difference() {
                cylinder(d=can_id - 4, h=2.5);
                translate([0, 0, -0.01])
                    cylinder(d=can_id - 10, h=2.7);
            }
    }
}

// ============================================================
// Connector standoff insulator (PTFE-equivalent in plastic)
// Isolates N-connector center pin from base plate
// ============================================================
module connector_standoff() {
    // PTFE or Delrin insulating disc
    // 25mm OD, 4mm thick, 5mm center bore for stud
    // Specify: cut this from PTFE rod stock, DO NOT print
    // (PTFE has superior dielectric properties at RF)
    // This module is for reference dimensions only.
    color("White", 0.8)
    difference() {
        cylinder(d=25, h=4);
        translate([0,0,-0.01]) cylinder(d=5.2, h=4.02);
        for (a=[45,135,225,315]) {
            translate([n_bolt_r*cos(a), n_bolt_r*sin(a), -0.01])
                cylinder(d=n_bolt_d, h=4.02);
        }
    }
}

// ============================================================
// Render parts (spread out for printing)
// ============================================================
// Base plate
translate([0, 0, 0]) base_plate();

// Lid (offset for print preview)
translate([can_od + 20, 0, 0]) sealing_lid();

// Standoff insulator (cut from PTFE in practice)
translate([can_od + 20, can_od + 20, 0]) connector_standoff();

// ============================================================
// Labels for each part
// ============================================================
echo("=== CANTENNA DUMMY LOAD ===");
echo(str("Can OD: ", can_od, "mm  Can H: ", can_h, "mm"));
echo("Part 1: base_plate — print 1x in ASA");
echo("Part 2: sealing_lid — print 1x in ASA");
echo("Part 3: connector_standoff — CUT FROM PTFE ROD, 25mm OD x 4mm");
echo("Can body: standard 1-gallon steel paint can (purchased)");
echo("Oil: 3.5 liters naphthenic mineral oil (transformer grade)");
echo(str("Base plate fits 200mm bed: ", can_od + 4 <= 195 ? "YES" : "MARGINALLY"));
echo("NOTE: Seal all ASA printed parts with mineral-oil-safe epoxy");
