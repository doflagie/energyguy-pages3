/*
 * base_plates_mounts.scad
 * TM-MAST-001 Rev A — Mast Base Plates and Mounting Hardware
 *
 * 3D-printable base and mounting hardware for portable antenna masts:
 *   - Ground spike base plate (soil installation)
 *   - Surface base plate (concrete / hard surface)
 *   - Vehicle hitch mast mount (2" receiver)
 *   - Tripod foot pad (non-slip, telescoping leg end)
 *   - Tilt-over hinge cap (pivot protection)
 *   - Rotator tower top plate adapter
 *   - Crank-up tower base pivot collar
 *   - Ballast base plate frame (for sandbag weighting)
 *   - Mast plumb leveling wedge set
 *
 * Material: PETG; 5 perimeters, 50% infill (gyroid)
 * Note: Metal base plates for structural applications — prints are adapters/caps.
 * Units: mm
 */

$fn = 60;

WALL = 3.0;
M4_C = 4.4;
M6_C = 6.4;
M8_C = 8.5;
M10_C = 10.5;
M12_C = 13.0;
TOL  = 0.3;

// =============================================================================
// MODULE 1: GROUND SPIKE BASE PLATE
// =============================================================================
// Adapts mast tube base to a driven ground spike.
// Metal spike (20mm × 600mm steel rod) driven into soil.
// Printed collar clamps mast to spike; allows tilt-over operation.
// =============================================================================
module ground_spike_collar(
    mast_od   = 63,   // Mast base section outer diameter
    spike_od  = 20,   // Ground spike rod diameter
    collar_h  = 80,   // Total collar height
    plate_w   = 120   // Base flange width
) {
    collar_t = WALL + 4;
    flange_t = 6;

    difference() {
        union() {
            // Upper mast socket
            cylinder(d = mast_od + 2 * collar_t, h = collar_h * 0.6);

            // Lower spike socket (smaller; spike inserts upward)
            cylinder(d = spike_od + 2 * collar_t, h = collar_h);

            // Base flange
            translate([-(plate_w/2), -(plate_w/2), 0])
                cube([plate_w, plate_w, flange_t]);
        }

        // Mast bore (upper)
        translate([0, 0, collar_h * 0.4])
            cylinder(d = mast_od + TOL * 2, h = collar_h);

        // Spike bore (lower — through entire collar)
        cylinder(d = spike_od + TOL * 2, h = collar_h + 2);

        // Mast split cut (clamp action)
        translate([-2, -(mast_od/2 + collar_t + 2), collar_h * 0.4 - 1])
            cube([4, mast_od + 2 * collar_t + 4, collar_h]);

        // Mast clamp bolt holes (M8, 2× per side)
        for (z = [collar_h * 0.45, collar_h * 0.52]) {
            translate([0, 0, z])
                rotate([0, 90, 0])
                    cylinder(d = M8_C, h = mast_od + 2 * collar_t + 2, center = true);
        }

        // Flange anchor holes (4× M12, corners)
        for (x = [-plate_w/2 + 20, plate_w/2 - 20])
            for (y = [-plate_w/2 + 20, plate_w/2 - 20])
                translate([x, y, -1]) cylinder(d = M12_C, h = flange_t + 2);
    }
}

// =============================================================================
// MODULE 2: SURFACE MOUNT BASE PLATE
// =============================================================================
// Flat base for hard surfaces. Accepts Tapcon screws or dead-weight loading.
// Central socket receives mast base section.
// =============================================================================
module surface_base_plate(
    mast_od  = 63,
    plate_w  = 200,   // Plate width (200mm fits print bed)
    plate_h  = 180,   // Plate depth
    plate_t  = 8,     // Plate thickness
    socket_h = 60     // Mast socket height above plate
) {
    socket_t = WALL + 4;

    difference() {
        union() {
            // Flat base plate
            cube([plate_w, plate_h, plate_t]);

            // Mast socket (central)
            translate([plate_w/2, plate_h/2, plate_t])
                cylinder(d = mast_od + 2 * socket_t, h = socket_h);

            // Gusset ribs from socket to corners
            for (ang = [45, 135, 225, 315]) {
                rotate([0, 0, ang]) {
                    translate([plate_w/2, plate_h/2, plate_t/2]) // Approximate
                        cube([plate_w * 0.5, socket_t, plate_t], center = true);
                }
            }
        }

        // Mast bore
        translate([plate_w/2, plate_h/2, plate_t - 1])
            cylinder(d = mast_od + TOL * 2, h = socket_h + 2);

        // Concrete anchor holes (6×, perimeter)
        for (pos = [[20, 20], [plate_w-20, 20], [plate_w-20, plate_h-20],
                    [20, plate_h-20], [plate_w/2, 20], [plate_w/2, plate_h-20]]) {
            translate([pos[0], pos[1], -1]) cylinder(d = M10_C, h = plate_t + 2);
            // Countersink
            translate([pos[0], pos[1], plate_t - 3]) cylinder(d = 20, h = 3 + 1);
        }

        // Mast locking bolt (through socket wall)
        translate([plate_w/2, plate_h/2, plate_t + socket_h * 0.7])
            rotate([0, 90, 0])
                cylinder(d = M8_C, h = mast_od + 2 * socket_t + 2, center = true);
    }
}

