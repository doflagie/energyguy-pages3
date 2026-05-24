// ============================================================
// TM-WHP-ENC-002 Rev A
// Whip Base Unit — NMO Socket, L-Network, Radials, SMA Input
// 5.6m Telescopic Whip Antenna Project
// ============================================================
// PURPOSE:
//   Main base housing mounts at the bottom of the whip assembly.
//   Accepts plug-in loading coil modules (TM-WHP-ENC-001) via
//   a female NMO bayonet socket on the top face.  Contains the
//   L-network component bay, SMA(F) coax input, 4 radial banana
//   jacks, 3 LED SWR indicators, choke balun bolt holes, and a
//   1/4-20 camera-tripod insert pocket in the base flange.
//
// PRINT SETTINGS (PETG):
//   Layer height : 0.2 mm
//   Perimeters   : 4
//   Infill       : 30 % gyroid
//   Supports     : required inside for NMO boss floor; use
//                  support blockers to limit to that area
//   Orientation  : body upright (lid printed separately)
//
// HARDWARE INSERTS (heat-set, M3 unless noted):
//   - 4 × M3 corner lid screws
//   - 4 × M3 component mounting posts
//   - 1 × 1/4-20 brass insert, 10 mm deep (tripod pocket)
//   - SMA(F) panel jack, 6.5 mm hole, M2 × 4 flanged screws
//   - 4 × banana jack sockets, 4.2 mm holes
//   - 3 × 3 mm LED bezels (front panel)
//
// ASSEMBLY:
//   1. Install heat-set inserts before housing cools fully.
//   2. Wire SMA centre pin to L-network series toroid input.
//   3. Solder toroid → trimmer cap → output post.
//   4. Mount board on internal posts with M3 × 6 screws.
//   5. Install banana jacks, LEDs, SMA.
//   6. Snap coil module onto NMO socket; radial wires to bananas.
//   7. Press lid and secure with 4 × M3 × 10 cap screws.
//
// CHOKE BALUN NOTE:
//   FT-240-31 toroid choke mounts externally on right side wall.
//   Four M4 bolt holes provided for 60 × 60 mm bolt pattern.
// ============================================================

$fn = 64;

// ── Box dimensions ────────────────────────────────────────────
bx      = 100;   // width  (X)
by      = 100;   // depth  (Y)
bz      = 60;    // height (Z)
wall    = 3.0;
floor_t = 3.0;

// ── NMO socket (female) ───────────────────────────────────────
nmo_od      = 24;    // outer boss diameter
nmo_id      = 22;    // inner bore (accepts male stub of coil module)
nmo_h       = 14;    // boss height above box top face
slot_w      = 3.5;   // bayonet slot width
slot_l      = 8.0;   // bayonet slot length
slot_angle  = 45;    // twist angle for bayonet lock

// ── Connector / jack dimensions ───────────────────────────────
sma_d       = 6.5;   // SMA panel jack hole
banana_d    = 4.2;   // banana jack hole
led_d       = 3.2;   // LED bezel hole

// ── Tripod insert ─────────────────────────────────────────────
tripod_d    = 6.35;  // 1/4-20 hole (6.35 mm for heat-set insert)
tripod_h    = 10.0;

// ── Internal component posts ─────────────────────────────────
post_d      = 6.0;
post_h      = bz - floor_t - 8;
post_offset = 15;    // inset from corner

// ── Lid ───────────────────────────────────────────────────────
lid_t       = 2.5;
lid_screw_d = 3.2;

// ── Choke balun bolt pattern (right side wall) ────────────────
balun_bolt_d  = 4.2;  // M4 clearance
balun_pattern = 60;   // centre-to-centre bolt spacing (square)

// ==============================================================
// MODULE: main_box
//   Hollow box with floor, open top, rounded exterior corners
//   (achieved by hull + sphere trick at 2 mm radius).
// ==============================================================
module main_box() {
    r = 3; // corner rounding radius
    difference() {
        // Outer shell — rounded rectangular prism
        hull() {
            for (x = [-bx/2 + r, bx/2 - r])
            for (y = [-by/2 + r, by/2 - r]) {
                translate([x, y, 0])
                    cylinder(r = r, h = bz);
            }
        }

        // Interior cavity
        translate([-(bx/2 - wall), -(by/2 - wall), floor_t])
            cube([bx - 2*wall, by - 2*wall, bz]);

        // Open top
        translate([-(bx/2 - wall), -(by/2 - wall), bz - 0.1])
            cube([bx - 2*wall, by - 2*wall, bz]);
    }
}

