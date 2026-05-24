/**
 * watson_watt_antenna.scad — TM-ADF-ENC-004 Rev A
 * Adcock / Watson-Watt HF Antenna Element Holders and Junction Box
 *
 * Set includes:
 *   1. element_stake() — ground stake + element base (×4)
 *   2. coax_junction_box() — central summing point, houses baluns + LNA
 *   3. coax_strain_relief() — coax strain relief for feed cables
 *
 * Elements: λ/4 verticals (wire or whip), driven/not-driven,
 *           coax to central box via phase-matched runs.
 *
 * Material: PETG (weather resistant), print in neutral color
 * Envelope: stake 40×40×80mm, junction box 100×80×50mm
 * Document: TM-ADF-ENC-004 Rev A
 */

// ─── Parameters ───────────────────────────────────────────────

// Ground stake (drives into soil, holds element vertical rod)
stake_w   = 30.0;
stake_d   = 30.0;
stake_h   = 120.0;  // total height: 80mm in ground + 40mm above
stake_in_ground = 80.0;  // portion that goes into soil
stake_above     = 40.0;  // above-ground portion
stake_taper     = 5.0;   // taper angle of stake tip

// Element holder on stake
elem_tube_id = 9.0;   // bore for aluminum or fiberglass tube (8mm rod)
elem_tube_od = 14.0;
elem_tube_h  = 40.0;  // holding collar height
set_screw_d  = 3.3;   // M3 set screw hole

// Cable exit on stake
cable_exit_d = 7.0;   // PG7 equivalent cable exit

// Central junction box
jb_w  = 100.0;
jb_d  = 80.0;
jb_h  = 50.0;
jb_r  = 6.0;
jb_wall = 3.0;
jb_floor= 3.5;

// Junction box features
// 4× BNC female connectors (9.5mm hole, from N/S/E/W elements)
bnc_hole_d = 9.5;
// 1× BNC for RF output
// 1× power input (3.5mm or 5.5mm barrel)
pwr_hole_d = 5.5;
// SMA or BNC output
out_hole_d = 6.5;  // SMA

// Ground stake anchor options: 2× guy wire holes at top
guy_hole_d = 4.0;

// PCB standoffs inside junction box
so_h  = 5.0; so_od = 6.0; so_id = 2.7;

// ─── Modules ──────────────────────────────────────────────────

module rounded_box(w, d, h, r) {
    hull()
        for (x=[r, w-r]) for (y=[r, d-r])
            translate([x, y, 0]) cylinder(h=h, r=r, $fn=20);
}

module standoff(x, y) {
    translate([x, y, jb_floor])
        difference() {
            cylinder(h=so_h, d=so_od, $fn=16);
            translate([0,0,-0.5]) cylinder(h=so_h+1, d=so_id, $fn=16);
        }
}

module element_stake() {
    difference() {
        union() {
            // Above-ground collar section
            translate([0, 0, stake_in_ground]) {
                difference() {
                    cube([stake_w, stake_d, stake_above]);
                    // Element tube bore
                    translate([stake_w/2, stake_d/2, -0.5])
                        cylinder(h=stake_above+1, d=elem_tube_id, $fn=24);
                }
                // Set screw bosses (2 sides)
                for (side=[0, 1]) {
                    translate([side*(stake_w-6)+3, stake_d/2, stake_above/2])
                    rotate([0, 90*(1-side*2), 0])
                        cylinder(h=6, d=10, $fn=16, center=true);
                }
            }

            // In-ground stake body (slightly tapered)
            hull() {
                translate([0, 0, stake_in_ground])
                    cube([stake_w, stake_d, 0.1]);
                translate([stake_taper, stake_taper, 0])
                    cube([stake_w-2*stake_taper, stake_d-2*stake_taper, 0.1]);
            }
            // Pointed tip
            translate([stake_w/2, stake_d/2, 0])
                cylinder(h=stake_taper, r1=2, r2=6, $fn=16);
        }

        // Element tube bore clear through
        translate([stake_w/2, stake_d/2, stake_in_ground - 0.5])
            cylinder(h=stake_above + 1, d=elem_tube_id, $fn=24);

        // Set screws (M3, 2× on adjacent faces)
        translate([stake_w/2, stake_d+1, stake_in_ground + stake_above/2])
        rotate([90, 0, 0])
            cylinder(h=stake_d+2, d=set_screw_d, $fn=16);
        translate([-1, stake_d/2, stake_in_ground + stake_above/2])
        rotate([0, 90, 0])
            cylinder(h=stake_w+2, d=set_screw_d, $fn=16);

        // Cable exit (side of stake, at ground level)
        translate([-1, stake_d/2, stake_in_ground + 8])
        rotate([0, 90, 0])
            cylinder(h=stake_w+2, d=cable_exit_d, $fn=16);

        // Guy wire holes at top
        for (y=[8, stake_d-8])
            translate([-1, y, stake_in_ground + stake_above - 8])
            rotate([0, 90, 0])
                cylinder(h=stake_w+2, d=guy_hole_d, $fn=16);
    }
}

