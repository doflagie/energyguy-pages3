// ============================================================
// COAXIAL TRAP FORMER
// Single-layer coax winding former with integral end flanges
// Fits 200x200x200mm build volume (split in half for large sizes)
//
// PARAMETERS: Edit the variables below for each band/coax type.
//
// Print settings:
//   Material:   PETG or ASA (UV and weather resistant)
//   Infill:     30% minimum
//   Walls:      3 perimeters
//   Temp:       PETG 235/85, ASA 255/100 (enclosure recommended)
//   Support:    None required
//   Orientation: Stand vertically on end flange
// ============================================================

/* [Winding Parameters] */
// Former outer diameter (mm) — matches coil diameter column in winding table
former_od       = 76.0;   // e.g. 76 for 40M RG-213

// Wall thickness of former tube (mm)
former_wall     = 4.0;

// Cable outer diameter (mm): RG-58=4.95, RG-8X=6.15, RG-213=10.29
cable_od        = 10.29;  // RG-213

// Number of turns
turns           = 7.5;

// Inter-turn gap (mm) — 0=close-wound, 1-2=spaced
gap             = 0.5;

/* [Flange Parameters] */
// Flange OD = former_od + 2*flange_extend
flange_extend   = 12.0;

// Flange thickness (mm)
flange_thick    = 5.0;

// Number of mounting holes in flange
mount_holes     = 4;

// Mounting hole diameter (mm)
mount_hole_d    = 4.5;   // M4 clearance

/* [Feed-Through Cable Ports] */
// Port hole diameter — slightly larger than cable
port_d          = cable_od + 2.0;

// Port offset from former axis (radial, on flange face)
port_offset     = former_od/2 + former_wall + 2.0;

/* [Trimmer Capacitor Access] */
// Add access hole for trimmer capacitor on side
add_trimmer_port = true;  // true or false

// Trimmer port diameter (mm) — fits 5-30 pF Sprague-Goodman or similar
trimmer_port_d  = 12.0;

/* [Split Former Option] */
// Set true to generate half-cylinder (for large formers)
split_former    = false;

// ============================================================
// Derived dimensions
// ============================================================
former_id       = former_od - 2 * former_wall;
pitch           = cable_od + gap;
winding_length  = turns * pitch;
total_length    = winding_length + 2 * flange_thick;
flange_od       = former_od + 2 * flange_extend;

$fn = 128;

// ============================================================
// Groove helix module
// Approximated as stacked rings (printable without supports)
// ============================================================
module coax_groove(turns, pitch, r, groove_d) {
    for (i = [0 : 1 : ceil(turns) - 1]) {
        translate([0, 0, i * pitch + pitch/2])
            difference() {
                cylinder(h=pitch + 0.01, r=r + groove_d/2 + 0.5, center=true);
                cylinder(h=pitch + 0.02, r=r - groove_d/2, center=true);
                // Groove profile: semi-circular channel
                rotate_extrude()
                    translate([r, 0, 0])
                        circle(d=groove_d);
            }
    }
}

// ============================================================
// Flange with cable port and mounting holes
// ============================================================
module flange(is_top) {
    difference() {
        union() {
            // Flange disc
            cylinder(d=flange_od, h=flange_thick);
            // Boss for former tube
            cylinder(d=former_od + 2*former_wall, h=flange_thick + 3);
        }

        // Hollow center
        translate([0,0,-0.01])
            cylinder(d=former_id, h=flange_thick + 4);

        // Cable entry/exit port
        port_angle = is_top ? 90 : 270;
        translate([port_offset * cos(port_angle),
                   port_offset * sin(port_angle),
                   -0.01])
            cylinder(d=port_d, h=flange_thick + 4);

        // Mounting holes
        for (i = [0 : mount_holes - 1]) {
            a = i * 360 / mount_holes + 45;
            r = flange_od/2 - 8;
            translate([r*cos(a), r*sin(a), -0.01])
                cylinder(d=mount_hole_d, h=flange_thick + 2);
        }
    }
}

// ============================================================
// Main former body
// ============================================================
module former_body() {
    difference() {
        // Outer tube
        cylinder(d=former_od + 2*former_wall, h=winding_length);
        // Inner bore
        translate([0, 0, -0.01])
            cylinder(d=former_id, h=winding_length + 0.02);
        // Winding grooves
        translate([0, 0, 0])
            for (i = [0 : ceil(turns) - 1]) {
                z = i * pitch + pitch/2;
                translate([0, 0, z])
                    rotate_extrude()
                        translate([(former_od + 2*former_wall)/2, 0, 0])
                            circle(d=cable_od + 0.5);
            }
        // Trimmer capacitor port (on side, mid-winding)
        if (add_trimmer_port) {
            translate([former_od/2 + former_wall, 0, winding_length/2])
                rotate([0, 90, 0])
                    cylinder(d=trimmer_port_d, h=former_wall*3, center=true);
        }
    }
}

// ============================================================
// Complete former assembly
// ============================================================
module trap_former() {
    if (split_former) {
        // Half former for large sizes (print two halves, join with M3 bolts)
        intersection() {
            trap_former_full();
            translate([-flange_od/2 - 1, 0, -0.01])
                cube([flange_od + 2, flange_od + 2, total_length + 0.02]);
        }
    } else {
        trap_former_full();
    }
}

module trap_former_full() {
    union() {
        // Bottom flange
        flange(false);
        // Former body
        translate([0, 0, flange_thick])
            former_body();
        // Top flange
        translate([0, 0, flange_thick + winding_length])
            flange(true);
    }
}

// ============================================================
// Generate the part
// ============================================================
trap_former();

// ============================================================
// Dimension check — prints to console
// ============================================================
echo(str("===== TRAP FORMER DIMENSIONS ====="));
echo(str("Former OD:        ", former_od, " mm"));
echo(str("Former ID (bore): ", former_id, " mm"));
echo(str("Flange OD:        ", flange_od, " mm"));
echo(str("Total length:     ", total_length, " mm"));
echo(str("Winding length:   ", winding_length, " mm"));
echo(str("Pitch per turn:   ", pitch, " mm"));
echo(str("Cable OD:         ", cable_od, " mm"));
echo(str("Turns:            ", turns));
echo(str("Fits 200x200 bed: ", (flange_od <= 195 && total_length <= 195) ? "YES" : "SPLIT REQUIRED"));
echo(str("==================================="));
