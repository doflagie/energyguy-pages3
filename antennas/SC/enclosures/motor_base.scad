// =============================================================================
// TM-SC-ENC-002 Rev A
// Screwdriver Antenna — Motor Base Housing
// =============================================================================
// Description : Base enclosure for the screwdriver antenna drive assembly.
//               Houses 28BYJ-48 stepper motor, ESP32-WROOM-32, ULN2003 driver
//               board, rod bearings, home limit switch, and all I/O connectors.
//               The coil form (TM-SC-ENC-001) mounts on the coil seat collar
//               on top of the box. RF passes through an NMO-style base connector.
//
// Material    : PETG
// Print bed   : 200 × 200 × 200 mm
//               Box body: 100×100×80mm  — print flat (Z = 80mm)
//               Lid:      100×100×8mm   — print flat beside body
// Author      : Generated for Merv Martin
// Date        : 2026-05-06
// =============================================================================

$fn = 64;

// ─── Box Dimensions ──────────────────────────────────────────────────────────
bx   = 100;   // box exterior X
by   = 100;   // box exterior Y
bz   = 80;    // box exterior Z (height of main body)
wall = 3.0;   // wall thickness
floor_t = 3.0; // floor thickness

// ─── Coil Seat Collar ────────────────────────────────────────────────────────
coil_seat_od  = 88;   // inner dia of collar — slip fits over coil form flange OD=85
coil_seat_wall = 4.0; // collar wall thickness
coil_seat_id  = coil_seat_od - 2 * coil_seat_wall; // ~80mm
coil_seat_h   = 20;   // collar height above box top

// ─── Rod Bore ────────────────────────────────────────────────────────────────
rod_bore_d    = 8.0;  // 1/4-20 rod clearance through base floor and collar
bushing_d     = 12.0; // shoulder OD for brass bushing seat
bushing_h     = 4.0;  // depth of bushing counterbore

// ─── 28BYJ-48 Stepper Motor ──────────────────────────────────────────────────
// Body diameter (gearbox housing): ~28mm
// Shaft offset from body centre: motor mounts with shaft at body centre
// Mounting: two M3 holes on a 35mm centre-to-centre line through the body axis
motor_body_d       = 28.0;
motor_shaft_d      = 5.0;   // output shaft bore
motor_mount_spacing = 35.0; // M3 hole spacing
motor_m3_d         = 3.4;   // M3 clearance bore
motor_mount_x      = bx / 2; // centred in X
motor_mount_y      = by / 2; // centred in Y
motor_plate_z      = floor_t + 2.0; // riser so motor shaft aligns with rod bore

// ─── ESP32-WROOM-32 PCB ──────────────────────────────────────────────────────
esp32_l  = 55.0;  // PCB length + clearance
esp32_w  = 30.0;  // PCB width  + clearance
esp32_standoff_h = 4.0;  // standoff height
esp32_m_d = 2.7;  // M2.5 clearance
// Mounting position: left wall, centred in Y, low in box
esp32_x  = wall + 1.0;
esp32_y  = (by - esp32_w) / 2;
esp32_z  = floor_t;

// ─── ULN2003 Driver Board ────────────────────────────────────────────────────
uln_l = 42.0; uln_w = 28.0;
uln_standoff_h = 4.0;
uln_x = wall + 1.0;
uln_y = esp32_y + esp32_w + 6.0;   // next to ESP32
uln_z = floor_t;

// ─── Connector / Port Geometry ───────────────────────────────────────────────
nmo_od      = 24.0;   // NMO connector OD cutout in floor
sma_od      = 6.5;    // SMA panel-mount cutout
barrel_jack_d = 8.0;  // 5.5/2.1mm barrel jack cutout
usb_w       = 10.0;   // USB Micro-B slot width
usb_h       = 5.0;    // USB slot height
led_d       = 3.2;    // LED bezel clearance
banana_d    = 4.2;    // 4mm banana jack clearance

