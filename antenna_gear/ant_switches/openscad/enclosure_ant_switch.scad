// ============================================================================
// TM-ANT-SW-001  Rev A — Antenna Switch Enclosures
// 3D-printable housings for manual rotary, relay, and motorized switches
// Print bed: 200 × 200 × 200 mm  |  Material: ASA (UV, weatherproof)
// Layer: 0.2 mm  |  Infill: 40% gyroid  |  Walls: 4 perimeters
// ============================================================================
//
// MODULES:
//   1. sp4t_manual_enclosure()    — SP4T rotary switch body, 5× SO-239
//   2. sp4t_manual_lid()          — lid for SP4T body, screw/snap
//   3. sp6t_manual_enclosure()    — SP6T (6-port) switch body
//   4. relay_switch_body()        — 4-port relay + ESP32/CYD housing
//   5. relay_switch_lid()         — lid for relay body
//   6. motorized_switch_body()    — motorized rotary with stepper bay
//   7. motorized_switch_lid()     — lid for motorized body
//   8. panel_insert_so239()       — SO-239 port panel insert (swap out)
//   9. panel_insert_ntype()       — N-type port panel insert
//  10. shaft_coupler_5to6()       — stepper 5mm → switch 6mm shaft coupler
//  11. motor_bracket_28byj()      — mounts 28BYJ-48 stepper to switch shaft
// ============================================================================

$fn = 64;

// ── Tolerances and hardware ─────────────────────────────────────────────────
WALL     = 3.0;
LID_WALL = 2.5;
TOL      = 0.25;

BOLT_M3  = 3.3;
BOLT_M4  = 4.2;
BOLT_M6  = 6.4;

NUT_M3_W = 6.0;  NUT_M3_H = 2.4;
NUT_M4_W = 7.7;  NUT_M4_H = 3.2;

SO239_HOLE  = 15.8;   // SO-239 panel hole (5/8")
SO239_FLANGE = 21.8;  // SO-239 mounting flange OD
SO239_PCD   = 30.0;   // SO-239 mounting hole PCD (4× #6-32 on 30mm circle, 90° apart)
NTYPE_HOLE  = 20.5;   // N-type panel hole
NTYPE_PCD   = 31.8;   // N-type 4-hole PCD

ORIN_W = 3.0;   ORIN_D = 1.8;   // O-ring groove (3mm cord, 1.8mm deep)

// ── Render selection ────────────────────────────────────────────────────────
// SHOW: 0=layout, 1=sp4t body, 2=sp4t lid, 3=sp6t body, 4=relay body,
//       5=relay lid, 6=motor body, 7=motor lid, 8=shaft coupler, 9=motor bracket
SHOW = 0;

if (SHOW == 0) {
    // Print layout — representative parts
    sp4t_manual_enclosure();
    translate([170, 0, 0])  sp4t_manual_lid();
    translate([0, 140, 0])  relay_switch_body();
    translate([170, 140, 0]) shaft_coupler_5to6();
}
if (SHOW == 1) sp4t_manual_enclosure();
if (SHOW == 2) sp4t_manual_lid();
if (SHOW == 3) sp6t_manual_enclosure();
if (SHOW == 4) relay_switch_body();
if (SHOW == 5) relay_switch_lid();
if (SHOW == 6) motorized_switch_body();
if (SHOW == 7) motorized_switch_lid();
if (SHOW == 8) shaft_coupler_5to6();
if (SHOW == 9) motor_bracket_28byj();


// ============================================================================
// 1. SP4T MANUAL SWITCH ENCLOSURE
//    Houses a 4-position ceramic rotary switch (e.g., Grayhill 56D / Alpha)
//    4× SO-239 antenna ports radially on top; 1× SO-239 common on front face
//    Rotary shaft exits through top-center knob hole (3/8-32 bushing)
//    Lid: 4× M4 screw-on with gasket groove (IP54)
// ============================================================================
module sp4t_manual_enclosure() {
    BW = 120;   // box width
    BD = 100;   // box depth
    BH = 70;    // box height (body, no lid)
    T  = WALL;

    // Antenna port positions on top panel (radial from center)
    ant_r   = 38;   // radial distance from center to each SO-239
    ant_a   = [0, 90, 180, 270];  // angles (N, E, S, W)
    cx = BW / 2;
    cy = BD / 2;