module junction_box_body() {
    difference() {
        union() {
            rounded_box(jb_w, jb_d, jb_h, jb_r);
        }

        // Inner cavity
        translate([jb_wall, jb_wall, jb_floor])
            rounded_box(jb_w-2*jb_wall, jb_d-2*jb_wall, jb_h, jb_r-jb_wall+0.5);

        // ── FRONT PANEL: 4 input BNC connectors ─────────────
        // N element: left, S element: right, E: center-right, W: center-left
        translate([15, -0.5, 20]) rotate([-90,0,0])
            cylinder(h=jb_wall+1, d=bnc_hole_d, $fn=16);  // N
        translate([35, -0.5, 20]) rotate([-90,0,0])
            cylinder(h=jb_wall+1, d=bnc_hole_d, $fn=16);  // S
        translate([55, -0.5, 20]) rotate([-90,0,0])
            cylinder(h=jb_wall+1, d=bnc_hole_d, $fn=16);  // E
        translate([75, -0.5, 20]) rotate([-90,0,0])
            cylinder(h=jb_wall+1, d=bnc_hole_d, $fn=16);  // W

        // SENSE input BNC
        translate([90, -0.5, 35]) rotate([-90,0,0])
            cylinder(h=jb_wall+1, d=bnc_hole_d, $fn=16);

        // ── REAR PANEL: output + power ───────────────────────
        translate([30, jb_d+0.5, 20]) rotate([90,0,0])
            cylinder(h=jb_wall+1, d=out_hole_d, $fn=16);  // RF out SMA
        translate([60, jb_d+0.5, 20]) rotate([90,0,0])
            cylinder(h=jb_wall+1, d=pwr_hole_d, $fn=16);  // DC power

        // ── BOTTOM: ground stake bolt holes (mount on stake) ─
        translate([jb_w/2 - 15, jb_d/2, -0.5])
            cylinder(h=jb_floor+1, d=4.3, $fn=16);
        translate([jb_w/2 + 15, jb_d/2, -0.5])
            cylinder(h=jb_floor+1, d=4.3, $fn=16);

        // ── LID BOLT HOLES (4×) ──────────────────────────────
        for (x=[10, jb_w-10]) for (y=[10, jb_d-10])
            translate([x, y, jb_h-4]) {
                cylinder(h=5, d=3.3, $fn=16);
                cylinder(h=3, d=7,   $fn=16);
            }
    }

    // PCB standoffs (4× M3)
    standoff(20, 15);
    standoff(80, 15);
    standoff(20, 60);
    standoff(80, 60);
}

module junction_box_lid() {
    lid_h_total = 10.0;
    lid_overlap = 3.0;
    difference() {
        union() {
            rounded_box(jb_w, jb_d, lid_h_total - lid_overlap, jb_r);
            translate([0, 0, lid_h_total-lid_overlap])
                rounded_box(jb_w-2*jb_wall-0.4, jb_d-2*jb_wall-0.4, lid_overlap, jb_r-jb_wall);
        }
        for (x=[10, jb_w-10]) for (y=[10, jb_d-10])
            translate([x, y, -0.5])
                cylinder(h=lid_h_total+1, d=3.3, $fn=16);
        // Vent holes
        for (i=[0:4])
            translate([15+i*14, jb_d/2-10, -0.5])
                cylinder(h=lid_h_total+1, d=5, $fn=16);
    }
}

// ─── Render ───────────────────────────────────────────────────
$fn = 48;
print_part = "stake";  // "stake" | "box" | "lid" | "all"

if (print_part == "stake") {
    // Print 4 stakes
    for (i=[0:3])
        translate([i*40, 0, 0]) color("tan") element_stake();
} else if (print_part == "box") {
    color("lightgray") junction_box_body();
} else if (print_part == "lid") {
    color("gray") junction_box_lid();
} else {
    color("tan") element_stake();
    color("lightgray", 0.85)
    translate([70, 0, 0]) junction_box_body();
}

// ─── Assembly Notes ───────────────────────────────────────────
// ELEMENT ASSEMBLY:
//   1. Drive 4 stakes into ground at λ/4 spacing (see band table)
//   2. Orient N stake toward magnetic North (use compass)
//   3. Insert λ/4 aluminum or fiberglass rod into each stake tube
//   4. Tighten M3 set screws to secure element
//   5. Run phase-matched RG-58 coax from each stake to junction box
//   6. Place junction box at center (tie to stake with cable tie)
//
// COAX PHASE MATCHING:
//   All 5 coax runs (N, S, E, W, SENSE) must be identical electrical length.
//   Cut all to same physical length from same reel of coax.
//   Verify with NanoVNA TDR before deployment.