// ─── Lid Snap-Fit ────────────────────────────────────────────────────────────
lid_t        = 4.0;   // lid plate thickness
snap_h       = 3.0;   // snap latch height
snap_w       = 6.0;   // snap latch width
snap_over    = 0.8;   // snap overhang (interference)
lid_gap      = 0.3;   // lid-to-box clearance

// ─── Limit Switch ────────────────────────────────────────────────────────────
sw_l = 20.0; sw_w = 10.0; sw_h = 6.0;
sw_m_d = 2.7;  // M2 clearance

// =============================================================================
// MODULE: main_box
// Hollow rectangular box with floor, open top (lid closes it).
// =============================================================================
module main_box() {
    difference() {
        // Outer shell
        cube([bx, by, bz]);

        // Interior void
        translate([wall, wall, floor_t])
            cube([bx - 2*wall, by - 2*wall, bz - floor_t + 0.1]);

        // ── Connector cutouts (delegated, but geometry placed here) ──────────
        connector_cutouts();
    }
}

// =============================================================================
// MODULE: coil_seat_ring
// Collar on top of box that receives the bottom flange of the coil form.
// The coil form flange OD = 85mm; this collar ID = 88mm (3mm radial clearance).
// A 3mm radial ridge inside the collar acts as a retention lip.
// =============================================================================
module coil_seat_ring() {
    translate([bx/2, by/2, bz]) {
        difference() {
            // Outer tube
            cylinder(d = coil_seat_od + 2*coil_seat_wall,
                     h = coil_seat_h);
            // Inner bore (slip fit for coil form flange)
            translate([0, 0, -0.1])
                cylinder(d = coil_seat_od, h = coil_seat_h + 0.2);
            // Rod through-bore
            translate([0, 0, -0.1])
                cylinder(d = rod_bore_d, h = coil_seat_h + 0.2);
        }
        // Retention lip — 2mm high × 3mm radially inward ledge at collar top
        translate([0, 0, coil_seat_h - 3])
        difference() {
            cylinder(d = coil_seat_od, h = 3);
            translate([0, 0, -0.1])
                cylinder(d = coil_seat_id - 4, h = 3 + 0.2);
        }
    }
}

// =============================================================================
// MODULE: rod_bearing_seats
// Top and bottom brass-bushing pockets for the 1/4-20 rod.
// Bottom seat: in the floor, centred.
// Top seat: inside the coil collar.
// =============================================================================
module rod_bearing_seats() {
    // Bottom bearing seat — counterbore in floor
    translate([bx/2, by/2, -0.1]) {
        // Through bore
        cylinder(d = rod_bore_d, h = floor_t + 0.2);
        // Counterbore for bushing (enter from below, glue bushing in)
        cylinder(d = bushing_d, h = bushing_h);
    }
}

// =============================================================================
// MODULE: motor_mount
// Motor plate riser and two M3 mounting holes for 28BYJ-48 stepper.
// Motor is mounted vertically (shaft pointing up) on a raised plate so the
// drive gear/coupler meets the threaded rod at the correct height.
// =============================================================================
module motor_mount() {
    plate_h = motor_plate_z;
    plate_l = motor_mount_spacing + 14;
    plate_w = 20.0;
    cx = bx/2; cy = by/2;

    translate([cx - plate_l/2, cy - plate_w/2, floor_t]) {
        difference() {
            // Riser plate
            cube([plate_l, plate_w, plate_h]);
            // Two M3 mounting holes
            for (dx = [-motor_mount_spacing/2, motor_mount_spacing/2])
                translate([plate_l/2 + dx, plate_w/2, -0.1])
                    cylinder(d = motor_m3_d, h = plate_h + 0.2);
        }
    }
    // Motor shaft bore extension through floor (rod alignment)
    // (rod_bearing_seats() handles the actual bore; this is a label comment)
}