    difference() {
        union() {
            // Main body with open top
            difference() {
                cube([BW, BD, BH]);
                translate([T, T, T])
                cube([BW - 2*T, BD - 2*T, BH]);
            }
            // Internal corner pillars for lid screws (M4 inserts)
            for (px = [T + 4, BW - T - 12]) for (py = [T + 4, BD - T - 12])
                translate([px, py, T])
                cube([8, 8, BH - T]);
        }

        // 4× SO-239 antenna port holes (top face)
        for (a = ant_a) {
            ax = cx + ant_r * cos(a);
            ay = cy + ant_r * sin(a);
            // Center hole
            translate([ax, ay, BH - 1])
            cylinder(d = SO239_HOLE, h = T + 2);
            // 4× mounting holes per connector (rotate 45° so diagonal fits)
            for (ma = [45, 135, 225, 315])
                translate([ax + cos(ma)*12.7, ay + sin(ma)*12.7, BH - 1])
                cylinder(d = BOLT_M3, h = T + 2);
        }

        // COMMON SO-239 on front face
        translate([cx, -1, BH * 0.4])
        rotate([-90, 0, 0])
        cylinder(d = SO239_HOLE, h = T + 2);
        for (ma = [45, 135, 225, 315])
            translate([cx + cos(ma)*12.7, -1, BH * 0.4 + sin(ma)*12.7])
            rotate([-90, 0, 0])
            cylinder(d = BOLT_M3, h = T + 2);

        // Rotary shaft hole (top center, 3/8" = 9.5mm for bushing)
        translate([cx, cy, BH - 1])
        cylinder(d = 10.2, h = T + 2);

        // M4 lid screw holes (into corner pillars)
        for (px = [T + 8, BW - T - 8]) for (py = [T + 8, BD - T - 8])
            translate([px, py, BH - 10])
            cylinder(d = BOLT_M4, h = 14);

        // Lid gasket groove (top face perimeter)
        translate([T/2 + 2, T/2 + 2, BH - ORIN_D])
        linear_extrude(height = ORIN_D + 0.5)
        difference() {
            offset(r = 2) square([BW - T - 4, BD - T - 4]);
            offset(r = -ORIN_W) offset(r = 2) square([BW - T - 4, BD - T - 4]);
        }

        // Ground lug hole (rear face, bottom)
        translate([BW * 0.8, BD - 1, T + 8])
        rotate([-90, 0, 0])
        cylinder(d = 6, h = T + 2);

        // Drain hole (bottom face, front-left corner)
        translate([T + 5, T + 5, -1])
        cylinder(d = 4, h = T + 2);
    }
}

// ── SP4T lid ─────────────────────────────────────────────────────────────────
module sp4t_manual_lid() {
    BW = 120;  BD = 100;  T = WALL;
    LH = 12;

    difference() {
        union() {
            cube([BW, BD, LH]);
            // Lip that drops into box opening
            translate([T + TOL, T + TOL, -6])
            cube([BW - 2*T - 2*TOL, BD - 2*T - 2*TOL, 6.5]);
        }
        // 4× SO-239 holes (same positions as body top)
        ant_r = 38; cx = BW/2; cy = BD/2;
        for (a = [0, 90, 180, 270]) {
            ax = cx + ant_r * cos(a);
            ay = cy + ant_r * sin(a);
            translate([ax, ay, -1]) cylinder(d = SO239_HOLE, h = LH + 2);
            for (ma = [45, 135, 225, 315])
                translate([ax + cos(ma)*12.7, ay + sin(ma)*12.7, -1])
                cylinder(d = BOLT_M3, h = LH + 2);
        }
        // Shaft hole
        translate([cx, cy, -1]) cylinder(d = 10.2, h = LH + 2);
        // M4 screw holes
        for (px = [T + 8, BW - T - 8]) for (py = [T + 8, BD - T - 8])
            translate([px, py, -1]) cylinder(d = BOLT_M4, h = LH + 2);
        // Label recess
        translate([20, 15, LH - 0.8]) cube([BW - 40, BD - 30, 1]);
    }
}


// ============================================================================
// 3. SP6T MANUAL SWITCH ENCLOSURE
//    6-position version — slightly larger to accommodate 60° port spacing
//    6× SO-239 on top (60° apart), 1× common on front
// ============================================================================
module sp6t_manual_enclosure() {
    BW = 140;  BD = 120;  BH = 75;  T = WALL;
    ant_r = 45; cx = BW/2; cy = BD/2;

