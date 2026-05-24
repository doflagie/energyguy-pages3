/**
 * cap_mounting_bracket.scad — TM-VCAP-001 Rev A
 * Variable Capacitor Mounting Bracket and Frame Support
 *
 * Accommodates standard amateur radio variable capacitors:
 *   - Frame width: 60–100mm (adjustable)
 *   - Shaft height: 30–50mm from base
 *   - Shaft diameter: 6.35mm (1/4 inch) standard
 *
 * Print settings:
 *   Material:    PETG
 *   Infill:      50% gyroid
 *   Wall:        4 perimeters
 *   Layer:       0.2mm
 *   Orientation: Base flat on bed
 *   Supports:    Yes under shaft collar
 *
 * Print envelope: 120mm × 80mm × 60mm
 *
 * HIGH VOLTAGE NOTE: This bracket contacts the capacitor frame.
 * If the capacitor frame is at RF potential (floating),
 * use nylon M4 standoffs to isolate bracket from chassis.
 */

// ─── Parameters ───────────────────────────────────────────────────────────────

// Capacitor frame dimensions (measure your specific capacitor)
frame_w         = 80.0;    // Frame width (X axis)
frame_d         = 50.0;    // Frame depth (Y axis)
frame_h         = 40.0;    // Frame height from base to shaft centerline
frame_bolt_sp_x = 65.0;    // Bolt hole spacing X (frame mounting bolts)
frame_bolt_sp_y = 40.0;    // Bolt hole spacing Y
frame_bolt_dia  = 4.3;     // M4 clearance hole (4.0mm + 0.3mm)
frame_bolt_head = 8.0;     // M4 head diameter
frame_bolt_hh   = 4.0;     // M4 head height (counterbore)

// Shaft dimensions
shaft_dia       = 6.35;    // 1/4 inch standard
shaft_h         = 40.0;    // Shaft centerline height from bracket base
shaft_bore_dia  = shaft_dia + 1.0;  // Clearance in shaft collar

// Base plate
base_w          = frame_w + 20;
base_d          = frame_d + 20;
base_thick      = 6.0;

// Upright walls
wall_thick      = 5.0;
wall_h          = frame_h + 10;

// Chassis mounting holes (4× M4, for mounting bracket to enclosure floor)
chassis_bolt_sp_x = base_w - 16;
chassis_bolt_sp_y = base_d - 16;
chassis_bolt_dia  = 4.3;

// Vibration isolation (rubber grommet seats)
iso_od          = 12.0;    // Grommet OD
iso_id          = 4.5;     // Grommet ID
iso_depth       = 4.0;     // Counterbore depth for grommet

// Anti-rotation tab
tab_w           = 10.0;
tab_h           = 8.0;
tab_d           = 4.0;

// ─── Modules ──────────────────────────────────────────────────────────────────

module base_plate() {
    difference() {
        // Main base
        hull() {
            for (x=[-1,1]) for (y=[-1,1])
                translate([x*(base_w/2-8), y*(base_d/2-8), 0])
                    cylinder(h=base_thick, r=8, $fn=32);
        }
        // Chassis mounting holes with grommet counterbores
        for (x=[-1,1]) for (y=[-1,1]) {
            translate([x*chassis_bolt_sp_x/2, y*chassis_bolt_sp_y/2, -0.5]) {
                // Grommet seat (from below)
                cylinder(h=iso_depth+0.5, d=iso_od, $fn=32);
                // Through bolt hole
                cylinder(h=base_thick+1, d=chassis_bolt_dia, $fn=16);
            }
        }
        // Weight relief pocket (bottom center)
        translate([0, 0, 2])
            hull() {
                for (x=[-1,1]) for (y=[-1,1])
                    translate([x*(base_w/2-20), y*(base_d/2-20), 0])
                        cylinder(h=base_thick, r=6, $fn=16);
            }
    }
}

module side_walls() {
    wall_offset_x = frame_w/2 + 2;
    // Two side walls supporting the capacitor frame
    for (side=[-1,1]) {
        translate([side*wall_offset_x, 0, base_thick]) {
            difference() {
                // Wall body
                hull() {
                    translate([0, -frame_d/2, 0])
                        cube([wall_thick, frame_d, wall_h], center=false);
                }
                // Frame bolt holes (counterbored)
                for (y=[-1,1]) {
                    translate([-0.5, y*frame_bolt_sp_y/2, frame_h])
                    rotate([0,90,0]) {
                        cylinder(h=wall_thick+1, d=frame_bolt_dia, $fn=16);
                        cylinder(h=frame_bolt_hh+0.5, d=frame_bolt_head+1, $fn=16);
                    }
                }
            }
        }
    }
}

module shaft_collar() {
    // Raised collar at shaft height to prevent lateral shaft flex
    translate([0, -frame_d/2 - wall_thick, base_thick + frame_h])
    difference() {
        cylinder(h=wall_thick+4, d=shaft_bore_dia+12, $fn=32);
        translate([0,0,-0.5])
            cylinder(h=wall_thick+5, d=shaft_bore_dia, $fn=32);
    }
}

module anti_rotation_tab() {
    // Prevents capacitor frame from rotating about shaft axis
    translate([-tab_w/2, frame_d/2, base_thick + 5])
        cube([tab_w, tab_d, tab_h]);
    // Slot in tab for set screw
    translate([0, frame_d/2 + tab_d/2, base_thick + 5 + tab_h/2])
    rotate([90,0,0])
        cylinder(h=tab_d+1, d=3.3, $fn=16, center=true);
}

module cable_guide() {
    // Route flexible wiper wire away from RF field
    // Clip on back wall
    translate([base_w/2 - 15, -base_d/2, base_thick + 20])
    difference() {
        cube([12, 8, 14], center=false);
        translate([6, -0.5, 7])
        rotate([-90,0,0])
            cylinder(h=9, d=5, $fn=16);
        translate([6, 3, 7])
        rotate([-90,0,0])
            cylinder(h=5, d=3, $fn=16);
    }
}

// ─── Full Assembly ─────────────────────────────────────────────────────────────

module cap_mounting_bracket() {
    union() {
        base_plate();
        side_walls();
        shaft_collar();
        anti_rotation_tab();
        cable_guide();
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 32;
cap_mounting_bracket();

// ─── Notes ────────────────────────────────────────────────────────────────────
// Assembly:
// 1. Insert M4 nylon grommets (McMaster 90131A151) into chassis bolt counterbores
// 2. Mount bracket to enclosure floor with M4×12 screws through grommets
// 3. Set capacitor frame on side wall ledges, secure with M4×8 screws
// 4. Route wiper wire through cable guide to RF terminal
// 5. If frame is at RF potential, use M4 nylon insulating washers at ALL mounting points
