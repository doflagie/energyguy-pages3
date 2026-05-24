/**
 * cap_enclosure.scad — TM-VCAP-001 Rev A
 * Weatherproof Enclosure for Variable Capacitor and Drive System
 *
 * IP54 weatherproof (dust-resistant, splash-proof) when gasket-sealed.
 * Houses: variable capacitor, motor+gearbox, controller PCB, limit switches.
 * External: 2× RF connectors (SO-239 or N-type), DC power input, control cable.
 *
 * Print settings:
 *   Material:    PETG (UV and moisture resistant for outdoor use)
 *   Infill:      30% gyroid (walls carry load)
 *   Wall:        4 perimeters
 *   Layer:       0.2mm
 *   Orientation: Box open-face down, lid flat on bed separately
 *
 * Print envelope:
 *   Box body:  195mm × 140mm × 90mm ← fits in 200mm cube
 *   Lid:       195mm × 140mm × 12mm ← fits in 200mm cube
 *
 * HIGH VOLTAGE NOTE:
 *   The interior of this enclosure may contain components at RF potential
 *   of several kV during transmit. All exterior surfaces must be:
 *   - At chassis/safety ground OR
 *   - Clearly labeled "HV — DO NOT CONTACT DURING TRANSMIT"
 *   The controller PCB and motor must be isolated from RF-potential nodes.
 */

// ─── Parameters ───────────────────────────────────────────────────────────────

// Box dimensions
box_w           = 195.0;   // Width (X)
box_d           = 135.0;   // Depth (Y)
box_h           = 88.0;    // Height (Z) — enclosure body
lid_h           = 12.0;    // Lid height (with gasket channel)
wall            = 4.0;     // Wall thickness
floor_h         = 5.0;     // Floor thickness (extra for PCB standoff bosses)
corner_r        = 8.0;     // Corner radius

// Lid
gasket_groove_d = 3.0;     // Gasket cord diameter (3mm EPDM O-cord)
gasket_groove_r = gasket_groove_d * 0.7;  // Groove radius (70% cord dia)
lid_overlap     = 4.0;     // Lid overlap into box for alignment

// RF Connectors (SO-239 or N-type side walls)
so239_hole_d    = 16.0;    // SO-239 panel hole diameter
so239_bolt_sp   = 24.5;    // SO-239 4-bolt hole spacing (center-to-center diagonal)
n_type_hole_d   = 20.5;    // N-type panel hole diameter
n_type_bolt_sp  = 22.8;    // N-type bolt pattern (M3×4, on 22.8mm BCD)

rf_conn_type    = "SO239";  // "SO239" or "NTYPE"
rf_hole_d       = (rf_conn_type == "NTYPE") ? n_type_hole_d : so239_hole_d;
rf_bolt_sp      = (rf_conn_type == "NTYPE") ? n_type_bolt_sp : so239_bolt_sp;

// RF connector positions (left wall)
rf_in_y         = box_d / 3;
rf_out_y        = box_d * 2 / 3;
rf_z            = box_h / 2;

// DC power input (rear wall, right side)
dc_jack_d       = 12.0;    // 5.5/2.1mm barrel jack panel hole
dc_jack_x       = box_w / 2 - 20;
dc_jack_z       = 20.0;

// Cable gland (motor cable, rear)
gland_d         = 16.0;    // PG9 cable gland thread OD
gland_x         = -box_w / 2 + 30;
gland_z         = 20.0;

// PCB mounting standoffs
pcb_standoff_h  = 5.0;
pcb_standoff_od = 6.0;
pcb_standoff_id = 2.9;     // M3 thread or press-fit M3 insert
pcb_x           = 60.0;    // PCB center X offset from box center
pcb_y           = -20.0;   // PCB center Y
pcb_bolt_sp     = 50.0;    // PCB mounting hole spacing

// Capacitor mounting area
cap_center_x    = -15.0;   // Capacitor centered slightly left
cap_center_y    = 10.0;

// Motor mount area
mot_center_x    = 50.0;
mot_center_y    = cap_center_y;