// =============================================================================
// MODULE 3: VEHICLE HITCH MAST MOUNT
// =============================================================================
// Inserts into standard 2" Class III tow hitch receiver.
// Mast socket on top; rated for masts up to 25 ft.
// =============================================================================
module hitch_mast_mount(
    mast_od     = 50,
    hitch_size  = 50.8,   // 2" = 50.8mm (1.25" = 31.75mm)
    insert_l    = 150,    // Insert depth into receiver
    mast_h      = 120     // Mast socket height above hitch
) {
    insert_t = WALL + 3;
    mast_t   = WALL + 4;

    difference() {
        union() {
            // Hitch receiver insert (square cross-section)
            cube([hitch_size, hitch_size, insert_l]);

            // Vertical mast socket on top
            translate([hitch_size/2, hitch_size/2, insert_l])
                cylinder(d = mast_od + 2 * mast_t, h = mast_h);
        }

        // Hollow insert (receiver pin hole + weight reduction)
        translate([insert_t, insert_t, insert_t])
            cube([hitch_size - 2*insert_t, hitch_size - 2*insert_t, insert_l + mast_h]);

        // Receiver pin hole (1/2" / 12.7mm, 50mm from end)
        translate([-1, hitch_size/2, insert_l - 50])
            rotate([0, 90, 0]) cylinder(d = 13, h = hitch_size + 2);

        // Mast socket bore
        translate([hitch_size/2, hitch_size/2, insert_l - 1])
            cylinder(d = mast_od + TOL * 2, h = mast_h + 2);

        // Mast clamp bolt (M8, 2/3 up socket)
        translate([hitch_size/2, hitch_size/2, insert_l + mast_h * 0.65])
            rotate([0, 90, 0])
                cylinder(d = M8_C, h = mast_od + 2 * mast_t + 2, center = true);
    }

    // Anti-pivot pin boss (prevents insert rotating in receiver)
    translate([0, -10, insert_l - 60]) {
        difference() {
            cube([hitch_size, 10, 25]);
            // Through hole for pin
            translate([hitch_size/2, -1, 12.5])
                rotate([-90, 0, 0]) cylinder(d = 10, h = 12);
        }
    }
}

// =============================================================================
// MODULE 4: TRIPOD FOOT PAD
// =============================================================================
// Non-slip foot for telescoping tripod leg ends.
// Accepts standard 50mm OD leg tube.
// =============================================================================
module tripod_foot_pad(
    leg_od = 50,
    pad_d  = 80,   // Foot pad diameter
    pad_t  = 12,   // Pad thickness
    nub_h  = 4     // Ground-contact nub height
) {
    leg_t = WALL;

    difference() {
        union() {
            // Foot pad disc
            cylinder(d = pad_d, h = pad_t);

            // Leg socket (above pad)
            translate([0, 0, pad_t])
                cylinder(d = leg_od + 2 * leg_t, h = 30);
        }

        // Leg bore
        translate([0, 0, pad_t - 1])
            cylinder(d = leg_od + TOL * 2, h = 32);

        // Anti-rotation slot (set screw from side)
        translate([leg_od/2 + leg_t - 2, 0, pad_t + 15])
            rotate([0, 90, 0]) cylinder(d = M4_C, h = leg_t + 4);
    }

    // Non-slip ground nubs (concentric ring of nubs)
    for (i = [0:5]) {
        rotate([0, 0, i * 60]) translate([pad_d/2 - 10, 0, 0])
            cylinder(d = 6, h = pad_t + nub_h);
    }

    // Center ground nub
    cylinder(d = 8, h = pad_t + nub_h);
}

