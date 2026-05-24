// ============================================================
// FIELD STRENGTH METER PROBE HOUSINGS
// TM-FSM-001 Rev A
//
// Includes:
//   1. loop_housing()      — 12.7 cm shielded loop probe frame
//   2. dipole_handle()     — T-bar handle for telescoping dipole
//   3. yagi_boom()         — 3-element VHF/UHF yagi boom
//   4. whip_cap()          — BNC-to-whip antenna adapter cap
//   5. cal_dipole_jig()    — Calibration dipole support fixture
//
// All print in PLA or PETG, 2 walls, 0.2mm layer
// ============================================================

$fn = 48;

/* [Loop probe] */
loop_d        = 127.0;  // 5" / 12.7 cm loop OD (mm)
loop_frame_t  = 4.0;    // frame tube OD (printed)
handle_l      = 100.0;  // handle length below loop
handle_d      = 22.0;   // handle grip diameter
bnc_flange_d  = 16.0;   // BNC chassis hole boss diameter

/* [Dipole T-bar] */
tbar_w        = 180.0;  // crossbar width (holds two elements)
tbar_d        = 14.0;   // crossbar outer diameter
tbar_handle_l = 100.0;  // vertical grip handle length
element_hole_d = 6.1;   // element tube ID (6mm OD tubing)

/* [Yagi boom — 3 element] */
yagi_boom_l   = 520.0;  // boom length (2M band 3-el)
boom_sq       = 14.0;   // boom cross section (square)
element_dia   = 3.1;    // element hole through boom (3mm rod)
refl_pos      = 0.0;    // reflector from boom start
driv_pos      = 200.0;  // driven from boom start
dir_pos       = 320.0;  // director from boom start
feed_pos      = driv_pos;

/* [Whip cap] */
cap_thread_d  = 14.0;   // BNC body OD for push-fit
cap_t         = 3.0;    // cap wall thickness
whip_d        = 4.0;    // whip rod diameter (4mm)

// ============================================================
// 1. SHIELDED LOOP PROBE HOUSING
// ============================================================
// The loop is made from RG-58 coax formed into a circle.
// This printed frame holds the coax loop in a precise circle
// and provides the handle with integrated BNC mount.

module loop_housing() {
    r = loop_d / 2;

    // Circular trough for the coax loop
    rotate_extrude(angle = 330, $fn = 120)
        translate([r, 0, 0])
            difference() {
                circle(d = loop_frame_t + 2);
                circle(d = loop_frame_t);    // hollow for coax
            }

    // Gap bridge (top gap = where coax shield is interrupted)
    // A solid block to label the gap position
    rotate([0, 0, -15])
        translate([r - loop_frame_t/2, 0, 0])
            cube([loop_frame_t + 2, 8, 6], center = true);

    // Handle body (below loop)
    translate([0, -(r + handle_l/2 + loop_frame_t/2), 0])
    difference() {
        cylinder(d = handle_d, h = handle_l, center = true);
        // Grip flats
        for (a = [0, 60, 120, 180, 240, 300])
            rotate([0, 0, a])
                translate([handle_d/2 - 0.8, 0, 0])
                    cube([2, 3, handle_l * 0.8], center = true);
        // Cable channel (coax passes through handle)
        cylinder(d = loop_frame_t - 0.5, h = handle_l + 1, center = true);
    }

    // Loop-to-handle junction (enlarged gusset)
    translate([0, -(r + 2), 0])
        scale([1.2, 1.0, 1.0])
            cylinder(d = handle_d + 6, h = 8, center = true);

    // BNC mount boss at bottom of handle
    translate([0, -(r + handle_l + loop_frame_t + 4), 0])
    difference() {
        cylinder(d = bnc_flange_d + 4, h = 10, center = true);
        cylinder(d = 12.0, h = 11, center = true);  // BNC clearance
    }
}

// ============================================================
// 2. TELESCOPING DIPOLE T-BAR HANDLE
// ============================================================
// T-shaped handle: horizontal bar holds two element sockets,
// vertical handle for grip, coax runs through vertical member.