// Vents (NO vents for weatherproof — use sealed design with desiccant)
// Optional vent: covered with Gore-Tex membrane for pressure equalization
vent_d          = 8.0;
vent_z          = box_h - 15;

// Lid fasteners (M4 screws)
lid_bolt_count  = 6;
lid_bolt_dia    = 4.3;
lid_bolt_head_d = 8.0;
lid_bolt_head_h = 4.0;
lid_boss_od     = 10.0;

// ─── Modules ──────────────────────────────────────────────────────────────────

module box_outline(w, d, h, r, t) {
    // Rounded rectangle tube
    hull() {
        for (x=[-1,1]) for (y=[-1,1])
            translate([x*(w/2-r), y*(d/2-r), 0])
                cylinder(h=h, r=r, $fn=32);
    }
}

module rf_connector_hole(x, y, z) {
    // Panel cutout for RF connector
    translate([-(box_w/2+0.5), y - box_d/2, z]) {
        rotate([0,90,0]) {
            cylinder(h=wall+1, d=rf_hole_d, $fn=32);
            // Bolt holes (4× M3)
            for (a=[45,135,225,315])
                rotate([0,0,a])
                translate([rf_bolt_sp/2, 0, 0])
                    cylinder(h=wall+1, d=3.3, $fn=16);
        }
    }
}

module gasket_groove(w, d) {
    // O-cord groove path in lid mating surface
    r_groove = corner_r - 3;
    offset = wall - 2;
    hull_translate = offset;
    translate([0, 0, box_h - gasket_groove_r * 2 - 0.5])
    difference() {
        hull() {
            for (x=[-1,1]) for (y=[-1,1])
                translate([x*(w/2-r_groove-hull_translate), y*(d/2-r_groove-hull_translate), 0])
                    cylinder(h=gasket_groove_r*2+1, r=r_groove, $fn=32);
        }
        hull() {
            for (x=[-1,1]) for (y=[-1,1])
                translate([x*(w/2-r_groove-hull_translate-gasket_groove_r*2),
                           y*(d/2-r_groove-hull_translate-gasket_groove_r*2), -0.5])
                    cylinder(h=gasket_groove_r*2+2, r=r_groove, $fn=32);
        }
    }
}

module pcb_standoff(x, y) {
    translate([x, y, floor_h]) {
        difference() {
            cylinder(h=pcb_standoff_h, d=pcb_standoff_od, $fn=16);
            translate([0,0,-0.5])
                cylinder(h=pcb_standoff_h+1, d=pcb_standoff_id, $fn=16);
        }
    }
}

module lid_boss(x, y) {
    translate([x, y, box_h - 0.5]) {
        difference() {
            cylinder(h=lid_h, d=lid_boss_od, $fn=16);
            translate([0,0,-0.5])
                cylinder(h=lid_h+1, d=lid_bolt_dia, $fn=16);
        }
    }
}

// ─── Box Body ─────────────────────────────────────────────────────────────────

module box_body() {
    difference() {
        // Outer shell
        box_outline(box_w, box_d, box_h, corner_r, wall);

        // Inner cavity
        translate([0, 0, floor_h])
            box_outline(box_w - 2*wall, box_d - 2*wall, box_h, corner_r-wall, wall);

        // RF IN connector (left wall)
        rf_connector_hole(0, rf_in_y - box_d/2, rf_z);

        // RF OUT connector (left wall)
        rf_connector_hole(0, rf_out_y - box_d/2, rf_z);

        // DC power jack (rear wall)
        translate([dc_jack_x, -(box_d/2+0.5), dc_jack_z])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=dc_jack_d, $fn=32);

