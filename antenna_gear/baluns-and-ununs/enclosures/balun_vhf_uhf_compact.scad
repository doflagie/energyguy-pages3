// VHF/UHF COMPACT BALUN ENCLOSURE — FDM 3D-PRINTABLE
// Purpose: Small, lightweight enclosure for VHF/UHF air-core chokes
// Dimensions: 80×55×40 mm (ultra-compact, minimal weight)
// Features: BNC connectors (mini-sized), snap-fit lid
// Material: PETG recommended (UV-resistant)
// Print time: ~2–3 hours
// Filament: ~25–35 grams

$fn = 28;  // Smooth cylinder quality

// External dimensions
box_l = 80;
box_w = 55;
box_h = 40;
wall = 2.5;  // Thinner walls for VHF/UHF

// Internal cavity
cav_l = box_l - 2*wall;
cav_w = box_w - 2*wall;
cav_h = box_h - 2*wall;

// BNC connector hole diameter
bnc_hole = 9.5;  // For standard BNC plug

// Snap-fit lid clip height
clip_height = 3;

module base() {
    difference() {
        // Outer box
        cube([box_l, box_w, box_h], center=true);

        // Internal cavity
        translate([0, 0, wall])
            cube([cav_l, cav_w, cav_h], center=true);

        // BNC connector hole (front)
        translate([-box_l/2 + wall, 0, box_h/2 - 12])
            cylinder(h=8, r=bnc_hole/2, center=true);

        // BNC connector hole (rear)
        translate([box_l/2 - wall, 0, box_h/2 - 12])
            cylinder(h=8, r=bnc_hole/2, center=true);

        // Drain hole
        translate([0, 0, -box_h/2 + wall])
            cylinder(h=wall+1, r=1.2, center=true);
    }
}

module snap_fit_lid() {
    difference() {
        // Lid plate
        cube([box_l - 2*wall - 2, box_w - 2*wall - 2, 2.5], center=true);

        // Snap-fit notch (fits into base)
        translate([0, 0, -1])
            cube([box_l - 2*wall - 4, box_w - 2*wall - 4, 1.5], center=true);
    }

    // Snap-fit clips (4 corners)
    for(i=[-1, 1])
        for(j=[-1, 1])
            translate([i*(box_l/2 - 8), j*(box_w/2 - 8), 1])
                cube([3, 3, clip_height], center=true);
}

module retention_tabs() {
    // Optional: Retention slots on base for lid stability
    difference() {
        cube([1, 8, 5], center=true);
    }
}

base();

// Uncomment to show lid position:
// translate([0, 0, box_h/2 + 3])
//     snap_fit_lid();

// PRINTING & ASSEMBLY:
// 1. Print base (standard orientation)
// 2. Print snap-fit lid separately
// 3. Assembly:
//    - Insert BNC connectors
//    - Snap lid onto base (push down until clips engage)
//    - No screws required (fast assembly)
// 4. For potting:
//    - Snap lid off before potting
//    - Apply epoxy or silicone inside
//    - Re-snap lid after cure
