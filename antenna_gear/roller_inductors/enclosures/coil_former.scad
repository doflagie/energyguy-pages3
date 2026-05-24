/**
 * coil_former.scad — TM-ROLLER-001 Rev A
 * Roller Inductor Coil Former with Helical Groove
 *
 * Print settings:
 *   Material:    PETG or PLA+ (PETG preferred for RF/thermal stability)
 *   Infill:      40% gyroid
 *   Wall:        3 perimeters
 *   Layer:       0.2mm
 *   Orientation: Vertical (Z-axis along former axis) for best groove accuracy
 *   Supports:    None required
 *
 * Print envelope: 64mm × 64mm × 180mm — fits in 200×200×200 bed
 *
 * Coil specifications:
 *   OD:     63.5mm (2.500 inch)
 *   ID:     57.5mm (wall = 3.0mm)
 *   Length: 180mm
 *   Groove: 2.54mm pitch, 30 turns, AWG 12 wire (2.05mm bare, 2.3mm with enamel)
 *   Wheeler: L_max ≈ 34 µH, L_min ≈ 2.4 µH (5 turns)
 */

// ─── Parameters ───────────────────────────────────────────────────────────────

former_od       = 63.5;  // Outer diameter of former (mm)
former_id       = 57.5;  // Inner diameter (wall = 3.0mm)
former_length   = 180.0; // Total former length
wall_thick      = 3.0;   // Wall thickness

// Wire and groove
wire_dia        = 2.05;  // AWG 12 bare copper, mm
enamel_clear    = 0.15;  // Enamel clearance per side
groove_depth    = 1.0;   // Groove depth (wire sits 50% in groove)
groove_rad      = (wire_dia / 2) + enamel_clear; // Groove radius
pitch           = 2.54;  // Turn-to-turn pitch (10 TPI = 2.54mm)

// Winding parameters
num_turns       = 30;
wind_start_z    = 40.0;  // Distance from bottom to first groove center
wind_length     = num_turns * pitch;  // 76.2mm

// Rail rod holes (for roller carriage guide rods)
rail_od         = 6.2;   // Rail rod diameter + 0.2mm clearance
rail_offset_x   = (former_od / 2) + 10;  // 10mm outside former OD
rail_offset_y   = 0;
rail_hole_len   = former_length;

// End flange
flange_thick    = 4.0;
flange_od       = former_od + 10;   // 73.5mm flange OD

// Wire exit holes
wire_exit_dia   = 3.5;   // Wire exit hole diameter

// Mounting holes
mount_screw_od  = 3.3;   // M3 clearance hole

// ─── Modules ──────────────────────────────────────────────────────────────────

module helical_groove(radius, pitch, turns, groove_r, steps_per_turn=120) {
    total_steps = round(turns * steps_per_turn);
    for (i = [0 : total_steps - 1]) {
        angle0 = i * 360 / steps_per_turn;
        angle1 = (i + 1) * 360 / steps_per_turn;
        z0 = i * pitch / steps_per_turn;
        z1 = (i + 1) * pitch / steps_per_turn;
        hull() {
            translate([radius * cos(angle0), radius * sin(angle0), z0])
                sphere(r = groove_r, $fn = 16);
            translate([radius * cos(angle1), radius * sin(angle1), z1])
                sphere(r = groove_r, $fn = 16);
        }
    }
}

module bottom_flange() {
    difference() {
        union() {
            cylinder(h = flange_thick, d = flange_od, $fn = 64);
            // Boss for bottom terminal connection
            translate([former_od/2 - 2, 0, flange_thick])
                cylinder(h = 4, d = 6, $fn = 16);
        }
        // Center bore
        cylinder(h = flange_thick + 1, d = former_id, $fn = 64);
        // Rail rod clearance holes
        for (side = [-1, 1]) {
            translate([rail_offset_x * side, 0, -0.5])
                cylinder(h = flange_thick + 1, d = rail_od, $fn = 16);
        }
        // Mounting screw holes (4×, on flange)
        for (a = [0, 90, 180, 270]) {
            rotate([0, 0, a])
            translate([(flange_od/2) - 4, 0, -0.5])
                cylinder(h = flange_thick + 1, d = mount_screw_od, $fn = 16);
        }
        // Wire exit hole (bottom terminal)
        translate([former_od/2 - 1, 0, -0.5])
            cylinder(h = flange_thick + 1, d = wire_exit_dia, $fn = 16);
    }
}

module top_flange() {
    difference() {
        union() {
            cylinder(h = flange_thick, d = flange_od, $fn = 64);
        }
        // Center bore
        cylinder(h = flange_thick + 1, d = former_id, $fn = 64);
        // Rail rod clearance holes
        for (side = [-1, 1]) {
            translate([rail_offset_x * side, 0, -0.5])
                cylinder(h = flange_thick + 1, d = rail_od, $fn = 16);
        }
        // Mounting screw holes
        for (a = [0, 90, 180, 270]) {
            rotate([0, 0, a])
            translate([(flange_od/2) - 4, 0, -0.5])
                cylinder(h = flange_thick + 1, d = mount_screw_od, $fn = 16);
        }
        // Top wire exit (fixed terminal for winding top end)
        translate([former_od/2 - 1, 0, -0.5])
            cylinder(h = flange_thick + 1, d = wire_exit_dia, $fn = 16);
    }
}

module former_tube() {
    difference() {
        // Main tube
        cylinder(h = former_length, d = former_od, $fn = 64);
        // Hollow bore
        cylinder(h = former_length + 1, d = former_id, $fn = 64);
        // Helical wire groove
        translate([0, 0, wind_start_z])
            helical_groove(
                radius        = former_od / 2,
                pitch         = pitch,
                turns         = num_turns,
                groove_r      = groove_rad
            );
        // Rail rod holes through tube (for carriage rod clearance)
        // Note: rods are outside the tube, but the rod-end supports pass through flanges
        // No through-holes in tube itself needed if rods run externally
    }
}

module position_tick_marks() {
    // Laser-engrave-style tick marks for every 5 turns
    // (visible notches on OD for counting)
    tick_turns = [5, 10, 15, 20, 25, 30];
    for (t = tick_turns) {
        tick_z = wind_start_z + (t - 0.5) * pitch;
        difference() {
            translate([0, 0, tick_z])
                rotate([0, 0, 0])
                translate([former_od/2 - 0.5, 0, 0])
                    cube([1.5, 2.0, 1.0], center = true);
        }
    }
}

// ─── Assembly ─────────────────────────────────────────────────────────────────

module coil_former_assembly() {
    union() {
        // Bottom flange
        bottom_flange();

        // Main tube
        translate([0, 0, flange_thick])
            former_tube();

        // Top flange
        translate([0, 0, flange_thick + former_length])
            top_flange();
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 64;
coil_former_assembly();

// ─── Verification Notes ───────────────────────────────────────────────────────
// Total printed height: flange_thick + former_length + flange_thick
//   = 4 + 180 + 4 = 188mm ← fits in 200mm Z
// OD: 63.5mm (tube) / 73.5mm (flange) ← fits in 200mm XY
//
// After printing:
// 1. Clean helical groove with wire brush
// 2. Wind AWG 12 silver-plated wire into groove, starting at bottom exit hole
// 3. Secure each end with terminal lug through exit hole
// 4. Coat wire with thin layer of epoxy resin (stabilizes turns)
// 5. Insert guide rods (6mm dia steel) through flange holes