        // Cable gland (rear wall)
        translate([gland_x, -(box_d/2+0.5), gland_z])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=gland_d, $fn=32);

        // Lid bolt holes (around perimeter)
        lid_bolt_x = box_w/2 - 12;
        lid_bolt_y = box_d/2 - 12;
        for (a=[0,1,2,3]) {
            xx = (a < 2) ? lid_bolt_x : -lid_bolt_x;
            yy_list = [lid_bolt_y, -lid_bolt_y, lid_bolt_y, -lid_bolt_y];
            translate([xx, yy_list[a], box_h - lid_bolt_head_h - 0.5]) {
                cylinder(h=lid_bolt_head_h+1, d=lid_bolt_head_d, $fn=16);
                translate([0,0,-box_h])
                    cylinder(h=box_h+2, d=lid_bolt_dia, $fn=16);
            }
        }
        // Extra 2 lid bolts on long sides (mid-span)
        for (side=[-1,1]) {
            translate([0, side*(box_d/2-8), box_h-lid_bolt_head_h-0.5]) {
                cylinder(h=lid_bolt_head_h+1, d=lid_bolt_head_d, $fn=16);
                translate([0,0,-box_h]) cylinder(h=box_h+2, d=lid_bolt_dia, $fn=16);
            }
        }
    }

    // PCB standoffs (4 corners of controller PCB)
    for (x=[-1,1]) for (y=[-1,1])
        pcb_standoff(pcb_x + x*pcb_bolt_sp/2, pcb_y + y*pcb_bolt_sp/2);

    // O-cord groove
    gasket_groove(box_w - 2, box_d - 2);
}

// ─── Lid ──────────────────────────────────────────────────────────────────────

module box_lid() {
    difference() {
        union() {
            // Main lid plate
            box_outline(box_w, box_d, lid_h - lid_overlap, corner_r, wall);
            // Alignment lip (fits inside box)
            translate([0, 0, lid_h - lid_overlap])
                box_outline(box_w - 2*wall - 0.5, box_d - 2*wall - 0.5, lid_overlap, corner_r-wall, wall);
        }
        // Lid bolt clearance holes
        lid_bolt_x = box_w/2 - 12;
        lid_bolt_y = box_d/2 - 12;
        for (a=[0,1,2,3]) {
            xx = (a < 2) ? lid_bolt_x : -lid_bolt_x;
            yy_list = [lid_bolt_y, -lid_bolt_y, lid_bolt_y, -lid_bolt_y];
            translate([xx, yy_list[a], -0.5])
                cylinder(h=lid_h+1, d=lid_bolt_dia, $fn=16);
        }
        for (side=[-1,1]) {
            translate([0, side*(box_d/2-8), -0.5])
                cylinder(h=lid_h+1, d=lid_bolt_dia, $fn=16);
        }
        // Humidity indicator window (optional — Cobalt chloride card visible from outside)
        translate([box_w/2 - 20, -box_d/2 + 15, -0.5])
            cylinder(h=lid_h+1, d=10, $fn=32);
    }
}

// ─── Label Plate Recess ───────────────────────────────────────────────────────

module label_recess() {
    // Recessed area on lid for adhesive label
    translate([0, 10, lid_h - 1.5])
    hull() {
        for (x=[-1,1]) for (y=[-1,1])
            translate([x*60, y*20, 0])
                cylinder(h=2, r=4, $fn=16);
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 64;

// Box body
color("lightgray", 0.8)
box_body();

// Lid (shown offset above for visualization)
color("lightgray", 0.6)
translate([0, 0, box_h + 10])
    box_lid();

// ─── Notes ────────────────────────────────────────────────────────────────────
// WEATHERPROOFING:
//   1. Print both body and lid in PETG (UV resistant, low moisture uptake)
//   2. Install 3mm EPDM O-cord in gasket groove (Trim-Lok 3842 or McMaster 5233K72)
//      Measure groove path, cut cord, join ends with cyanoacrylate
//   3. Cable gland: PG9 nylon cable gland with EPDM insert (max 9mm cable OD)
//   4. DC jack: sealed barrel jack (Amphenol PPPK-02P or equiv)
//   5. RF connectors: SO-239 or N-type with Teflon seal rings
//   6. Add 5g silica gel desiccant inside enclosure; replace annually
//   7. Seal all interior wire feedthroughs with RTV silicone
//
// INTERIOR LAYOUT:
//   Left side:  Variable capacitor + mounting bracket
//   Center:     Drive coupling (insulating coupler)
//   Right side: Motor + gearbox
//   Far right:  Controller PCB (on standoffs)
//   Floor rear: Limit switches, pot mount bracket
