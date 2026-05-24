/**
 * bird_element_hf.scad — TM-PWR-ENCL-EL-001 Rev A
 * HF / VHF plug-in slug element body
 * 25.0mm OD × 25.4mm, D-flat orientation key, 2mm banana pin
 * Fits Bird Model 43 compatible wattmeter body
 */

// ─── Parameters ──────────────────────────────────────────────────────────────
slug_od      = 25.0;   // Outer diameter (0.2mm under body slot for clearance)
slug_l       = 25.4;   // Length (1.000")
slug_flat    = 1.8;    // Flat depth (from OD surface, creates D-shape)

center_bore  = 5.2;    // Center conductor bore (slightly over 5mm for clearance)
pcb_w        = 21.5;   // PCB slot width
pcb_d        = 18.0;   // PCB slot depth
pcb_t        = 1.8;    // PCB slot thickness (1.6mm PCB + 0.2mm clearance)

banana_d     = 2.2;    // 2mm banana pin hole
banana_x     = slug_l / 2;  // Centered along length
banana_depth = 8;       // Depth of banana pin well

oring_d      = 25.0;   // O-ring groove diameter
oring_w      = 2.5;    // O-ring groove width
oring_depth  = 1.5;    // O-ring groove depth (for 2mm O-ring)
oring_pos    = slug_l / 2;  // O-ring at slug midpoint

// PCB access slot (open bottom for component access during assembly)
slot_w       = 16;
slot_h       = 12;
slot_z       = 2;     // From bottom end of slug

// Label recess (on flat face)
label_w      = 18;
label_h      = 8;
label_depth  = 0.4;   // Engraved text depth

// ─── Modules ──────────────────────────────────────────────────────────────────

module slug_body_profile() {
    // D-shape cross-section: circle with one flat
    difference() {
        circle(d=slug_od, $fn=80);
        translate([-(slug_od), slug_od/2 - slug_flat])
            square([slug_od * 2, slug_od]);
    }
}

module oring_groove() {
    rotate_extrude($fn=80)
        translate([oring_d/2 - oring_depth, 0, 0])
            square([oring_depth + 0.1, oring_w], center=true);
}

// ─── Main Slug Body ───────────────────────────────────────────────────────────
module slug() {
    difference() {
        // D-shaped extruded body
        linear_extrude(slug_l)
            slug_body_profile();

        // Center conductor bore (RF through-hole, centered at circle axis)
        translate([0, 0, -0.1])
            cylinder(d=center_bore, h=slug_l + 0.2, $fn=20);

        // PCB slot (axial, from flat face side)
        translate([-pcb_w/2, slug_od/2 - slug_flat - pcb_d, slug_z_for_pcb()])
            cube([pcb_w, pcb_d + 0.1, pcb_t]);

        // PCB slot continues full length for coaxial components
        translate([-pcb_w/2, slug_od/2 - slug_flat - pcb_d, 0])
            cube([pcb_w, pcb_d, slug_l]);

        // Banana pin hole (bottom center, for slug output contact)
        translate([0, -slug_od/2 - 0.1, banana_x])
            rotate([-90, 0, 0])
                cylinder(d=banana_d, h=banana_depth + 0.1, $fn=16);

        // Banana pin counterbore (wider, for nut/flange)
        translate([0, -slug_od/2 - 0.1, banana_x])
            rotate([-90, 0, 0])
                cylinder(d=4.5, h=4, $fn=16);

        // O-ring groove (at slug midpoint)
        translate([0, 0, oring_pos])
            oring_groove();

        // Label recess (on flat face)
        translate([-label_w/2, slug_od/2 - slug_flat - label_depth, slug_l/2])
            cube([label_w, label_depth + 0.1, label_h], center=false);

        // Component access slot (from round side, for toroid winding access)
        translate([-slot_w/2, -slug_od/2 - 0.1, slot_z])
            cube([slot_w, slug_od/3 + 0.1, slot_h]);
    }

    // Toroid retention boss (center of PCB cavity)
    // Small post that the toroid seats against
    translate([0, slug_od/2 - slug_flat - 3, slug_l/2 - 5])
        difference() {
            cylinder(d=5, h=6, $fn=16);
            cylinder(d=3, h=6.1, $fn=12);  // toroid pin hole
        }
}

function slug_z_for_pcb() = 1.5;  // PCB slot starts 1.5mm from end

// ─── Slug Cap (optional, covers PCB end) ─────────────────────────────────────
module slug_cap() {
    difference() {
        linear_extrude(3)
            slug_body_profile();
        translate([0, 0, -0.1])
            cylinder(d=center_bore, h=3.2, $fn=20);
        // Snap into O-ring groove when installed
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────
slug();
translate([slug_od + 5, 0, 0]) slug_cap();

// ─── Notes ────────────────────────────────────────────────────────────────────
// Material: PLA+ (for rigidity, dimensional accuracy)
//           PETG if higher temperature environment (e.g., near PA amplifier)
// Layer:    0.15mm for best dimensional accuracy of O-ring groove
// Infill:   40% for structural rigidity around center bore
// Supports: None required
//
// Assembly:
// (1) Install PCB wafer (22×18mm) into PCB slot — toroid + detector circuit
// (2) Thread secondary winding through center conductor bore during install
// (3) Solder center conductor pin into center bore (tin slug bore first)
// (4) Install 2mm banana pin — solder or press-fit
// (5) Apply O-ring (25.0 × 2mm Buna-N) into groove
// (6) Test: connect to body, verify meter deflects with RF applied
//
// Identification:
// Print element range on flat face with label recess or use small label.
// Color-code via filament: green=HF-10, yellow=HF-100, orange=HF-1kW, red=HF-3kW
