// BALUN/UNUN HF COMPACT ENCLOSURE — FDM 3D-PRINTABLE
// Purpose: Weatherproof enclosure for HF balun/unun devices
// Dimensions: 110×80×55 mm (fits FDM printer bed)
// Features: SO-239 connectors (opposite ends), O-ring groove, mounting tabs
// Material: PETG recommended (UV-resistant), ABS acceptable
// Print time: ~4–6 hours @ 0.2mm layer height
// Filament: ~50–70 grams

// CUSTOMIZATION PARAMETERS
$fn = 32;  // Cylinder quality (32 = smooth, 16 = fast)

// Box dimensions (external)
box_length = 110;   // Front to back (SO-239 to SO-239)
box_width = 80;     // Left to right
box_height = 55;    // Top to bottom
wall_thickness = 3; // Perimeter wall (3mm recommended)

// Internal cavity dimensions
internal_length = box_length - 2 * wall_thickness;
internal_width = box_width - 2 * wall_thickness;
internal_height = box_height - 2 * wall_thickness;

// SO-239 connector specifications
so239_diameter = 13.5;  // Outer diameter of bushing
so239_hole = 13.0;      // Hole to drill for SO-239
so239_z_pos = box_height / 2;  // Vertical center

// O-ring groove (silicone seal on lid)
oring_diameter = 78;    // ID of O-ring groove
oring_depth = 2.5;      // Groove depth

// Mounting tabs (M5 bolt holes)
tab_thickness = 3;
tab_height = 8;
bolt_hole_diameter = 5.5;  // For M5 bolt

module base_box() {
    difference() {
        // Outer envelope
        cube([box_length, box_width, box_height], center=true);

        // Internal cavity
        translate([0, 0, wall_thickness])
            cube([internal_length, internal_width, internal_height], center=true);

        // SO-239 connector hole (front)
        translate([-box_length/2 + wall_thickness, 0, so239_z_pos])
            cylinder(h=10, r=so239_hole/2, center=true);

        // SO-239 connector hole (rear)
        translate([box_length/2 - wall_thickness, 0, so239_z_pos])
            cylinder(h=10, r=so239_hole/2, center=true);

        // Drain hole (bottom, center)
        translate([0, 0, -box_height/2 + wall_thickness])
            cylinder(h=wall_thickness+1, r=1.5, center=true);
    }
}

module mounting_tabs() {
    // Left tab (M5 bolt hole)
    translate([-box_length/2 - 8, -box_width/2 - 8, 0]) {
        difference() {
            cube([tab_thickness, 16, tab_height], center=false);
            // M5 bolt hole
            translate([tab_thickness/2, 8, tab_height/2])
                cylinder(h=tab_height+2, r=bolt_hole_diameter/2, center=true);
        }
    }

    // Right tab (M5 bolt hole)
    translate([box_length/2 + 5, -box_width/2 - 8, 0]) {
        difference() {
            cube([tab_thickness, 16, tab_height], center=false);
            translate([tab_thickness/2, 8, tab_height/2])
                cylinder(h=tab_height+2, r=bolt_hole_diameter/2, center=true);
        }
    }
}

module lid() {
    // Top cover plate (separate piece)
    difference() {
        cube([box_length - 2*wall_thickness, box_width - 2*wall_thickness, 3], center=true);

        // O-ring groove (silicone seal surface)
        translate([0, 0, -0.5])
            cylinder(h=1.5, r=oring_diameter/2, center=true);
    }
}

// ASSEMBLY (uncomment to visualize)
base_box();
mounting_tabs();

// Show lid position (commented out for printing base only)
// translate([0, 0, box_height/2 + 3])
//     lid();

// PRINTING INSTRUCTIONS
// 1. Print base_box() as main body (standing upright)
// 2. Print mounting_tabs() (or include in same print)
// 3. Print lid() separately (easier to remove)
// 4. Post-processing:
//    - Clean support material
//    - Sand connector hole area (ensure SO-239 fits smoothly)
//    - Paint exterior (UV-resistant coating recommended)
//    - Install O-ring in groove (silicone, 78mm ID)
// 5. Assembly:
//    - Insert SO-239 connectors in front/rear holes (solder from inside)
//    - Route internal wiring (avoid sharp edges)
//    - Apply silicone caulk around O-ring (if potting)
//    - Attach lid with self-tapping screws (3–4 screws, M3 recommended)

// MATERIAL NOTES
// PETG: Excellent UV resistance, flexible (won't crack), print at 240°C
// ABS: Stronger, less UV-resistant, requires heated bed (100°C)
// PLA: Weak for outdoor use, breaks under thermal stress

// CUSTOMIZATION EXAMPLES
// For compact VHF/UHF (reduce height):
//   box_height = 40; box_length = 90; box_width = 60;
// For QRO/stacked cores (increase size):
//   box_height = 65; box_length = 120; box_width = 100;
// For single BNC connector:
//   Add second hole (different diameter), mirror SO-239 hole definition