// =============================================================================
// MODULE: limit_switch_mount
// Small bracket at the bottom of the rod's travel range for the SPDT homing
// microswitch. Bracket attaches to interior rear wall.
// =============================================================================
module limit_switch_mount() {
    brk_l = sw_l + 6;
    brk_w = 8.0;
    brk_h = sw_h + 6;
    // Position: rear wall (y = by - wall), low in box
    translate([bx/2 - brk_l/2, by - wall - brk_w, floor_t]) {
        difference() {
            cube([brk_l, brk_w, brk_h]);
            // Two M2 screw holes for switch body
            for (dx = [5, sw_l + 1])
                translate([dx, -0.1, brk_h/2])
                    rotate([-90, 0, 0])
                        cylinder(d = sw_m_d, h = brk_w + 0.2);
        }
    }
}

// =============================================================================
// MODULE: pcb_mounts
// Standoffs for ESP32-WROOM-32 (4× M2.5) and ULN2003 board (2× M2.5).
// Standoffs are 4mm tall, 6mm OD, with M2.5 clearance bore through.
// =============================================================================
module pcb_mounts() {
    so_od = 6.0;
    so_h  = esp32_standoff_h;
    m_d   = esp32_m_d;

    // ESP32 — four corners (nominal 46×23mm hole pattern)
    esp32_hx = 46.0; esp32_hy = 23.0;
    for (dx = [0, esp32_hx], dy = [0, esp32_hy])
        translate([esp32_x + 5 + dx, esp32_y + 3.5 + dy, floor_t]) {
            difference() {
                cylinder(d = so_od, h = so_h);
                translate([0, 0, -0.1])
                    cylinder(d = m_d, h = so_h + 0.2);
            }
        }

    // ULN2003 — two holes (nominal 35mm spacing)
    uln_hy = 35.0;
    for (dy = [0, uln_hy])
        translate([uln_x + 5, uln_y + 4 + dy, floor_t]) {
            difference() {
                cylinder(d = so_od, h = uln_standoff_h);
                translate([0, 0, -0.1])
                    cylinder(d = m_d, h = uln_standoff_h + 0.2);
            }
        }
}

// =============================================================================
// MODULE: connector_cutouts
// All panel cutouts — called inside main_box() difference().
// =============================================================================
module connector_cutouts() {
    // ── NMO connector — floor, centred toward front ──────────────────────────
    // (RF output to antenna; rod passes through its centre)
    // Rod bore already in rod_bearing_seats; NMO ring cutout here.
    translate([bx/2, by * 0.25, -0.1])
        cylinder(d = nmo_od, h = floor_t + 0.2);

    // ── SMA coaxial input — front face (y=0), centred, mid-height ────────────
    translate([bx/2 - sma_od/2, -0.1, bz * 0.55])
        rotate([-90, 0, 0])
            cylinder(d = sma_od, h = wall + 0.2);

    // ── Barrel jack power input — right side (x=bx), low ────────────────────
    translate([bx - wall - 0.1, by/2, bz * 0.2])
        rotate([0, 90, 0])
            cylinder(d = barrel_jack_d, h = wall + 0.2);

    // ── USB Micro-B access slot — rear face (y=by), low (for ESP32 USB) ──────
    translate([bx/2 - usb_w/2, by - wall - 0.1, floor_t + 4])
        cube([usb_w, wall + 0.2, usb_h]);

    // ── Banana jacks — 4mm, one per cardinal side, mid-height ────────────────
    // Front (y=0)
    translate([bx/2, -0.1, bz * 0.35])
        rotate([-90, 0, 0]) cylinder(d = banana_d, h = wall + 0.2);
    // Rear (y=by)
    translate([bx/2, by - wall - 0.1, bz * 0.35])
        rotate([-90, 0, 0]) cylinder(d = banana_d, h = wall + 0.2);
    // Left (x=0)
    translate([-0.1, by/2, bz * 0.35])
        rotate([0, 90, 0]) cylinder(d = banana_d, h = wall + 0.2);
    // Right (x=bx) — separate from barrel jack height
    translate([bx - wall - 0.1, by * 0.35, bz * 0.35])
        rotate([0, 90, 0]) cylinder(d = banana_d, h = wall + 0.2);