// ==============================================================
// MODULE: nmo_socket
//   Female NMO bayonet receiver boss on top face.
//   Two L-shaped bayonet slots at 180° allow the male coil
//   module stub to push in and twist 45° to lock.
//
//   Slot geometry: vertical entry leg + horizontal lock leg.
// ==============================================================
module nmo_socket() {
    difference() {
        // Boss cylinder
        cylinder(d = nmo_od, h = nmo_h);

        // Centre bore for male stub
        cylinder(d = nmo_id, h = nmo_h + 0.1);

        // Two bayonet slots, 180° apart
        for (angle = [0, 180]) {
            rotate([0, 0, angle]) {
                // Vertical entry slot (top of boss)
                translate([nmo_id/2 - 0.1, -slot_w/2, nmo_h - slot_l])
                    cube([nmo_od/2 - nmo_id/2 + 0.2, slot_w, slot_l + 0.1]);

                // Horizontal lock leg (45° twist, offset downward 3 mm)
                translate([0, 0, nmo_h - slot_l - 3])
                    rotate([0, 0, slot_angle])
                    translate([nmo_id/2 - 0.1, -slot_w/2, 0])
                        cube([nmo_od/2 - nmo_id/2 + 0.2, slot_w, slot_l/2]);
            }
        }
    }
}

// ==============================================================
// MODULE: sma_cutout
//   SMA(F) panel jack hole on front face (Y = -by/2),
//   centred in X, 15 mm up from floor.
// ==============================================================
module sma_cutout() {
    translate([0, -(by/2 + 0.1), 15])
        rotate([-90, 0, 0])
            cylinder(d = sma_d, h = wall + 0.2);
}

// ==============================================================
// MODULE: banana_cutouts
//   Four banana jack holes, one per cardinal face, 25 mm up.
// ==============================================================
module banana_cutouts() {
    // Front (Y-)
    translate([0, -(by/2 + 0.1), 25])
        rotate([-90, 0, 0])
            cylinder(d = banana_d, h = wall + 0.2);

    // Rear (Y+)
    translate([0, by/2 - wall - 0.1, 25])
        rotate([-90, 0, 0])
            cylinder(d = banana_d, h = wall + 0.2);

    // Left (X-)
    translate([-(bx/2 + 0.1), 0, 25])
        rotate([0, 90, 0])
            cylinder(d = banana_d, h = wall + 0.2);

    // Right (X+)
    translate([bx/2 - wall - 0.1, 0, 25])
        rotate([0, 90, 0])
            cylinder(d = banana_d, h = wall + 0.2);
}

// ==============================================================
// MODULE: led_cutouts
//   Three 3.2 mm LED holes on front face, 35 mm up.
//   Spacing: 12 mm apart, centred.
// ==============================================================
module led_cutouts() {
    for (x = [-12, 0, 12]) {
        translate([x, -(by/2 + 0.1), 35])
            rotate([-90, 0, 0])
                cylinder(d = led_d, h = wall + 0.2);
    }
}

// ==============================================================
// MODULE: tripod_pocket
//   1/4-20 insert pocket on bottom face, centred.
// ==============================================================
module tripod_pocket() {
    translate([0, 0, -0.1])
        cylinder(d = tripod_d, h = tripod_h + 0.1);
}

// ==============================================================
// MODULE: balun_bolt_holes
//   Four M4 clearance holes on right side wall (X+)
//   for FT-240-31 external choke balun bracket.
//   60 × 60 mm bolt pattern centred at mid-wall height.
// ==============================================================
module balun_bolt_holes() {
    half = balun_pattern / 2;
    z_centre = bz / 2;
    for (y = [-half/2, half/2])
    for (z = [z_centre - half/2, z_centre + half/2]) {
        translate([bx/2 - wall - 0.1, y, z])
            rotate([0, 90, 0])
                cylinder(d = balun_bolt_d, h = wall + 0.2);
    }
}

