// ============================================================
//  TRAP HOUSING — Parallel LC Trap for EFQW Roll-Up Antenna
//  TM-EF-ENC-001 Rev A
//  Fits T50 or T68 toroid + leaded/SMD capacitor
//  Snap-fit lid; wire grommets on both ends
//  Material: PETG, UV-stable
//  Print bed: 200×200×200 mm (fits easily)
// ============================================================

// --- Parameters ---
body_w   = 30;   // body width (X)
body_d   = 22;   // body depth (Y)
body_h   = 20;   // body height (Z, excluding lid)
wall     = 2.0;  // wall thickness
floor_t  = 2.0;  // floor thickness

lid_h    = 3.5;  // lid height
snap_h   = 1.5;  // snap boss height
snap_d   = 0.8;  // snap depth (interference)

grom_od  = 5.5;  // wire grommet outer diameter passage
grom_id  = 1.8;  // wire passage inner diameter (#16–18 AWG)
grom_z   = body_h / 2; // grommet center height

core_dia = 13.0; // T50 OD (T68 = 17.5 mm)
core_h   = 6.0;  // toroid height / thickness

cap_l    = 6.0;  // capacitor body length
cap_w    = 3.5;  // capacitor body width
lead_h   = 8.0;  // lead clearance height

$fn = 48;

// ============================================================
module body() {
    difference() {
        // outer shell
        cube([body_w, body_d, body_h]);

        // hollow interior
        translate([wall, wall, floor_t])
            cube([body_w - 2*wall, body_d - 2*wall, body_h]);

        // wire grommet passages — left and right walls
        translate([0, body_d/2, grom_z])
            rotate([0, 90, 0])
                cylinder(d=grom_od, h=wall+1, center=false);

        translate([body_w - wall, body_d/2, grom_z])
            rotate([0, 90, 0])
                cylinder(d=grom_od, h=wall+1, center=false);

        // wire passage inner bore
        translate([-1, body_d/2, grom_z])
            rotate([0, 90, 0])
                cylinder(d=grom_id, h=body_w+2, center=false);

        // snap groove around top perimeter
        translate([wall, wall, body_h - snap_h])
            cube([body_w - 2*wall, body_d - 2*wall, snap_h + 1]);
    }

    // snap-fit posts inside at four corners
    snap_inset = wall + 2.5;
    for (cx = [snap_inset, body_w - snap_inset])
        for (cy = [snap_inset, body_d - snap_inset])
            translate([cx, cy, body_h - snap_h - 1])
                cylinder(d=2.5, h=snap_h + 1);
}

// ============================================================
module core_pocket() {
    // Recess in floor to locate toroid; toroid rests flat
    translate([body_w/2, body_d/2, floor_t])
        cylinder(d=core_dia + 1.5, h=core_h/2);
}

// ============================================================
module interior_detail() {
    // Capacitor slot recess on floor beside core
    translate([body_w/2 + core_dia/2 + 1, body_d/2 - cap_w/2, floor_t])
        cube([cap_l, cap_w, lead_h]);
}

// ============================================================
module lid() {
    translate([0, 0, body_h]) {
        difference() {
            // outer lid plate
            cube([body_w, body_d, lid_h]);

            // hollow underside (snap skirt fits over body)
            translate([wall + snap_d, wall + snap_d, 0])
                cube([body_w - 2*(wall + snap_d),
                      body_d - 2*(wall + snap_d),
                      lid_h - wall]);

            // wire passage continuous through lid
            translate([-1, body_d/2, lid_h/2])
                rotate([0, 90, 0])
                    cylinder(d=grom_id, h=body_w + 2);
        }
    }
}

// ============================================================
// Label boss on top face
module label_boss() {
    translate([body_w/2, body_d/2, body_h + lid_h])
        linear_extrude(0.5)
            text("LC TRAP", size=3.5, halign="center", valign="center",
                 font="Liberation Sans:style=Bold");
}

// ============================================================
// RENDER — body + lid exploded 10 mm for inspection
body();
difference() {
    body();
    core_pocket();
    interior_detail();
}
// Re-render clean (difference subtracted from body above, body() alone is the shell)

// Lid offset upward for print layout (print both flat)
translate([body_w + 5, 0, 0]) {
    lid();
    label_boss();
}

// ============================================================
// Assembly notes (CM):
// 1. Wind toroid per trap_winding_tables.csv for the target band.
// 2. Drop toroid into core_pocket; solder capacitor leads to winding ends.
// 3. Thread antenna wire through grommet passage; solder to winding ends.
// 4. Snap lid closed.  Apply UV-cure potting compound if weather seal needed.
// 5. Mark band label on top (e.g., "10m" for 28.5 MHz trap).
// ============================================================