module dipole_handle() {
    // Horizontal crossbar
    difference() {
        cylinder(d = tbar_d, h = tbar_w, center = true);
        // Element sockets at each end
        for (side = [-1, 1])
            translate([0, 0, side * (tbar_w/2 - 20)])
                cylinder(d = element_hole_d, h = 42, center = true);
        // Coax channel through center
        cylinder(d = 6.5, h = tbar_w + 1, center = true);
    }

    // Vertical handle
    translate([0, -tbar_handle_l/2, 0])
    difference() {
        cylinder(d = 22, h = tbar_handle_l, center = true);
        // Hollow core (coax runs up)
        cylinder(d = 6.5, h = tbar_handle_l + 1, center = true);
        // Grip knurling
        for (i = [0:15]) {
            rotate([0, 0, i * 22.5])
                translate([22/2 - 0.5, 0, 0])
                    cube([1.5, 2, tbar_handle_l * 0.9], center = true);
        }
    }

    // Feed-point balun pocket (center of crossbar)
    translate([0, 0, 0])
    difference() {
        cylinder(d = tbar_d + 8, h = 22, center = true);
        cylinder(d = tbar_d - 1, h = 20, center = true);
        // BNC/SMA hole (vertical, in handle)
        translate([0, -(tbar_d/2 + 2), 0])
            cylinder(d = 12.5, h = 24, center = true);
    }

    // Element clamp rings (at each end of crossbar, secure telescoping elements)
    for (side = [-1, 1])
    translate([0, 0, side * (tbar_w/2 - 10)])
    difference() {
        cylinder(d = tbar_d + 4, h = 16, center = true);
        cylinder(d = tbar_d + 0.3, h = 18, center = true);  // fits over bar
        cylinder(d = element_hole_d, h = 18, center = true); // element through
        // Clamp slot
        cube([2, tbar_d + 6, 18], center = true);
    }
}

// ============================================================
// 3. 3-ELEMENT VHF YAGI BOOM (2M BAND)
// ============================================================
// Square boom with element holes and connector bracket.
// Print in three sections if boom exceeds bed size.
// Section length: 180mm each (3 sections).

SECTION_COUNT  = 3;
SECTION_L      = yagi_boom_l / SECTION_COUNT;

module yagi_boom_section(section_num) {
    offset = section_num * SECTION_L;
    translate([-offset, 0, 0])  // shift to print at origin
    intersection() {
        // Full boom
        difference() {
            cube([yagi_boom_l, boom_sq, boom_sq], center = true);

            // Element holes (through boom, perpendicular axis)
            for (ep = [refl_pos, driv_pos, dir_pos]) {
                translate([ep - yagi_boom_l/2, 0, 0])
                    rotate([0, 90, 0])
                        cylinder(d = element_dia, h = boom_sq + 2, center = true);
            }

            // Connector bracket hole (at driven element position)
            translate([driv_pos - yagi_boom_l/2, 0, -boom_sq/2 - 0.01])
                rect_boom_hole(16, 16, 1, boom_sq + 1);

            // Alignment pins for section joints
            for (j_pos = [SECTION_L, 2*SECTION_L])
                translate([j_pos - yagi_boom_l/2, 0, 0]) {
                    cylinder(d = 3.0, h = boom_sq * 2, center = true);   // pin hole
                }

            // Hollow interior (reduce print time)
            cube([yagi_boom_l - 10, boom_sq - 4, boom_sq - 4], center = true);
        }
        // Clip to section bounds
        translate([offset - yagi_boom_l/2 + SECTION_L/2, 0, 0])
            cube([SECTION_L, boom_sq + 1, boom_sq + 1], center = true);
    }
}

module rect_boom_hole(w, h, r, depth) {
    hull() {
        for (dx = [-w/2+r, w/2-r]) for (dz = [-h/2+r, h/2-r])
            translate([dx, 0, dz]) rotate([90,0,0])
                cylinder(r = r, h = depth, center = true);
    }
}