    difference() {
        union() {
            difference() {
                cube([BW, BD, BH]);
                translate([T, T, T]) cube([BW-2*T, BD-2*T, BH]);
            }
            for (px = [T+4, BW-T-12]) for (py = [T+4, BD-T-12])
                translate([px, py, T]) cube([8, 8, BH - T]);
        }

        // 6× SO-239 at 0°, 60°, 120°, 180°, 240°, 300°
        for (a = [0:60:300]) {
            ax = cx + ant_r * cos(a);
            ay = cy + ant_r * sin(a);
            translate([ax, ay, BH-1]) cylinder(d = SO239_HOLE, h = T+2);
            for (ma = [45, 135, 225, 315])
                translate([ax + cos(ma)*12.7, ay + sin(ma)*12.7, BH-1])
                cylinder(d = BOLT_M3, h = T+2);
        }

        translate([cx, -1, BH*0.4])
        rotate([-90, 0, 0]) cylinder(d = SO239_HOLE, h = T+2);

        translate([cx, cy, BH-1]) cylinder(d = 10.2, h = T+2);

        for (px = [T+8, BW-T-8]) for (py = [T+8, BD-T-8])
            translate([px, py, BH-10]) cylinder(d = BOLT_M4, h = 14);

        translate([T/2+2, T/2+2, BH-ORIN_D])
        linear_extrude(height = ORIN_D+0.5)
        difference() {
            offset(r=2) square([BW-T-4, BD-T-4]);
            offset(r=-ORIN_W) offset(r=2) square([BW-T-4, BD-T-4]);
        }

        translate([BW*0.8, BD-1, T+8]) rotate([-90,0,0]) cylinder(d=6, h=T+2);
        translate([T+5, T+5, -1]) cylinder(d=4, h=T+2);
    }
}


// ============================================================================
// 4. RELAY SWITCH BODY WITH ESP32/CYD BAY
//    Houses 4× relay PCB + ESP32 controller + CYD 2.8" TFT display
//    4× SO-239 on rear panel (antenna ports), 1× common on rear
//    CYD display window on front face
//    4× LED holes + 4× push-button holes on front face
// ============================================================================
module relay_switch_body() {
    BW = 190;  BD = 110;  BH = 80;  T = WALL;

    // CYD screen opening (2.8" TFT active area ≈ 57mm × 43mm; add 4mm border)
    CYD_W = 65;  CYD_H = 52;  CYD_X = 18;  CYD_Y = 24;

    difference() {
        union() {
            difference() {
                cube([BW, BD, BH]);
                translate([T, T, T]) cube([BW-2*T, BD-2*T, BH]);
            }
            // Corner pillars (lid screws)
            for (px = [T+3, BW-T-11]) for (py = [T+3, BD-T-11])
                translate([px, py, T]) cube([8, 8, BH-T]);
            // PCB standoffs (4× per PCB, two PCBs)
            for (px = [20, BW-20]) for (py = [20, BD-20])
                translate([px-3, py-3, T]) cylinder(d=6, h=6);
        }

        // FRONT FACE (y=0):
        // CYD display window
        translate([CYD_X, -1, CYD_Y]) cube([CYD_W, T+2, CYD_H]);
        // 4× LED holes (5mm, row above display)
        for (i = [0:3])
            translate([20 + i*38, -1, BH-18]) rotate([-90,0,0])
            cylinder(d=5.2, h=T+2);
        // 4× push buttons (6mm, row below LEDs)
        for (i = [0:3])
            translate([20 + i*38, -1, BH-30]) rotate([-90,0,0])
            cylinder(d=6.8, h=T+2);
        // Power LED (small, right side of display)
        translate([CYD_X + CYD_W + 8, -1, CYD_Y + CYD_H/2]) rotate([-90,0,0])
        cylinder(d=3.2, h=T+2);

        // REAR FACE (y=BD):
        // 4× SO-239 antenna ports (row)
        for (i = [0:3])
            translate([30 + i*38, BD-1, BH*0.5]) rotate([-90,0,0]) {
                cylinder(d=SO239_HOLE, h=T+2);
                for (ma = [45, 135, 225, 315])
                    translate([cos(ma)*12.7, sin(ma)*12.7, 0])
                    cylinder(d=BOLT_M3, h=T+2);
            }
        // COMMON SO-239
        translate([BW-25, BD-1, BH*0.5]) rotate([-90,0,0]) {
            cylinder(d=SO239_HOLE, h=T+2);
            for (ma = [45, 135, 225, 315])
                translate([cos(ma)*12.7, sin(ma)*12.7, 0])
                cylinder(d=BOLT_M3, h=T+2);
        }

        // RIGHT SIDE: power jack (2.1mm barrel), TX-inhibit jack (3.5mm)
        translate([BW-1, BD*0.35, T+12]) rotate([0,90,0]) cylinder(d=6.5, h=T+2);
        translate([BW-1, BD*0.55, T+12]) rotate([0,90,0]) cylinder(d=3.8, h=T+2);

        // Lid gasket groove
        translate([T/2+2, T/2+2, BH-ORIN_D])
        linear_extrude(height=ORIN_D+0.5)
        difference() {
            offset(r=2) square([BW-T-4, BD-T-4]);
            offset(r=-ORIN_W) offset(r=2) square([BW-T-4, BD-T-4]);
        }

        // M4 lid screw holes
        for (px = [T+7, BW-T-7]) for (py = [T+7, BD-T-7])
            translate([px, py, BH-10]) cylinder(d=BOLT_M4, h=14);

        // Ground lug
        translate([15, BD-1, T+8]) rotate([-90,0,0]) cylinder(d=6, h=T+2);

        // Vent slots (left side, near ESP32 — passive cooling)
        for (z = [T+15, T+30])
            translate([-1, BD*0.3, z]) cube([T+2, BD*0.4, 3]);

        // Drain
        translate([T+5, T+5, -1]) cylinder(d=4, h=T+2);
    }
}

