// EFHW UNUN COMPACT ENCLOSURE — FDM 3D-PRINTABLE
// Purpose: Mid-size enclosure for EFHW 49:1 unun
// Dimensions: 100×70×50 mm
// Features: SO-239 (coax feed), 2× binding posts (antenna input), internal divider
// Material: PETG recommended
// Print time: ~3–4 hours
// Filament: ~40–55 grams

$fn = 32;

// External dimensions
box_l = 100;
box_w = 70;
box_h = 50;
wall = 3;

// Internal cavity
cav_l = box_l - 2*wall;
cav_w = box_w - 2*wall;
cav_h = box_h - 2*wall;

// SO-239 connector hole
so239_hole = 13.0;

// Binding post hole diameter (banana plug or M4 stud)
binding_hole = 5.5;

// Internal divider (separates primary/secondary windings)
divider_thickness = 2;

module base_box() {
    difference() {
        // Outer envelope
        cube([box_l, box_w, box_h], center=true);

        // Internal cavity
        translate([0, 0, wall])
            cube([cav_l, cav_w, cav_h], center=true);

        // SO-239 hole (front, coax output)
        translate([-box_l/2 + wall, 0, box_h/2 - 15])
            cylinder(h=10, r=so239_hole/2, center=true);

        // Binding post holes (rear, antenna input, left)
        translate([box_l/2 - wall, -10, box_h/2 - 15])
            cylinder(h=10, r=binding_hole/2, center=true);

        // Binding post hole (rear, antenna input, right)
        translate([box_l/2 - wall, 10, box_h/2 - 15])
            cylinder(h=10, r=binding_hole/2, center=true);

        // Drain hole (bottom)
        translate([0, 0, -box_h/2 + wall])
            cylinder(h=wall+1, r=1.5, center=true);
    }
}

module internal_divider() {
    // Separates primary coil area from secondary coil area
    translate([0, 0, 0])
        cube([divider_thickness, box_w - 2*wall - 4, cav_h - 4], center=true);
}

module reinforcement_pads() {
    // Extra material around binding post holes (strengthen weak areas)
    for(pos_y=[-10, 10]) {
        translate([box_l/2 - wall - 5, pos_y, box_h/2 - 15])
            cube([3, 8, 6], center=true);
    }
}

module mounting_tab() {
    // Single tab for mast mounting
    translate([0, -box_w/2 - 8, 0]) {
        cube([3, 10, 10], center=false);
        // M5 bolt hole
        translate([1.5, 5, 5])
            cylinder(h=12, r=2.75, center=true);
    }
}

module lid() {
    difference() {
        cube([box_l - 2*wall - 2, box_w - 2*wall - 2, 3], center=true);
        // Slight depression for gasket seating
        translate([0, 0, -0.5])
            cube([box_l - 2*wall - 6, box_w - 2*wall - 6, 1.5], center=true);
    }
}

// ASSEMBLY
base_box();
mounting_tab();
// reinforcement_pads();  // Uncomment if desired

// Uncomment to show lid:
// translate([0, 0, box_h/2 + 3])
//     lid();

// PRINTING & ASSEMBLY NOTES:
// 1. Print base in standard orientation (SO-239 facing front)
// 2. Print lid separately
// 3. Installation:
//    - SO-239 connector to front (solder coax output)
//    - Binding posts to rear (solder antenna primary wires)
//    - Internal: FT-240-43 (3:21 turns) mounted on base
//    - Potting: Optional (silicone recommended for thermal stability)
// 4. Weatherproofing:
//    - Paint exterior (UV-resistant coating)
//    - Silicone caulk around lid (creates water seal)
//    - Install O-ring if potting (prevents air pockets)
