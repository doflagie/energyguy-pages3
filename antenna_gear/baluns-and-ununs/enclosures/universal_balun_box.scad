// UNIVERSAL BALUN/UNUN ENCLOSURE — FDM 3D-PRINTABLE
// Purpose: Large, general-purpose enclosure for all balun types
// Dimensions: 130×90×65 mm (largest design, supports stacked cores)
// Features: 2× SO-239 + 2× BNC feedthroughs, 4-bolt lid, 1/4"-20 pole mount
// Material: PETG or ABS recommended
// Print time: ~6–8 hours
// Filament: ~70–100 grams

$fn = 32;

// External dimensions
box_l = 130;
box_w = 90;
box_h = 65;
wall = 3.5;

// Internal cavity
cav_l = box_l - 2*wall;
cav_w = box_w - 2*wall;
cav_h = box_h - 2*wall;

// Connector specifications
so239_hole = 13.0;
bnc_hole = 9.5;

// Lid mounting bolts (M3 corner bolts)
bolt_hole = 3.3;
bolt_inset = 10;

// Pole mount (1/4"-20 TPI = 1.27mm pitch, OD ~6.35mm)
pole_hole = 6.5;

module base_box() {
    difference() {
        // Outer envelope
        cube([box_l, box_w, box_h], center=true);

        // Internal cavity
        translate([0, 0, wall])
            cube([cav_l, cav_w, cav_h], center=true);

        // SO-239 holes (left and right sides)
        translate([-35, -box_w/2 + wall, box_h/2 - 20])
            cylinder(h=10, r=so239_hole/2, center=true);
        translate([35, -box_w/2 + wall, box_h/2 - 20])
            cylinder(h=10, r=so239_hole/2, center=true);

        // BNC holes (back side, upper)
        translate([-20, box_w/2 - wall, box_h/2 - 12])
            cylinder(h=10, r=bnc_hole/2, center=true);
        translate([20, box_w/2 - wall, box_h/2 - 12])
            cylinder(h=10, r=bnc_hole/2, center=true);

        // Drain hole (bottom center)
        translate([0, 0, -box_h/2 + wall])
            cylinder(h=wall+1, r=1.8, center=true);
    }
}

module corner_bolt_holes() {
    // M3 bolt holes for 4-bolt lid retention
    corners = [[-45, -30], [45, -30], [-45, 30], [45, 30]];
    for(c=corners) {
        translate([c[0], c[1], box_h/2 - 2])
            cylinder(h=10, r=bolt_hole/2, center=true);
    }
}

module pole_mount_hole() {
    // 1/4"-20 TPI threaded insert hole (bottom center)
    translate([0, 0, -box_h/2 + wall - 1])
        cylinder(h=wall+2, r=pole_hole/2, center=true);
}

module ventilation_grooves() {
    // Heat dissipation grooves (sides of box)
    for(i=[-20, 0, 20]) {
        translate([box_l/2 - wall - 1, i, 0])
            cube([1, 5, 50], center=true);
    }
}

module lid() {
    difference() {
        // Lid plate (bolt-down design)
        cube([box_l - 2*wall - 4, box_w - 2*wall - 4, 4], center=true);

        // Counter-sinks for M3 bolt heads (cone-shaped)
        corners = [[-40, -25], [40, -25], [-40, 25], [40, 25]];
        for(c=corners) {
            translate([c[0], c[1], 1.5])
                cylinder(h=3, r1=5, r2=3, center=true);
        }

        // Large cavity for components/potting
        translate([0, 0, 0])
            cube([box_l - 2*wall - 8, box_w - 2*wall - 8, 2.5], center=true);
    }
}

// MAIN ASSEMBLY
base_box();
corner_bolt_holes();
pole_mount_hole();
ventilation_grooves();

// Uncomment to show lid:
// translate([0, 0, box_h/2 + 4])
//     lid();

// PRINTING & ASSEMBLY INSTRUCTIONS:
// 1. Print base (standard orientation)
// 2. Print lid separately
// 3. Installation (mixed connectors example):
//    - Left SO-239: Input (50Ω from coax feedline)
//    - Right SO-239: Output (50Ω to tuner or antenna coupler)
//    - Back BNC: Optional secondary connection
//    - Pole mount: Bottom threaded insert (for mast attachment)
// 4. Internal component placement:
//    - FT-240-43 core (or stacked FT-240-43×2) center bottom
//    - Wire routing: Keep separation between primary/secondary
//    - Potting: Optional (allows tight thermal coupling)
// 5. Lid assembly:
//    - Place 4× M3 stainless bolts in corner holes
//    - Ensure gasket/O-ring positioned (if using)
//    - Tighten evenly (finger-tight + 1/4 turn)
// 6. Weatherproofing:
//    - Paint exterior (2-part polyurethane or epoxy coating)
//    - Silicone caulk around lid gasket
//    - Grease bolt threads (prevents corrosion)

// CUSTOMIZATION OPTIONS:
// 1. Change connector types:
//    - Replace SO-239 holes with N-connector (larger diameter)
//    - Replace BNC with SMA (for UHF/microwave)
// 2. Add potting provisions:
//    - Increase internal height (box_h = 75)
//    - Add overflow slot (prevents air bubbles)
// 3. Thermal management:
//    - Add cooling fins to exterior
//    - Increase ventilation grooves
// 4. High-power variant:
//    - Increase wall thickness (wall = 5)
//    - Add aluminum backing plate (heat sink)
//    - Use copper potting compound (improves thermal conductivity)