// ── Relay switch lid ──────────────────────────────────────────────────────────
module relay_switch_lid() {
    BW = 190;  BD = 110;  T = WALL;  LH = 10;
    difference() {
        union() {
            cube([BW, BD, LH]);
            translate([T+TOL, T+TOL, -5]) cube([BW-2*T-2*TOL, BD-2*T-2*TOL, 5.5]);
        }
        // Stiffening cutout (weight saving; keep 15mm frame)
        translate([25, 25, LH-0.8]) cube([BW-50, BD-50, 2]);
        // M4 screw holes
        for (px = [T+7, BW-T-7]) for (py = [T+7, BD-T-7])
            translate([px, py, -1]) cylinder(d=BOLT_M4, h=LH+2);
    }
}


// ============================================================================
// 6. MOTORIZED SWITCH BODY
//    Houses rotary switch + 28BYJ-48 stepper in motor bay
//    ESP32 in control bay; CYD display on front
//    4× SO-239 ports on top (same as SP4T manual)
//    Stepper motor bay at rear with motor exit slot for shaft coupling
// ============================================================================
module motorized_switch_body() {
    BW = 160;  BD = 130;  BH = 100;  T = WALL;

    // Motor bay dimensions (28BYJ-48: 28mm OD, 19mm long body)
    MB_W = 40;  MB_D = 40;  MB_Z = T + 15;

    ant_r = 42; cx = BW/2; cy = BD/2;

    difference() {
        union() {
            difference() {
                cube([BW, BD, BH]);
                translate([T, T, T]) cube([BW-2*T, BD-2*T, BH]);
            }
            // Corner pillars
            for (px = [T+3, BW-T-11]) for (py = [T+3, BD-T-11])
                translate([px, py, T]) cube([8, 8, BH-T]);
            // PCB standoffs
            for (px = [20, BW-20]) for (py = [20, BD/2])
                translate([px-3, py-3, T]) cylinder(d=6, h=6);
        }

        // TOP: 4× SO-239 antenna ports (same as sp4t)
        for (a = [0, 90, 180, 270]) {
            ax = cx + ant_r*cos(a);
            ay = cy + ant_r*sin(a);
            translate([ax, ay, BH-1]) cylinder(d=SO239_HOLE, h=T+2);
            for (ma = [45,135,225,315])
                translate([ax+cos(ma)*12.7, ay+sin(ma)*12.7, BH-1])
                cylinder(d=BOLT_M3, h=T+2);
        }

        // TOP: rotary switch shaft hole
        translate([cx, cy, BH-1]) cylinder(d=10.2, h=T+2);

        // FRONT: CYD display window
        translate([18, -1, 28]) cube([65, T+2, 52]);
        // LEDs (4×)
        for (i = [0:3])
            translate([18 + i*36, -1, BH-20]) rotate([-90,0,0]) cylinder(d=5.2, h=T+2);
        // Buttons (2× forward/reverse)
        translate([BW-35, -1, BH-30]) rotate([-90,0,0]) cylinder(d=7, h=T+2);
        translate([BW-15, -1, BH-30]) rotate([-90,0,0]) cylinder(d=7, h=T+2);

        // REAR: motor exit hole (stepper shaft + coupling clearance)
        translate([cx-20, BD-1, MB_Z]) cube([40, T+2, 40]);

        // RIGHT SIDE: power + TX jack
        translate([BW-1, BD*0.3, T+12]) rotate([0,90,0]) cylinder(d=6.5, h=T+2);
        translate([BW-1, BD*0.5, T+12]) rotate([0,90,0]) cylinder(d=3.8, h=T+2);

        // Hall sensor wire holes (4× small holes in switch bay)
        for (i = [0:3])
            translate([cx + 25*cos(i*90), cy + 25*sin(i*90), BH-1])
            cylinder(d=3, h=T+2);

        // Lid gasket groove
        translate([T/2+2, T/2+2, BH-ORIN_D])
        linear_extrude(height=ORIN_D+0.5)
        difference() {
            offset(r=2) square([BW-T-4, BD-T-4]);
            offset(r=-ORIN_W) offset(r=2) square([BW-T-4, BD-T-4]);
        }

        // M4 lid screws
        for (px = [T+7, BW-T-7]) for (py = [T+7, BD-T-7])
            translate([px, py, BH-10]) cylinder(d=BOLT_M4, h=14);

        // Vent / drain
        translate([T+5, T+5, -1]) cylinder(d=4, h=T+2);
    }
}

