// ============================================================
// COAXIAL TRAP WEATHERPROOF ENCLOSURE
// Two-piece clamshell housing for wound coax trap
// UV-resistant, drain holes, compression seal lip
//
// Print settings:
//   Material:   ASA preferred (UV stable), PETG acceptable
//   Color:      Black (UV resistant pigment)
//   Infill:     40% gyroid
//   Walls:      4 perimeters (weather resistance)
//   Temp:       ASA 255/105 (enclosure mandatory to prevent warping)
//
// Hardware:
//   4x M4 x 20mm stainless hex bolts
//   4x M4 stainless nuts
//   4x M4 flat washers
//   Self-amalgamating tape for final seal (optional)
//
// Cable entry: two holes with strain relief grooves
// Mounting: 1x M6 saddle clamp slot for mast mounting
// ============================================================

/* [Trap Dimensions (from former)] */
// Trap wound outer diameter (former OD + 2*wall + cable_od)
trap_od         = 97.6;    // = 76 + 2*4 + 10.29 + 3 (clearance)

// Trap assembly length (total_length from former + flanges)
trap_length     = 92.0;    // from former calc

// Cable OD entering/exiting the trap
cable_od        = 10.29;   // RG-213

/* [Enclosure Parameters] */
// Wall thickness of enclosure
enc_wall        = 3.5;

// Lid overlap depth (clamshell joint)
lid_overlap     = 8.0;

// Clearance between trap OD and enclosure ID
clearance       = 3.0;

// Drain hole diameter (bottom of each half)
drain_d         = 4.0;

// Number of drain holes
drain_count     = 3;

/* [Cable Strain Relief] */
// Strain relief length
sr_length       = 15.0;

// Strain relief ID (squeeze fit on cable)
sr_id           = cable_od - 0.5;

/* [Mast Mounting Slot] */
// Saddle clamp slot width (U-bolt or hose clamp)
mount_slot_w    = 12.0;

// Saddle clamp slot depth
mount_slot_d    = 30.0;

// Position along length (center)
mount_pos       = trap_length / 2;

$fn = 96;

// ============================================================
// Derived dimensions
// ============================================================
enc_id          = trap_od + 2 * clearance;
enc_od          = enc_id + 2 * enc_wall;
enc_total_len   = trap_length + 2 * sr_length + 2 * enc_wall;

// ============================================================
// Cable strain relief port
// ============================================================
module strain_relief(length, od, id) {
    difference() {
        cylinder(d=od + 2*enc_wall, h=length);
        translate([0,0,-0.01])
            cylinder(d=id, h=length+0.02);
        // Grip grooves (3 rings spaced evenly)
        for (i=[1:3]) {
            translate([0, 0, i * length/4])
                rotate_extrude()
                    translate([id/2, 0, 0])
                        circle(r=1.2);
        }
    }
}