// =============================================================================
// MODULE 5: TILT-OVER HINGE PIN CAP
// =============================================================================
// Protective cover over tilt-over hinge pivot pin.
// Keeps corrosion, grit, and water off the bearing surface.
// Snaps over standard weld-on hinge (80mm × 80mm leaf, 16mm pin).
// =============================================================================
module tiltover_hinge_cap(
    pin_d    = 16,   // Hinge pin diameter
    leaf_w   = 80,   // Hinge leaf width
    cap_t    = 3
) {
    cap_h = pin_d + 2 * cap_t + 6;
    cap_w = leaf_w + 2 * cap_t;

    difference() {
        // Cap body
        cube([cap_w, cap_t * 2 + pin_d + 4, cap_h]);

        // Hinge leaf slots (allow leaves to pass through)
        for (z = [cap_t, cap_h - cap_t - 4]) {
            translate([-1, -1, z]) cube([cap_w + 2, cap_t + 3, 4]);
        }

        // Pin bore
        translate([cap_w/2, cap_t + pin_d/2 + 2, -1])
            cylinder(d = pin_d + TOL * 2, h = cap_h + 2);
    }
}

// =============================================================================
// MODULE 6: ROTATOR TOWER TOP PLATE ADAPTER
// =============================================================================
// Adapts rotator U-bolt mounting to a round mast tube top.
// Bolts to top plate on crank-up tower Section C; rotator straps to this plate.
// =============================================================================
module rotator_tower_topplate(
    plate_w    = 150,
    plate_h    = 150,
    plate_t    = 8,
    mast_stub_od = 38,    // Stub mast OD above rotator
    rotator_ubolt_span = 75,   // U-bolt spacing for rotator
    rotator_ubolt_d    = 8
) {
    stub_t = WALL + 3;

    difference() {
        union() {
            // Plate
            cube([plate_w, plate_h, plate_t]);

            // Mast stub socket (center)
            translate([plate_w/2, plate_h/2, plate_t])
                cylinder(d = mast_stub_od + 2 * stub_t, h = 50);
        }

        // Mast stub bore
        translate([plate_w/2, plate_h/2, plate_t - 1])
            cylinder(d = mast_stub_od + TOL * 2, h = 53);

        // Rotator U-bolt saddle slots
        for (x = [-rotator_ubolt_span/2, rotator_ubolt_span/2]) {
            translate([plate_w/2 + x, plate_h/2 - 40, -1])
                cube([rotator_ubolt_d + 2, 80, plate_t + 2]);
        }

        // Rotator U-bolt nut pockets (bottom face)
        for (x = [-rotator_ubolt_span/2, rotator_ubolt_span/2]) {
            for (y = [plate_h/2 - 25, plate_h/2 + 25]) {
                translate([plate_w/2 + x, y, -1])
                    cylinder(d = 14, h = 5, $fn = 6);
            }
        }

        // Tower attachment holes (M10, 4× corners)
        for (cx = [25, plate_w - 25]) for (cy = [25, plate_h - 25]) {
            translate([cx, cy, -1]) cylinder(d = M10_C, h = plate_t + 2);
        }
    }
}

// =============================================================================
// MODULE 7: MAST PLUMB LEVELING WEDGE
// =============================================================================
// Set of graduated wedges for leveling mast base on uneven terrain.
// Stack combinations to achieve 0.5°–5° correction.
// =============================================================================
module plumb_wedge(
    angle_deg = 2.0,    // Correction angle in degrees
    base_w    = 80,
    base_d    = 80,
    center_hole_d = 65  // Mast base OD + clearance
) {
    H_thick = base_w * tan(angle_deg);  // Max thickness of wedge

    difference() {
        // Wedge body — linear taper along Y
        hull() {
            cube([base_w, base_d, 2]);                    // Thin end (2mm min)
            translate([0, 0, H_thick]) cube([base_w, 2, 2]);  // Thick end
        }

        // Center bore (mast passes through)
        translate([base_w/2, base_d/2, -1])
            cylinder(d = center_hole_d, h = H_thick + 4);

        // Grip slots (easier to remove under loaded mast)
        for (x = [10, base_w - 10]) {
            translate([x - 5, base_d * 0.3, -1])
                cube([10, base_d * 0.4, H_thick + 4]);
        }

        // Angle label recess
        translate([base_w/2 - 8, base_d - 5, H_thick - 0.4])
            cube([16, 3, 0.5]);
    }
}

// =============================================================================
// RENDER CONTROLS
// =============================================================================

ground_spike_collar(mast_od=63, spike_od=20);
// translate([0, 170, 0]) surface_base_plate(mast_od=50);
// translate([250, 0, 0]) hitch_mast_mount(mast_od=50);
// translate([0, 300, 0]) { for (i=[0:2]) translate([i*90, 0, 0]) tripod_foot_pad(); }
// translate([200, 150, 0]) tiltover_hinge_cap();
// translate([0, 380, 0]) rotator_tower_topplate();
// translate([200, 0, 0]) { for (i=[0:3]) translate([i*90, 0, 0]) plumb_wedge(angle_deg=i*0.5+0.5); }