// ── Motorized switch lid ──────────────────────────────────────────────────────
module motorized_switch_lid() {
    BW = 160;  BD = 130;  T = WALL;  LH = 10;
    ant_r = 42; cx = BW/2; cy = BD/2;
    difference() {
        union() {
            cube([BW, BD, LH]);
            translate([T+TOL, T+TOL, -5]) cube([BW-2*T-2*TOL, BD-2*T-2*TOL, 5.5]);
        }
        // 4× SO-239 pass-through
        for (a = [0,90,180,270]) {
            ax = cx + ant_r*cos(a); ay = cy + ant_r*sin(a);
            translate([ax, ay, -1]) cylinder(d=SO239_HOLE, h=LH+2);
            for (ma=[45,135,225,315])
                translate([ax+cos(ma)*12.7, ay+sin(ma)*12.7, -1])
                cylinder(d=BOLT_M3, h=LH+2);
        }
        translate([cx, cy, -1]) cylinder(d=10.2, h=LH+2);
        // M4 screw holes
        for (px = [T+7, BW-T-7]) for (py = [T+7, BD-T-7])
            translate([px, py, -1]) cylinder(d=BOLT_M4, h=LH+2);
        // Label recess
        translate([20, 20, LH-0.8]) cube([BW-40, BD-40, 1]);
    }
}


// ============================================================================
// 8. PANEL INSERT — SO-239
//    Flat plate (2mm) for swapping connector type.
//    4× screw holes for M3 machine screws into box side.
// ============================================================================
module panel_insert_so239(port_count=4) {
    PW = 30 + (port_count - 1) * 38;
    PH = 35;
    PT = 2.5;
    difference() {
        cube([PW, PT, PH]);
        for (i = [0:port_count-1]) {
            x = 15 + i * 38;
            translate([x, -1, PH/2]) rotate([-90,0,0]) {
                cylinder(d=SO239_HOLE, h=PT+2);
                for (ma=[45,135,225,315])
                    translate([cos(ma)*12.7, sin(ma)*12.7, 0])
                    cylinder(d=BOLT_M3, h=PT+2);
            }
        }
        // 4× corner mount holes
        for (cx = [6, PW-6])
            for (cz = [6, PH-6])
                translate([cx, -1, cz]) rotate([-90,0,0]) cylinder(d=BOLT_M3, h=PT+2);
    }
}


// ============================================================================
// 9. PANEL INSERT — N-TYPE
//    Same footprint as SO-239 insert; N-type hole pattern.
// ============================================================================
module panel_insert_ntype(port_count=4) {
    PW = 32 + (port_count - 1) * 40;
    PH = 38;
    PT = 2.5;
    difference() {
        cube([PW, PT, PH]);
        for (i = [0:port_count-1]) {
            x = 16 + i * 40;
            translate([x, -1, PH/2]) rotate([-90,0,0]) {
                cylinder(d=NTYPE_HOLE, h=PT+2);
                for (ma=[45,135,225,315])
                    translate([cos(ma)*NTYPE_PCD/2, sin(ma)*NTYPE_PCD/2, 0])
                    cylinder(d=BOLT_M4, h=PT+2);
            }
        }
        for (cx = [6, PW-6]) for (cz = [6, PH-6])
            translate([cx, -1, cz]) rotate([-90,0,0]) cylinder(d=BOLT_M3, h=PT+2);
    }
}