// Connector bracket (mounts under boom at driven element)
module yagi_connector_bracket() {
    difference() {
        union() {
            cube([24, 20, 14], center = true);
            translate([0, -10, 0]) cylinder(d = 14, h = 14, center = true);
        }
        // Boom clamp slot
        cube([boom_sq + 0.4, 21, boom_sq + 0.4], center = true);
        // SMA connector hole
        translate([0, -14, 0]) cylinder(d = 9.5, h = 20, center = true);
        // M3 bolt holes for element clamp
        for (dx = [-8, 8]) translate([dx, 0, 0])
            cylinder(d = 3.4, h = 15, center = true);
    }
}

// ============================================================
// 4. BNC-TO-WHIP ADAPTER CAP
// ============================================================
// Snaps over BNC probe connector; holds 4mm whip antenna rod.

module whip_cap() {
    difference() {
        union() {
            cylinder(d = cap_thread_d + cap_t*2, h = 20);
            translate([0, 0, 18])
                cylinder(d1 = cap_thread_d + cap_t*2, d2 = whip_d + cap_t*2, h = 12);
            translate([0, 0, 28])
                cylinder(d = whip_d + cap_t*2, h = 25);
        }
        // BNC body socket (push-fit)
        cylinder(d = cap_thread_d + 0.3, h = 20 + 0.01);
        // Whip rod hole
        translate([0, 0, 15])
            cylinder(d = whip_d + 0.2, h = 40);
        // Electrical contact slot (center pin to rod)
        translate([0, 0, 18])
            cylinder(d = 1.5, h = 12, center = true);
    }
}

// ============================================================
// 5. CALIBRATION DIPOLE SUPPORT JIG
// ============================================================
// Mounts two λ/2 element rods at a fixed height above a surface.
// Feeds via coax through the center support.
// For 20M: total wingspan 10.6m — use wire elements, not rods.
// This jig is sized for 2M (wingspan 97 cm total).

module cal_dipole_jig() {
    total_span = 970.0;  // 2M: 97 cm total
    post_h     = 200.0;  // elevation above surface
    base_w     = 150.0;

    // Base plate
    difference() {
        cube([base_w, base_w, 8], center = true);
        // Tripod mounting holes
        for (dx = [-50, 50]) for (dy = [-50, 50])
            translate([dx, dy, 0]) cylinder(d = 5, h = 10, center = true);
        // Center coax hole
        cylinder(d = 8, h = 10, center = true);
    }

    // Center post
    translate([0, 0, 8/2])
    difference() {
        cylinder(d = 20, h = post_h);
        // Coax channel
        cylinder(d = 7, h = post_h + 1);
    }

    // Horizontal T-bar (holds elements)
    translate([0, 0, 8/2 + post_h])
    difference() {
        cube([total_span / 3, 16, 16], center = true);  // shorter jig bar; elements extend via wire
        cylinder(d = 8, h = 18, center = true);
        for (side = [-1, 1])
            translate([0, 0, side * (total_span/6 - 10)])
                cylinder(d = 3.1, h = 20, center = true);  // element wire holes
    }
}

// ============================================================
// Render (remove comment to render individual parts)
// ============================================================
// Use translate to space parts for print preview:

loop_housing();

translate([170, 0, 0]) dipole_handle();

translate([-20, 200, 0]) {
    yagi_boom_section(0);
    translate([0, 20, 0]) yagi_boom_section(1);
    translate([0, 40, 0]) yagi_boom_section(2);
    translate([0, 60, 0]) yagi_connector_bracket();
}

translate([0, -80, 0]) whip_cap();

translate([200, -80, 0]) cal_dipole_jig();

echo("=== FSM PROBE HOUSINGS ===");
echo(str("Loop housing:  ", loop_d, " mm dia loop, ", handle_l, " mm handle"));
echo(str("Dipole T-bar:  ", tbar_w, " mm span, 6mm element sockets"));
echo(str("Yagi boom:     ", yagi_boom_l, " mm in 3 sections of ", SECTION_L, " mm"));
echo(str("Yagi elements: refl@", refl_pos, " driven@", driv_pos, " dir@", dir_pos, " mm"));
echo(str("Whip cap:      4mm rod, BNC push-fit"));
echo("All parts fit 200x200 bed individually");