// ============================================================
// Lower half of enclosure (contains drain holes, mounting)
// ============================================================
module enc_lower() {
    difference() {
        union() {
            // Main body (half cylinder, open top)
            intersection() {
                cylinder(d=enc_od, h=enc_total_len);
                translate([-enc_od/2, -enc_od/2, 0])
                    cube([enc_od, enc_od/2, enc_total_len]);
            }
            // Cable ports (left)
            translate([0, 0, enc_wall])
                rotate([90, 0, 180])
                    strain_relief(sr_length + enc_wall, cable_od, sr_id);
            // Cable ports (right)
            translate([0, 0, enc_total_len - enc_wall])
                rotate([90, 0, 0])
                    strain_relief(sr_length + enc_wall, cable_od, sr_id);
            // Mast mounting boss
            translate([0, -enc_od/2, mount_pos + sr_length])
                rotate([90, 0, 0])
                    hull() {
                        cylinder(d=enc_wall*3, h=enc_wall*2);
                        translate([-mount_slot_w/2 - enc_wall, -enc_wall, 0])
                            cube([mount_slot_w + 2*enc_wall, 1, enc_wall*2]);
                    }
        }

        // Bore (inside)
        translate([0, -enc_od/2 - 0.01, enc_wall])
            cube([enc_id/2 + 0.01, enc_od/2 + 0.01, trap_length + 0.02]);
        // Full inner bore
        translate([0, 0, enc_wall])
            intersection() {
                cylinder(d=enc_id, h=trap_length);
                translate([-enc_id/2, -enc_id/2, 0])
                    cube([enc_id, enc_id/2 + 0.01, trap_length]);
            }

        // Bolt holes (4x M4, through flange)
        for (i = [0:3]) {
            a = i * 90 + 45;
            if (cos(a) < 0) {  // lower half only
                translate([enc_od/2 * cos(a), enc_od/2 * sin(a), enc_total_len/4])
                    rotate([0, 90, a])
                        cylinder(d=4.4, h=enc_wall*3, center=true);
            }
        }

        // Bolt holes at ends
        for (x = [-enc_od/4, enc_od/4]) {
            // Lower flange bolts
            translate([x, -enc_od/4, -0.01])
                cylinder(d=4.4, h=enc_wall+0.02);
            translate([x, -enc_od/4, enc_total_len - enc_wall - 0.01])
                cylinder(d=4.4, h=enc_wall+0.02);
        }

        // Drain holes (bottom)
        for (i = [1:drain_count]) {
            translate([0, 0, i * enc_total_len/(drain_count+1)])
                rotate([90, 0, 0])
                    cylinder(d=drain_d, h=enc_od);
        }

        // Mast clamp slot
        translate([-mount_slot_w/2, -enc_od, mount_pos + sr_length - 5])
            cube([mount_slot_w, enc_od/2, mount_slot_d]);

        // Cable through-holes at ends
        translate([0, 0, -0.01])
            cylinder(d=cable_od + 1, h=enc_wall + 0.02);
        translate([0, 0, enc_total_len - enc_wall - 0.01])
            cylinder(d=cable_od + 1, h=enc_wall + 0.02);
    }
}

// ============================================================
// Upper lid (snap/bolt closed over lower)
// ============================================================
module enc_upper() {
    translate([0, 0, enc_total_len + 5])
        mirror([0, 1, 0])
            difference() {
                union() {
                    // Upper half cylinder
                    intersection() {
                        cylinder(d=enc_od, h=enc_total_len);
                        translate([-enc_od/2, 0, 0])
                            cube([enc_od, enc_od/2, enc_total_len]);
                    }
                    // Overlap lip (mates with lower)
                    translate([0, 0, enc_wall])
                        difference() {
                            cylinder(d=enc_id - 0.4, h=trap_length);
                            translate([0,0,-0.01])
                                cylinder(d=enc_id - 2*enc_wall, h=trap_length+0.02);
                        }
                }

                // Inner bore
                translate([0, 0, enc_wall])
                    intersection() {
                        cylinder(d=enc_id, h=trap_length);
                        translate([-enc_id/2, 0, 0])
                            cube([enc_id, enc_id/2+0.01, trap_length]);
                    }

                // Bolt holes (matching lower)
                for (x = [-enc_od/4, enc_od/4]) {
                    translate([x, enc_od/4, -0.01])
                        cylinder(d=4.4, h=enc_wall+0.02);
                    translate([x, enc_od/4, enc_total_len - enc_wall - 0.01])
                        cylinder(d=4.4, h=enc_wall+0.02);
                }

                // Cable cutouts at ends (semi-circular)
                for (z = [enc_wall/2, enc_total_len - enc_wall/2]) {
                    translate([0, enc_od/2, z])
                        rotate([90, 0, 0])
                            cylinder(d=cable_od + 1, h=enc_od/2 + 1);
                }
            }
}

// ============================================================
// Assembly — print both parts, both on flat side down
// ============================================================
enc_lower();
enc_upper();

// ============================================================
// Dimension check
// ============================================================
echo(str("===== ENCLOSURE DIMENSIONS ====="));
echo(str("Enc OD:        ", enc_od, " mm"));
echo(str("Enc ID:        ", enc_id, " mm"));
echo(str("Total length:  ", enc_total_len, " mm"));
echo(str("Trap OD in:    ", trap_od, " mm"));
echo(str("Fits 200mm bed (each half): ", (enc_od <= 195) ? "YES" : "NO"));
echo(str("================================="));