// ==============================================================
// MODULE: component_posts
//   Four internal mounting posts for L-network PCB / board.
//   M3 blind hole on top for screw.
// ==============================================================
module component_posts() {
    for (x = [-(bx/2 - wall - post_offset), (bx/2 - wall - post_offset)])
    for (y = [-(by/2 - wall - post_offset), (by/2 - wall - post_offset)]) {
        translate([x, y, floor_t]) {
            difference() {
                cylinder(d = post_d, h = post_h);
                translate([0, 0, post_h - 6])
                    cylinder(d = 2.5, h = 6.1);  // M3 blind tap hole
            }
        }
    }
}

// ==============================================================
// MODULE: lid_screw_bosses
//   Four corner bosses on inside top rim for M3 lid screws.
// ==============================================================
module lid_screw_bosses() {
    boss_d = 8;
    boss_h = 6;
    offset = bx/2 - wall - 6;
    for (x = [-offset, offset])
    for (y = [-offset, offset]) {
        translate([x, y, bz - boss_h]) {
            difference() {
                cylinder(d = boss_d, h = boss_h);
                translate([0, 0, boss_h - 6])
                    cylinder(d = 2.5, h = 6.1);
            }
        }
    }
}

// ==============================================================
// MODULE: base_unit_body
//   Complete housing body with all cutouts applied.
// ==============================================================
module base_unit_body() {
    difference() {
        union() {
            main_box();
            // NMO socket boss centred on top face
            translate([0, 0, bz])
                nmo_socket();
            component_posts();
            lid_screw_bosses();
        }
        sma_cutout();
        banana_cutouts();
        led_cutouts();
        tripod_pocket();
        balun_bolt_holes();
    }
}

// ==============================================================
// MODULE: lid
//   Flat snap-fit lid with NMO socket clearance hole and four
//   corner screw holes.  Print separately, face-down on bed.
// ==============================================================
module lid() {
    difference() {
        union() {
            // Main lid plate
            hull() {
                r = 3;
                for (x = [-bx/2 + r, bx/2 - r])
                for (y = [-by/2 + r, by/2 - r]) {
                    translate([x, y, 0])
                        cylinder(r = r, h = lid_t);
                }
            }

            // Peripheral drop-in skirt (inside box walls)
            difference() {
                hull() {
                    r = 2;
                    skirt_w = bx - 2*wall - 1.2;
                    skirt_d = by - 2*wall - 1.2;
                    for (x = [-skirt_w/2 + r, skirt_w/2 - r])
                    for (y = [-skirt_d/2 + r, skirt_d/2 - r]) {
                        translate([x, y, -6])
                            cylinder(r = r, h = 6);
                    }
                }
                hull() {
                    r = 1.5;
                    inner_w = bx - 2*wall - 2.5;
                    inner_d = by - 2*wall - 2.5;
                    for (x = [-inner_w/2 + r, inner_w/2 - r])
                    for (y = [-inner_d/2 + r, inner_d/2 - r]) {
                        translate([x, y, -6.1])
                            cylinder(r = r, h = 6.2);
                    }
                }
            }
        }

        // NMO socket pass-through hole
        translate([0, 0, -0.1])
            cylinder(d = nmo_od + 1.0, h = lid_t + 0.2);

        // Four corner screw holes (through)
        offset = bx/2 - wall - 6;
        for (x = [-offset, offset])
        for (y = [-offset, offset]) {
            translate([x, y, -0.1])
                cylinder(d = lid_screw_d, h = lid_t + 0.2);
        }
    }
}

// ==============================================================
// MAIN RENDER
//   Body at origin; lid offset to side for print layout preview.
// ==============================================================

// Base unit body
base_unit_body();

// Lid — offset to the right, above bed, for layout preview
translate([bx + 10, 0, lid_t])
    rotate([180, 0, 0])
        lid();

// ── End of TM-WHP-ENC-002 ────────────────────────────────────