// ============================================================================
// 10. SHAFT COUPLER — 5mm to 6mm (stepper to rotary switch)
//     Rigid hub with M3 set screws on each side.
//     Slit on 5mm side allows compression clamping.
// ============================================================================
module shaft_coupler_5to6() {
    OD = 18;
    LEN = 30;
    difference() {
        cylinder(d=OD, h=LEN);
        // 5mm bore (motor side, first half)
        translate([0, 0, -1]) cylinder(d=5.2 + TOL, h=LEN/2 + 1);
        // 6mm bore (switch side, second half)
        translate([0, 0, LEN/2]) cylinder(d=6.2 + TOL, h=LEN/2 + 1);
        // Slit for compression (5mm side)
        translate([-0.8, -OD/2 - 1, -1]) cube([1.6, OD + 2, LEN/2 + 3]);
        // M3 set screw holes (2 per side, 90° apart)
        for (a = [0, 90])
            rotate([0, 0, a]) {
                // Motor side set screw
                translate([OD/2 + 1, 0, LEN * 0.25])
                rotate([0, 90, 0]) cylinder(d=BOLT_M3, h=OD/2 + 2);
                // Switch side set screw
                translate([OD/2 + 1, 0, LEN * 0.75])
                rotate([0, 90, 0]) cylinder(d=BOLT_M3, h=OD/2 + 2);
            }
        // M3 nut traps (hex, recessed) — set screw side
        for (a = [0, 90])
            rotate([0, 0, a]) {
                translate([OD/2 - NUT_M3_H, 0, LEN * 0.25])
                rotate([0, 90, 0]) cylinder(d=NUT_M3_W, h=NUT_M3_H + 1, $fn=6);
                translate([OD/2 - NUT_M3_H, 0, LEN * 0.75])
                rotate([0, 90, 0]) cylinder(d=NUT_M3_W, h=NUT_M3_H + 1, $fn=6);
            }
    }
}


// ============================================================================
// 11. MOTOR BRACKET — 28BYJ-48
//     Mounts the 28BYJ-48 stepper to the switch enclosure floor or rear wall.
//     Motor registers against a circular boss; 2× M3 screws secure it.
//     Aligns motor shaft coaxially with switch shaft.
// ============================================================================
module motor_bracket_28byj() {
    // 28BYJ-48 body: 28.5mm OD, 19mm body length, shaft at center
    // Mounting tabs: 35mm apart, 4mm holes
    M_OD   = 29.0;   // motor body OD + clearance
    M_LEN  = 21.0;   // motor body length (housing)
    TAB_SP = 35.0;   // mounting tab hole spacing

    BW = TAB_SP + 20;
    BD = M_OD / 2 + 20;
    BH = WALL + M_LEN;

    difference() {
        union() {
            cube([BW, BD, BH]);
            // Raised collar (locates motor OD)
            translate([BW/2, M_OD/2 + WALL, WALL])
            cylinder(d=M_OD + 4, h=5);
        }
        // Motor bore (clearance for motor body)
        translate([BW/2, M_OD/2 + WALL, WALL - 1])
        cylinder(d=M_OD + TOL*2, h=M_LEN + 2);
        // Shaft exit hole (top face; 5mm shaft + coupler clearance)
        translate([BW/2, M_OD/2 + WALL, -1])
        cylinder(d=6.5, h=WALL + 2);
        // Motor mounting tab holes (2× M3)
        for (dx = [-TAB_SP/2, TAB_SP/2])
            translate([BW/2 + dx, M_OD/2 + WALL, -1])
            cylinder(d=BOLT_M3, h=WALL + 2);
        // Bracket mounting holes to enclosure (4× M3)
        for (px = [6, BW-6]) for (py = [6, BD-6])
            translate([px, py, -1]) cylinder(d=BOLT_M3, h=WALL + 2);
        // Wire routing slot (for stepper cable exit)
        translate([BW/2 - 5, BD - WALL - 0.1, WALL])
        cube([10, WALL + 2, 10]);
    }
}

// ============================================================================
// END OF FILE
// ============================================================================
