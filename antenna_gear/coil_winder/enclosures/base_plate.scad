/**
 * base_plate.scad — TM-CWND-001 Rev A
 * Coil Winder Machine Base — Two-Section Design
 *
 * The complete machine bed is 400mm × 180mm, printed as two 200×180mm
 * sections joined by M5 alignment pins and M5 bolts through splice plates.
 *
 * Section A (print_section="A"): Contains spindle headstock mounting boss
 * Section B (print_section="B"): Contains traverse drive (T8 leadscrew) end
 * Splice plate:                  Joins A to B with 4× M5 bolts
 *
 * Print settings:
 *   Material:  PETG or PLA+
 *   Infill:    40% gyroid
 *   Wall:      4 perimeters
 *   Layer:     0.2mm
 *   Supports:  Yes under rail rod bosses
 *
 * Print envelope: 200×180×25mm per section (fits 200mm cube)
 *
 * Assembly note:
 *   Rail rods: 2× 12mm OD smooth rod, 400mm long, center spacing 60mm
 *   T8 leadscrew: 8mm, 400mm long, 2mm pitch (T8 standard)
 *   All linear rails use LM12UU linear bearings or printed bushings
 */

// ─── Parameters ───────────────────────────────────────────────────────────────

section_len     = 198.0;   // Length of each section (2mm gap at joint)
base_w          = 180.0;   // Width
base_thick      = 22.0;    // Thickness
corner_r        = 8.0;

// Rail rod geometry
rail_spacing    = 60.0;    // Center-to-center Y spacing
rail_dia        = 12.0;    // Rod diameter (12mm smooth rod)
rail_boss_od    = 22.0;    // Support boss OD
rail_boss_h     = 12.0;    // Boss height above base
rail_y1         = base_w/2 - rail_spacing/2;  // Front rail Y
rail_y2         = base_w/2 + rail_spacing/2;  // Rear rail Y

// T8 leadscrew channel
lead_y          = base_w/2;           // Center of base
lead_dia        = 8.4;                // 8mm + clearance
lead_boss_od    = 18.0;
lead_z_center   = base_thick + 8.0;   // Leadscrew axis height above base top

// Headstock end (section A, x=0 end)
head_boss_od    = 50.0;    // Large bearing housing boss
head_boss_h     = 20.0;    // Boss height
head_bore       = 14.0;    // 6004ZZ bearing OD = 42mm; use commercial flange bearing
                            // Alternatively: 12mm shaft bore + 32mm OD 6001ZZ
spindle_bore    = 32.4;    // 32mm OD bearing bore (6001ZZ or 6004ZZ outer)
spindle_y       = base_w / 2;
spindle_x       = 25.0;    // Headstock boss center from section A end

// Tailstock end (section B, support for leadscrew + rail)
tail_x          = section_len - 20;  // From section B start

// Motor mount (NEMA 23 or NEMA 17 on section B far end)
nema23_bolt_sp  = 47.14;   // NEMA 23: 47.14mm bolt pattern
nema17_bolt_sp  = 31.0;    // NEMA 17: 31mm bolt pattern
motor_boss_h    = 5.0;
motor_shaft_bore = 7.0;    // NEMA 23 shaft 6.35mm + clearance
motor_pilot_bore = 38.2;   // NEMA 23 pilot diameter

// Chassis mounting feet
foot_dia        = 8.0;
foot_hole       = 4.3;     // M4 clearance
foot_z          = 5.0;     // Rubber foot recess depth

// Alignment pins between sections
pin_dia         = 5.1;     // M5 pin or 5mm dowel pin
pin_depth       = 10.0;

// ─── Modules ──────────────────────────────────────────────────────────────────

module base_shell(len) {
    hull() {
        for (x=[corner_r, len-corner_r])
        for (y=[corner_r, base_w-corner_r])
            translate([x, y, 0])
                cylinder(h=base_thick, r=corner_r, $fn=32);
    }
}

module rail_support(x, y) {
    translate([x, y, base_thick]) {
        difference() {
            cylinder(h=rail_boss_h, d=rail_boss_od, $fn=32);
            translate([0,0,-0.5])
                cylinder(h=rail_boss_h+1, d=rail_dia+0.3, $fn=32);
        }
    }
}

module lead_support(x) {
    translate([x, lead_y, base_thick])
    difference() {
        cylinder(h=lead_z_center, d=lead_boss_od, $fn=32);
        // Leadscrew through-bore
        translate([0,0,lead_z_center-lead_boss_od/2-0.5])
            cylinder(h=lead_boss_od+1, d=lead_dia, $fn=32);
    }
}

module foot(x, y) {
    translate([x, y, 0]) {
        difference() {
            cylinder(h=foot_z+base_thick/2, d=foot_dia+10, $fn=32);
            // Rubber foot recess (from bottom)
            translate([0,0,-0.5])
                cylinder(h=foot_z+0.5, d=foot_dia*1.5, $fn=32);
            // M4 bolt hole through
            translate([0,0,-0.5])
                cylinder(h=foot_z+base_thick/2+1, d=foot_hole, $fn=16);
        }
    }
}

