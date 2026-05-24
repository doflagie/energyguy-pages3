// ============================================================
// TM-WHP-ENC-001 Rev A
// Loading Coil Housing — Plug-in Modular, NMO Bayonet Base
// 5.6m Telescopic Whip Antenna Project
// ============================================================
// PURPOSE:
//   Cylindrical snap-lid housing for wound toroid or air-core
//   solenoid loading coils.  NMO-style male bayonet base plugs
//   into the base-unit socket (TM-WHP-ENC-002).  Two terminal
//   pass-throughs for antenna wire connections.
//
// PRINT SETTINGS (PETG):
//   Layer height : 0.2 mm
//   Perimeters   : 3
//   Infill       : 25 % gyroid
//   Supports     : none required for body; brim recommended
//   Orientation  : body upright; lid printed separately upright
//
// SIZES (use coil_od / coil_h to switch):
//   T200-2 (160 m) : coil_od = 58, coil_h = 40, band_label = "160m COIL"
//   T130-2 (80 m)  : coil_od = 42, coil_h = 35, band_label = "80m COIL"  ← DEFAULT
//   T106-2 (60 m)  : coil_od = 36, coil_h = 28, band_label = "60m COIL"
//   T68-2  (40 m)  : coil_od = 26, coil_h = 20, band_label = "40m COIL"
//   T50-2  (30 m)  : coil_od = 22, coil_h = 18, band_label = "30m COIL"
//
// ASSEMBLY:
//   1. Wind toroid / form coil.
//   2. Route lead wires through terminal post holes.
//   3. Drop coil into housing body from top.
//   4. Press lid down until snap ring engages.
//   5. Align bayonet pins with base-unit slots; push down, rotate
//      clockwise ~45° to lock.
// ============================================================

$fn = 64;

// ── Primary parameters (T130-2 default) ───────────────────────
coil_od    = 42;        // toroid OD + 2 mm radial clearance
coil_h     = 35;        // toroid height + 2 mm axial clearance
wall       = 3.0;       // shell wall thickness
band_label = "80m COIL";

// ── Derived shell dimensions ───────────────────────────────────
shell_od   = coil_od + 2 * wall;
shell_id   = coil_od;
shell_h    = coil_h + 10;   // 5 mm floor + 5 mm headroom

// ── NMO bayonet base (male stub, 3/4-inch NMO style) ──────────
base_od    = 22;
base_h     = 12;
pin_od     = 3.0;
pin_h      = 6.0;

// ── Terminal pass-through posts ────────────────────────────────
term_d     = 2.5;

// ── Knurl parameters ──────────────────────────────────────────
knurl_ribs      = 24;   // number of vertical grip ridges
knurl_rib_depth = 0.8;  // radial depth of each ridge
knurl_rib_w     = 1.0;  // angular width at top of ridge (mm approx)

// ── Lid snap parameters ────────────────────────────────────────
lid_t      = 2.5;   // lid disc thickness
snap_h     = 3.0;   // snap ring height
snap_depth = 0.8;   // radial snap engagement

// ── Label emboss parameters ────────────────────────────────────
label_depth = 0.6;  // emboss depth into top face

// ==============================================================
// MODULE: shell
//   Cylindrical outer body with knurl grip ridges and floor.
//   Interior bore = coil_od (shell_id).
// ==============================================================
module shell() {
    difference() {
        union() {
            // Smooth outer cylinder
            cylinder(d = shell_od, h = shell_h);

            // Vertical knurl ribs on outer surface
            for (i = [0 : knurl_ribs - 1]) {
                rotate([0, 0, i * (360 / knurl_ribs)])
                translate([shell_od / 2 - knurl_rib_depth / 2, 0, 0])
                    linear_extrude(height = shell_h)
                        circle(d = knurl_rib_depth * 2);
            }
        }

        // Central bore (stops 3 mm above bottom = floor)
        translate([0, 0, 3])
            cylinder(d = shell_id, h = shell_h);

        // Snap groove on inner top rim for lid
        translate([0, 0, shell_h - snap_h - 0.5])
            difference() {
                cylinder(d = shell_id + 1.0, h = snap_h);
                cylinder(d = shell_id - snap_depth * 2, h = snap_h);
            }
    }
}

// ==============================================================
// MODULE: bayonet_base
//   Male NMO-style stub with two locking pins at 180°.
//   Pins ride in base-unit bayonet slots to lock the module.
// ==============================================================
module bayonet_base() {
    // Main stub cylinder
    cylinder(d = base_od, h = base_h);

    // Two locking pins, diametrically opposite
    for (angle = [0, 180]) {
        rotate([0, 0, angle])
        translate([base_od / 2, 0, base_h - pin_h / 2])
            sphere(d = pin_od);
    }
}

// ==============================================================
// MODULE: terminal_posts
//   Two 2.5 mm wire pass-through holes:
//     - top face,  offset from centre toward front
//     - side wall, 8 mm up from shell floor
// ==============================================================
module terminal_posts() {
    // Top face hole
    translate([shell_od / 4, 0, shell_h - 0.1])
        cylinder(d = term_d, h = wall + 0.2);

    // Side wall hole (front, at 0°)
    translate([shell_od / 2 - wall - 0.1, 0, 8])
        rotate([0, 90, 0])
            cylinder(d = term_d, h = wall + 0.2);
}

// ==============================================================
// MODULE: label_boss
//   Embossed text on top face.  Depth is subtracted from top.
// ==============================================================
module label_boss(text_str = band_label) {
    translate([0, 0, shell_h - label_depth])
        linear_extrude(height = label_depth + 0.01)
            text(text_str,
                 size   = 5,
                 halign = "center",
                 valign = "center",
                 font   = "Liberation Sans:style=Bold");
}

// ==============================================================
// MODULE: lid
//   Snap-on cap printed separately.  Snap bead engages groove
//   in top rim of shell.
// ==============================================================
module lid() {
    // Top disc
    difference() {
        union() {
            cylinder(d = shell_od + knurl_rib_depth * 2 + 0.2, h = lid_t);

            // Snap bead ring on underside inner edge
            translate([0, 0, -snap_h + 0.4])
                difference() {
                    cylinder(d = shell_id - 0.4,          h = snap_h);
                    cylinder(d = shell_id - 0.4 - snap_depth * 2, h = snap_h);
                }
        }

        // Top wire hole (same position as body top hole)
        translate([shell_od / 4, 0, -0.1])
            cylinder(d = term_d, h = lid_t + 0.2);

        // Optional finger lift slot
        translate([-(shell_od / 2 - 5), -3, -0.1])
            cube([shell_od - 10, 6, lid_t + 0.2]);
    }
}

// ==============================================================
// MAIN RENDER
//   Body at origin; lid offset above for print layout preview.
// ==============================================================

// Housing body
difference() {
    union() {
        shell();
        translate([0, 0, -base_h])
            bayonet_base();
    }
    terminal_posts();
    // Emboss label into top face (label_boss adds geometry,
    // so subtract a recessed version for engraved style)
    translate([0, 0, shell_h - label_depth - 0.01])
        linear_extrude(height = label_depth + 0.1)
            text(band_label,
                 size   = 5,
                 halign = "center",
                 valign = "center",
                 font   = "Liberation Sans:style=Bold");
}

// Lid — offset upward by 15 mm for print-preview layout
translate([0, 0, shell_h + 15])
    lid();

// ── End of TM-WHP-ENC-001 ────────────────────────────────────