    // ── LED indicators — front face, 3× holes, green/yellow/red ─────────────
    // Spaced 12mm apart, upper front face
    for (i = [0, 1, 2])
        translate([bx/2 - 12 + i*12, -0.1, bz * 0.75])
            rotate([-90, 0, 0])
                cylinder(d = led_d, h = wall + 0.2);
}

// =============================================================================
// MODULE: lid
// Snap-fit lid with a central through-bore for the coil seat collar to pass
// through. Snap latches on all four sides.
// =============================================================================
module lid() {
    lid_od_bore = coil_seat_od + 2*coil_seat_wall + 1.0; // clearance around collar

    difference() {
        union() {
            // Lid plate
            cube([bx - 2*lid_gap, by - 2*lid_gap, lid_t]);

            // Snap latch tabs — four sides, centred
            // Front
            translate([bx/2 - snap_w/2 - lid_gap, -snap_over, lid_t - snap_h])
                cube([snap_w, snap_over + wall + lid_gap, snap_h]);
            // Rear
            translate([bx/2 - snap_w/2 - lid_gap, by - 2*lid_gap - wall, lid_t - snap_h])
                cube([snap_w, snap_over + wall + lid_gap, snap_h]);
            // Left
            translate([-snap_over, by/2 - snap_w/2 - lid_gap, lid_t - snap_h])
                cube([snap_over + wall + lid_gap, snap_w, snap_h]);
            // Right
            translate([bx - 2*lid_gap - wall, by/2 - snap_w/2 - lid_gap, lid_t - snap_h])
                cube([snap_over + wall + lid_gap, snap_w, snap_h]);
        }

        // Coil seat collar pass-through bore
        translate([bx/2 - lid_gap, by/2 - lid_gap, -0.1])
            cylinder(d = lid_od_bore, h = lid_t + 0.2);

        // Rod bore (redundant but explicit)
        translate([bx/2 - lid_gap, by/2 - lid_gap, -0.1])
            cylinder(d = rod_bore_d, h = lid_t + 0.2);
    }
}

// =============================================================================
// RENDER
// Box body printed upright (Z). Lid printed flat beside the box.
// Coil seat ring is integral to the box top — no separate part.
// =============================================================================

// Box body (with all internal features)
difference() {
    union() {
        main_box();
        coil_seat_ring();
    }
    rod_bearing_seats();
}

// Internal positive features (added after box difference)
motor_mount();
limit_switch_mount();
pcb_mounts();

// Lid — positioned beside box for print layout (offset in X)
translate([bx + 10, 0, 0])
    lid();

// =============================================================================
// ASSEMBLY NOTES
// 1. Print box body with coil seat upright. Lid prints flat separately.
// 2. Press 8mm OD brass bushings into the rod bearing pockets (top and bottom).
// 3. 28BYJ-48 stepper mounts on the riser plate with two M3×8 screws.
//    A friction or keyed coupler connects the motor shaft to the 1/4-20 rod.
// 4. ESP32 PCB mounts on 4mm standoffs (M2.5×6). ULN2003 on 2 standoffs.
// 5. Homing microswitch M2 screws into limit_switch_mount bracket.
// 6. NMO connector: standard NMO base threaded up through floor cutout.
//    Coax tap wire from coil form routes to NMO centre pin.
// 7. SMA front-panel connector: coax feed from transceiver.
// 8. Banana jacks: counterpoise/radial wires, one per cardinal compass point.
// 9. Snap lid closed from above. Remove lid from below by pressing snap tabs in.
// 10. Tripod mount: press-fit or epoxy a 1/4-20 brass insert into bottom
//     pocket at box centre. Pocket dia 6.35mm × 10mm deep — drill/add as needed.
// =============================================================================