// ─── Section A: Headstock end ─────────────────────────────────────────────────

module section_A() {
    difference() {
        union() {
            base_shell(section_len);
            // Rail supports along length
            for (x=[20, 80, 140, section_len-10])
            for (y=[rail_y1, rail_y2])
                rail_support(x, y);
            // Leadscrew support
            for (x=[30, 100]) lead_support(x);
            // Spindle headstock boss (large)
            translate([spindle_x, spindle_y, base_thick]) {
                difference() {
                    cylinder(h=head_boss_h, d=head_boss_od, $fn=64);
                    translate([0,0,-0.5])
                        cylinder(h=head_boss_h+1, d=spindle_bore, $fn=64);
                }
            }
        }
        // Alignment pin holes at joint end
        for (y=[40, base_w-40])
            translate([section_len-pin_depth-1, y, base_thick/2])
            rotate([0,90,0])
                cylinder(h=pin_depth+1, d=pin_dia, $fn=16);
        // M5 bolt holes (joint)
        for (y=[25, base_w-25])
            translate([section_len-15, y, -0.5])
                cylinder(h=base_thick+1, d=5.3, $fn=16);
        // Feet
        for (x=[20, section_len-20]) for (y=[20, base_w-20])
            translate([x, y, -0.5]) cylinder(h=foot_z+0.5, d=foot_hole, $fn=16);
    }
    // Feet bosses
    for (x=[20, section_len-20]) for (y=[20, base_w-20])
        foot(x, y);
}

// ─── Section B: Tailstock/Motor end ───────────────────────────────────────────

module section_B() {
    difference() {
        union() {
            base_shell(section_len);
            // Rail supports
            for (x=[10, 60, 120, 170])
            for (y=[rail_y1, rail_y2])
                rail_support(x, y);
            // Leadscrew supports
            for (x=[40, 120]) lead_support(x);
            // Motor mount boss (end) — NEMA 23 face mount
            translate([section_len-12, base_w/2, base_thick]) {
                difference() {
                    cylinder(h=motor_boss_h, d=head_boss_od, $fn=64);
                    // Pilot bore
                    translate([0,0,-0.5])
                        cylinder(h=motor_boss_h+1, d=motor_pilot_bore, $fn=64);
                }
            }
        }
        // Alignment pin sockets at joint end (section A end of B)
        for (y=[40, base_w-40])
            translate([0, y, base_thick/2])
            rotate([0,-90,0])
                cylinder(h=pin_depth+2, d=pin_dia, $fn=16);
        // M5 joint bolt clearance holes
        for (y=[25, base_w-25])
            translate([10, y, -0.5])
                cylinder(h=base_thick+1, d=5.3, $fn=16);
        // NEMA 23 motor bolt holes
        for (a=[45,135,225,315])
            rotate([0,0,a])
            translate([nema23_bolt_sp/2, 0, base_thick+motor_boss_h-0.5])
                cylinder(h=motor_boss_h+1, d=5.3, $fn=16);
        // Motor shaft access hole
        translate([section_len-12, base_w/2, -0.5])
            cylinder(h=base_thick+motor_boss_h+1, d=motor_shaft_bore, $fn=32);
        // Feet
        for (x=[20, section_len-20]) for (y=[20, base_w-20])
            translate([x, y, -0.5]) cylinder(h=foot_z+0.5, d=foot_hole, $fn=16);
    }
    for (x=[20, section_len-20]) for (y=[20, base_w-20])
        foot(x, y);
}

// ─── Splice Plate ─────────────────────────────────────────────────────────────

module splice_plate() {
    // 50×180×10mm plate, bolts over joint seam
    difference() {
        hull() {
            for (x=[5, 45]) for (y=[5, base_w-5])
                translate([x, y, 0]) cylinder(h=10, r=5, $fn=16);
        }
        // 4× M5 through holes
        for (x=[10, 40]) for (y=[25, base_w-25])
            translate([x, y, -0.5]) cylinder(h=11, d=5.3, $fn=16);
        // Center relief
        translate([5, 20, 3])
            cube([40, base_w-40, 10]);
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 32;
print_section = "A";  // Change to "B" or "SPLICE" for other parts

if (print_section == "A") {
    color("lightgray") section_A();
} else if (print_section == "B") {
    color("lightgray")
    translate([section_len + 5, 0, 0]) section_B();
} else if (print_section == "SPLICE") {
    color("orange") splice_plate();
} else {
    // Show assembled view
    color("lightgray", 0.8) section_A();
    color("lightblue", 0.8) translate([section_len + 2, 0, 0]) section_B();
    color("orange") translate([section_len - 25, 0, base_thick])
        splice_plate();
}
