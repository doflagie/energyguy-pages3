// ============================================================
//  ROLL-UP ANTENNA WIRE SPOOL
//  TM-EF-ENC-003 Rev A
//  Stores ~11m of #18 AWG wire + up to 5 LC trap housings
//  Trap housings clip into recesses on spool flanges
//  Handle + hanging hole for field use
//  Material: PETG; print in two halves, join with M3 screws
//  Print bed: 200×200×200 mm (each half fits easily)
// ============================================================

// --- Spool geometry ---
flange_od  = 160;   // flange outer diameter
flange_t   = 6;     // flange thickness
hub_od     = 60;    // hub (barrel) outer diameter
hub_h      = 45;    // hub height (wire capacity)
wt         = 3.0;   // general wall thickness

// --- Wire guide slot in flange ---
wire_slot_w  = 5;
wire_slot_d  = 4;

// --- Trap clip recesses in flange (5 positions) ---
trap_w   = 32;   // trap housing width + clearance
trap_d   = 25;   // trap housing depth
trap_h   = 4;    // recess depth in flange face
n_traps  = 5;

// --- Handle ---
handle_l  = 80;
handle_w  = 20;
handle_t  = 8;
handle_r  = 10;   // corner radius
hang_d    = 10;   // hanging hole diameter

// --- Axle ---
axle_d    = 12;
axle_h    = hub_h + 2 * flange_t + 10;

// --- Split joint ---
screw_d   = 3.4;   // M3 clearance
screw_z   = [10, hub_h/2, hub_h - 10];

$fn = 64;

// ============================================================
module flange(show_clips = true) {
    difference() {
        cylinder(d=flange_od, h=flange_t);

        // axle hole
        cylinder(d=axle_d, h=flange_t + 1);

        // wire slot
        translate([hub_od/2, -wire_slot_w/2, flange_t - wire_slot_d])
            cube([(flange_od - hub_od)/2, wire_slot_w, wire_slot_d + 1]);

        // trap clip recesses (evenly spaced around flange, outside hub)
        if (show_clips) {
            for (i = [0:n_traps-1]) {
                ang = i * (360 / n_traps);
                r   = (hub_od/2 + flange_od/2) / 2;
                rotate([0, 0, ang])
                    translate([r - trap_w/2, -trap_d/2, flange_t - trap_h])
                        cube([trap_w, trap_d, trap_h + 1]);
            }
        }

        // Split line — flat on one side for two-piece join
        translate([-flange_od/2 - 1, -flange_od/2 - 1, -1])
            cube([flange_od/2 + 1, flange_od + 2, flange_t + 2]);
    }
}

// ============================================================
module hub() {
    difference() {
        cylinder(d=hub_od, h=hub_h);
        cylinder(d=axle_d, h=hub_h + 1);

        // M3 screw holes for split join
        for (z = screw_z) {
            translate([hub_od/2 - wt - 1, 0, z])
                rotate([0, 90, 0])
                    cylinder(d=screw_d, h=wt + 2);
        }

        // wire anchor notch
        translate([axle_d/2, -2, hub_h/2 - 10])
            cube([hub_od/2 - axle_d/2, 4, 20]);
    }
}

// ============================================================
module handle_shape() {
    translate([flange_od/2 + 2, -handle_w/2, 0]) {
        difference() {
            // main handle body
            hull() {
                cube([handle_l - handle_r, handle_w, handle_t]);
                translate([handle_l - handle_r, handle_w/2, 0])
                    cylinder(r=handle_r, h=handle_t);
            }

            // hanging hole
            translate([handle_l - handle_r, handle_w/2, -1])
                cylinder(d=hang_d, h=handle_t + 2);
        }
    }
}

// ============================================================
// Half-spool assembly (print two, mirror the second)
module half_spool() {
    hub();
    translate([0, 0, -flange_t]) flange(show_clips=true);
    translate([0, 0, hub_h])     flange(show_clips=false);
    translate([0, 0, hub_h/2 + flange_t/2 - handle_t/2])
        handle_shape();
}

// ============================================================
// RENDER — two halves side by side
half_spool();

translate([flange_od + 20, 0, 0])
    mirror([1, 0, 0])
        half_spool();

// ============================================================
// Assembly instructions:
// 1. Print both halves in PETG.
// 2. Join at flat face with 3× M3×20 screws + M3 nyloc nuts.
// 3. Wire anchor: thread wire end through hub notch; knot inside.
// 4. Wind antenna wire onto hub in neat layers.
// 5. Clip LC trap housings into flange recesses (highest-freq traps on inner positions).
// 6. Wire passes through flange wire slot as it unwinds.
// 7. Hang from handle hole; deploy wire downward/upward as needed.
//
// Capacity estimate (hub):
//   Wire OD ~2mm (#18 THHN); hub_od=60, flange_od=160, hub_h=45
//   Winding area = (160-60)/2 × 45 = 2250 mm²; ~12m of #18 AWG
// ============================================================
